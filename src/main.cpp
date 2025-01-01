#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include <boost/json/src.hpp>
#include <InfluxDB/InfluxDB.h>
#include <InfluxDB/InfluxDBFactory.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <ranges>
#include <sstream>
#include <string>

#include <fmt/format.h>

std::string const HOSTNAME = "homelab";

struct Config
{
  std::string hostname;
  std::string influxdb_url;
  std::string pihole_endpoint;
  std::string pihole_token;

  static Config make_from_env()
  {
    Config config{};
    if (auto const hostname = std::getenv("HOSTNAME")) {
      config.hostname = hostname;
    }
    if (auto const url = std::getenv("INFLUXDB_URL")) {
      config.influxdb_url = url;
    }
    if (auto const endpoint = std::getenv("PIHOLE_ENDPOINT")) {
      config.pihole_endpoint = endpoint;
    }
    if (auto const token = std::getenv("PIHOLE_TOKEN")) {
      config.pihole_token = token;
    }
    return config;
  }
};

struct PiholeData
{
  std::vector<std::pair<int, int>> domains_over_time;
  std::vector<std::pair<int, int>> ads_over_time;
};

std::optional<boost::json::object> get_stats(std::string_view endpoint,
                                             std::string_view token)
{
  try {
    auto const url = fmt::format("{}/admin/api.php?overTimeData10mins&auth={}",
                                 endpoint, token);
    curlpp::Easy request;
    request.setOpt<curlpp::options::Url>(url);
    std::stringstream iss;
    curlpp::options::WriteStream ws{&iss};
    request.setOpt(ws);
    request.perform();
    return boost::json::parse(iss.str()).as_object();
  } catch (curlpp::RuntimeError& e) {
    std::cout << e.what() << std::endl;
  } catch (curlpp::LogicError& e) {
    std::cout << e.what() << std::endl;
  }
  return std::nullopt;
}

PiholeData parse_data(boost::json::object& jo)
{
  class Parser
  {
   public:
    auto operator()(boost::json::key_value_pair& kv)
    {
      return std::pair{std::stoi(kv.key()), kv.value().as_int64()};
    }
  };
  PiholeData data{};
  auto domains = jo["domains_over_time"].as_object();
  auto ads     = jo["ads_over_time"].as_object();
  std::transform(domains.begin(), domains.end(),
                 std::back_inserter(data.domains_over_time), Parser{});
  std::transform(ads.begin(), ads.end(), std::back_inserter(data.ads_over_time),
                 Parser{});
  return data;
}

void send_to_influx(PiholeData const& data, std::string const& url)
{
  try {
    influxdb::Point measurement{fmt::format("pihole.{}", HOSTNAME)};
    measurement.addTag("host", HOSTNAME);
    measurement.addField("dns_queries", data.domains_over_time.back().second);
    measurement.addField("ads_blocked", data.ads_over_time.back().second);
    auto influxdb = influxdb::InfluxDBFactory::Get(url);
    influxdb->write(std::move(measurement));
    std::cout << "data sent\n";
  } catch (std::exception const& e) {
    std::cout << "An exception occurred sending data to InfluxDB: " << e.what()
              << '\n';
  }
}

int main()
{
  auto config = Config::make_from_env();
  // Get stats from pihole
  auto result = get_stats(config.pihole_endpoint, config.pihole_token);
  if (!result.has_value()) {
    std::cerr << "Failed to get data from Pi Hole\n";
    return EXIT_FAILURE;
  }
  auto data = parse_data(result.value());
  std::cout << fmt::format(
      "found {} domains and {} blocked ads over last 10 minutes\n",
      data.domains_over_time.size(), data.ads_over_time.size());
  // Send data to influx 
  send_to_influx(data, config.influxdb_url);
  return EXIT_SUCCESS;
}

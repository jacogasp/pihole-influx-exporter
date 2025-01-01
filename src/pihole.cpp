#include "pihole.hpp"

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <fmt/format.h>

#include <exception>
#include <sstream>

PiHole::PiHole(std::string_view endpoint, std::string_view token)
    : m_url{fmt::format("{}/admin/api.php?overTimeData10mins&auth={}", endpoint,
                        token)}
{}

std::optional<boost::json::object> PiHole::curl_stats()
{
  try {
    curlpp::Easy request;
    request.setOpt<curlpp::options::Url>(m_url);
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

Data PiHole::poll_data()
{
  auto const parser = [](boost::json::key_value_pair& kv) {
    return kv.value().as_int64();
  };
  auto stats = curl_stats();
  if (!stats.has_value()) {
    throw std::runtime_error("failed to get data from Pi Hole");
  }

  auto& jo         = stats.value();
  auto domains_obj = jo["domains_over_time"].as_object();
  auto ads_obj     = jo["ads_over_time"].as_object();
  std::vector<int> domains{};
  std::vector<int> ads{};
  std::transform(domains_obj.begin(), domains_obj.end(),
                 std::back_inserter(domains), parser);
  std::transform(ads_obj.begin(), ads_obj.end(), std::back_inserter(ads),
                 parser);
  return {domains.back(), ads.back()};
}

#include "pihole.hpp"

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <fmt/format.h>

#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <exception>
#include <sstream>
#include <string>

PiHole::PiHole(std::string_view endpoint, std::string_view token)
    : m_endpoint{fmt::format("{}/admin/api.php?auth={}", endpoint, token)}
{}

std::optional<boost::json::object> PiHole::curl_api(std::string const& query)
{
  try {
    std::cout << "requested data to pihole...";
    auto const url{m_endpoint + query};
    curlpp::Easy request;
    request.setOpt<curlpp::options::Url>(url);
    std::stringstream iss;
    curlpp::options::WriteStream ws{&iss};
    request.setOpt(ws);
    request.perform();
    auto const resp = iss.str();
    std::cout << " received " << resp.size() << " bytes.\n";
    return boost::json::parse(resp).as_object();
  } catch (curlpp::RuntimeError& e) {
    std::cout << e.what() << std::endl;
  } catch (curlpp::LogicError& e) {
    std::cout << e.what() << std::endl;
  }
  return std::nullopt;
}

std::optional<Data> PiHole::poll_data()
{
  auto jo_opt = curl_api("&summaryRaw&overTimeData&topItems&recentItems&"
                         "getQueryTypes&getForwardDestinations&getQuerySources&"
                         "overTimeData10mins&jsonForceObject");
  if (!jo_opt.has_value()) {
    return std::nullopt;
  }
  auto& jo = jo_opt.value();

  Data data{
      static_cast<int>(jo["ads_blocked_today"].as_int64()),
      jo["ads_percentage_today"].as_double(),
      static_cast<int>(jo["clients_ever_seen"].as_int64()),
      static_cast<int>(jo["dns_queries_all_replies"].as_int64()),
      static_cast<int>(jo["dns_queries_today"].as_int64()),
      static_cast<int>(jo["domains_being_blocked"].as_int64()),
      static_cast<int>(jo["queries_cached"].as_int64()),
      static_cast<int>(jo["queries_forwarded"].as_int64()),
      static_cast<int>(jo["unique_clients"].as_int64()),
      static_cast<int>(jo["unique_domains"].as_int64()),
  };

  auto to_int = [](auto const& src_jo, auto& dst) {
    std::transform(
        src_jo.begin(), src_jo.end(), std::back_inserter(dst),
        [](auto& kv) { return std::pair{kv.key(), kv.value().as_int64()}; });
  };

  auto to_double = [](auto const& src_jo, auto& dst) {
    std::transform(
        src_jo.begin(), src_jo.end(), std::back_inserter(dst), [](auto& kv) {
          std::string key{kv.key().begin(), kv.key().end()};
          std::replace(key.begin(), key.end(), ' ', '_');
          return std::pair{key, kv.value().kind() == boost::json::kind::int64
                                    ? static_cast<double>(kv.value().as_int64())
                                    : kv.value().as_double()};
        });
  };

  to_int(jo["top_queries"].as_object(), data.top_queries);
  to_int(jo["top_ads"].as_object(), data.top_ads);
  to_int(jo["top_sources"].as_object(), data.top_sources);
  to_double(jo["forward_destinations"].as_object(), data.forward_destinations);
  to_double(jo["querytypes"].as_object(), data.querytypes);
  return data;
}

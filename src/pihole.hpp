#ifndef PIHOLE_HPP
#define PIHOLE_HPP

#include <boost/json.hpp>
#include <optional>
#include <string>

#include "data.hpp"

class PiHole
{
 private:
  std::string m_endpoint;
  std::optional<boost::json::object> curl_api(std::string const& query);

 public:
  PiHole(std::string_view endpoint, std::string_view token);
  std::optional<Data> poll_data();
};

#endif

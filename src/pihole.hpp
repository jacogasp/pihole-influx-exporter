#ifndef PIHOLE_HPP
#define PIHOLE_HPP

#include <optional>
#include <string>

#include "data.hpp"

#include <boost/json.hpp>

class PiHole
{
 private:
  std::string m_url;
  std::optional<boost::json::object> curl_stats();

 public:
  PiHole(std::string_view endpoint, std::string_view token);
  Data poll_data();
};

#endif

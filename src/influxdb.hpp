#ifndef INFLUXDB_HPP
#define INFLUXDB_HPP

#include "data.hpp"
#include <string>

class InfluxDB
{
  std::string m_url;
  std::string m_host;

 public:
  InfluxDB(std::string_view url, std::string_view host);
  void send_data(Data const& data);
};

#endif

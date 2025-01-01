#include "influxdb.hpp"

#include <InfluxDB/InfluxDB.h>
#include <InfluxDB/InfluxDBFactory.h>
#include <fmt/format.h>
#include <iostream>

InfluxDB::InfluxDB(std::string_view url, std::string_view hostname)
    : m_url{url}
    , m_hostname{hostname}
{}

void InfluxDB::send_data(Data const& data)
{
  try {
    influxdb::Point measurement{"pihole." + m_hostname};
    measurement.addTag("host", m_hostname);
    measurement.addField("dns_queries", data.domains_over_time);
    measurement.addField("ads_blocked", data.ads_over_time);
    auto influxdb = influxdb::InfluxDBFactory::Get(m_url);
    influxdb->write(std::move(measurement));
    std::cout << fmt::format(
        "data sent. found {} dns_queries and {} ads blocked\n",
        data.domains_over_time, data.ads_over_time);
  } catch (std::exception const& e) {
    std::cout << "An exception occurred sending data to InfluxDB: " << e.what()
              << '\n';
  }
}

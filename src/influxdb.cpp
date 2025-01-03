#include "influxdb.hpp"

#include <InfluxDB/InfluxDB.h>
#include <InfluxDB/InfluxDBFactory.h>

#include <fmt/format.h>
#include <iostream>
#include <string>

auto constexpr MEASUREMENT_NAME = "pihole";

InfluxDB::InfluxDB(std::string_view url, std::string_view hostname)
    : m_url{url}
    , m_host{hostname}
{}

void make_summary(auto influxdb, Data const& data, std::string_view host)
{
  influxdb::Point measure{MEASUREMENT_NAME};
  measure.addTag("host", host);
  measure.addField("status", data.status);
  measure.addField("ads_blocked_today", data.ads_blocked_today);
  measure.addField("ads_percentage_today", data.ads_percentage_today);
  measure.addField("clients_ever_seen", data.clients_ever_seen);
  measure.addField("dns_queries_all_replies", data.dns_queries_all_replies);
  measure.addField("dns_quries_today", data.dns_queries_today);
  measure.addField("domains_being_blocked", data.domains_being_blocked);
  measure.addField("queries_cached", data.queries_cached);
  measure.addField("queries_forwarded", data.queries_forwarded);
  measure.addField("unique_clients", data.unique_clients);
  measure.addField("unique_domains", data.unique_domains);
  influxdb->write(std::move(measure));
}

void make_top_queries(auto influxdb, Data const& data, std::string_view host)
{
  for (auto&& query : data.top_queries) {
    influxdb::Point measure{MEASUREMENT_NAME};
    measure.addTag("host", host);
    measure.addTag("domain", query.first);
    measure.addField("top_queries", query.second);
    influxdb->write(std::move(measure));
  }
}

void make_top_ads(auto influxdb, Data const& data, std::string_view host)
{
  for (auto&& ad : data.top_ads) {
    influxdb::Point measure{MEASUREMENT_NAME};
    measure.addTag("host", host);
    measure.addTag("domain", ad.first);
    measure.addField("top_ads", ad.second);
    influxdb->write(std::move(measure));
  }
}

void make_top_sources(auto influxdb, Data const& data, std::string_view host)
{
  for (auto&& source : data.top_sources) {
    influxdb::Point measure{MEASUREMENT_NAME};
    measure.addTag("host", host);
    measure.addTag("domain", source.first);
    measure.addField("top_sources", source.second);
    influxdb->write(std::move(measure));
  }
}

void make_forward_destinations(auto influxdb, Data const& data,
                               std::string_view host)
{
  for (auto&& dest : data.forward_destinations) {
    influxdb::Point measure{MEASUREMENT_NAME};
    measure.addTag("host", host);
    measure.addTag("domain", dest.first);
    measure.addField("forward_destinations", dest.second);
    influxdb->write(std::move(measure));
  }
}

void make_querytypes(auto influxdb, Data const& data, std::string_view host)
{
  for (auto&& qt : data.querytypes) {
    influxdb::Point measure{MEASUREMENT_NAME};
    measure.addTag("host", host);
    measure.addTag("domain", qt.first);
    measure.addField("querytypes", qt.second);
    influxdb->write(std::move(measure));
  }
}

void InfluxDB::send_data(Data const& data)
{
  try {
    auto influxdb = influxdb::InfluxDBFactory::Get(m_url);
    make_summary(influxdb.get(), data, m_host);
    make_top_queries(influxdb.get(), data, m_host);
    make_top_ads(influxdb.get(), data, m_host);
    make_top_sources(influxdb.get(), data, m_host);
    make_forward_destinations(influxdb.get(), data, m_host);
    make_querytypes(influxdb.get(), data, m_host);
    print_data(data);
    std::cout << "data sent to influxdb." << std::endl;
  } catch (std::exception const& e) {
    std::cout << "An exception occurred sending data to InfluxDB: " << e.what()
              << '\n';
  }
}

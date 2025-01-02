#include "influxdb.hpp"

#include <InfluxDB/InfluxDB.h>
#include <InfluxDB/InfluxDBFactory.h>
#include <fmt/format.h>
#include <iostream>

InfluxDB::InfluxDB(std::string_view url, std::string_view hostname)
    : m_url{url}
    , m_host{hostname}
{}

auto make_summary(Data const& data, std::string_view host)
{
  influxdb::Point summary{"summary"};
  summary.addTag("host", host);
  summary.addField("pihole_ads_blocked_today", data.ads_blocked_today);
  summary.addField("pihole_ads_percentage_today", data.ads_percentage_today);
  summary.addField("pihole_clients_ever_seen", data.clients_ever_seen);
  summary.addField("pihole_dns_queries_all_replies",
                   data.dns_queries_all_replies);
  summary.addField("pihole_dns_quries_today", data.dns_queries_today);
  summary.addField("pihole_domains_being_blocked", data.domains_being_blocked);
  summary.addField("pihole_queries_cached", data.queries_cached);
  summary.addField("pihole_queries_forwarded", data.queries_forwarded);
  summary.addField("pihole_unique_clients", data.unique_clients);
  summary.addField("pihole_unique_domains", data.unique_domains);
  return summary;
}

auto make_top_queries(Data const& data, std::string_view host)
{
  influxdb::Point top_queries{"pihole_top_queries"};
  top_queries.addTag("host", host);
  for (auto&& query : data.top_queries) {
    top_queries.addField(query.first, query.second);
  }
  return top_queries;
}

auto make_top_ads(Data const& data, std::string_view host)
{
  influxdb::Point top_ads{"pihole_top_ads"};
  top_ads.addTag("host", host);
  for (auto&& ad : data.top_ads) {
    top_ads.addField(ad.first, ad.second);
  }
  return top_ads;
}

auto make_top_sources(Data const& data, std::string_view host)
{
  influxdb::Point top_sources{"pihole_top_sources"};
  top_sources.addTag("host", host);
  for (auto&& source : data.top_sources) {
    top_sources.addField(source.first, source.second);
  }
  return top_sources;
}

auto make_forward_destinations(Data const& data, std::string_view host)
{
  influxdb::Point forward_destinations{"pihole_forward_destionations"};
  forward_destinations.addTag("host", host);
  for (auto&& dest : data.forward_destinations) {
    forward_destinations.addField(dest.first, dest.second);
  }
  return forward_destinations;
}

auto make_querytypes(Data const& data, std::string_view host)
{
  influxdb::Point querytypes{"pihole_querytypes"};
  querytypes.addTag("host", host);
  for (auto&& qt : data.querytypes) {
    querytypes.addField(qt.first, qt.second);
  }
  return querytypes;
}

void InfluxDB::send_data(Data const& data)
{
  try {
    auto influxdb = influxdb::InfluxDBFactory::Get(m_url);
    influxdb->write(make_summary(data, m_host));
    influxdb->write(make_top_queries(data, m_host));
    influxdb->write(make_top_ads(data, m_host));
    influxdb->write(make_top_sources(data, m_host));
    influxdb->write(make_forward_destinations(data, m_host));
    influxdb->write(make_querytypes(data, m_host));
    print_data(data);
    std::cout << "data sent to influxdb.\n";
  } catch (std::exception const& e) {
    std::cout << "An exception occurred sending data to InfluxDB: " << e.what()
              << '\n';
  }
}

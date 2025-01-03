#ifndef DATA_HPP
#define DATA_HPP

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using top_queries_t          = std::vector<std::pair<std::string, int>>;
using top_ads_t              = std::vector<std::pair<std::string, int>>;
using top_sources_t          = std::vector<std::pair<std::string, int>>;
using forward_destinations_t = std::vector<std::pair<std::string, double>>;
using querytypes_t           = std::vector<std::pair<std::string, double>>;

struct Data
{
  bool status;
  int ads_blocked_today;
  double ads_percentage_today;
  int clients_ever_seen;
  int dns_queries_all_replies;
  int dns_queries_today;
  int domains_being_blocked;
  int queries_cached;
  int queries_forwarded;
  int unique_clients;
  int unique_domains;
  top_queries_t top_queries;
  top_ads_t top_ads;
  top_sources_t top_sources;
  forward_destinations_t forward_destinations;
  querytypes_t querytypes;
};

void print_data(Data const& data);

template<typename T>
void print_stats(T const& stats)
{
  for (auto&& stat : stats) {
    std::cout << stat.first << ' ' << stat.second << '\n';
  }
}

#endif

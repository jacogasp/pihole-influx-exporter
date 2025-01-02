#include "data.hpp"
#include <fmt/format.h>

void print_data(Data const& data)
{
  std::cout << fmt::format(
      "\n*** Summary ***\n"
      "status: {}\n"
      "ads_blocked_today: {}\nads_percentage_today: "
      "{}\nclients_ever_seen: {}\ndns_queries_all_replies: "
      "{}\ndns_queries_today: "
      "{}\ndomains_being_blocked: "
      "{}\nquery_cached: {}\nquery_forwarded: "
      "{}\nunique_clients: {}\nunique_domains: {}\n"
      "*** End Summary ***\n\n",
      data.status, data.ads_blocked_today, data.ads_percentage_today,
      data.clients_ever_seen, data.dns_queries_all_replies,
      data.dns_queries_today, data.domains_being_blocked, data.queries_cached,
      data.queries_forwarded, data.unique_clients, data.unique_domains);
}

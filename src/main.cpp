#include "config.hpp"
#include "influxdb.hpp"
#include "pihole.hpp"

int main()
{
  auto config = Config::make_from_env();
  PiHole pihole{config.pihole_endpoint, config.pihole_token};
  InfluxDB influxdb{config.influxdb_url, config.hostname};
  auto data = pihole.poll_data();
  influxdb.send_data(data);
  return 0;
}

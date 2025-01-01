#include "config.hpp"
#include "influxdb.hpp"
#include "pihole.hpp"
#include "timer.hpp"

#include <boost/asio.hpp>
#include <iostream>

int main()
{
  auto config = Config::make_from_env();
  PiHole pihole{config.pihole_endpoint, config.pihole_token};
  InfluxDB influxdb{config.influxdb_url, config.hostname};

  std::cout << "Pihole Influx Exporter started. Hostname: " << config.hostname
            << ", polling time: " << config.polling_time << "s. \n";

  auto task = [&]() {
    auto data = pihole.poll_data();
    influxdb.send_data(data);
  };

  asio::io_context context{};
  Timer timer{context, task, config.polling_time};
  context.run();
  return 0;
}

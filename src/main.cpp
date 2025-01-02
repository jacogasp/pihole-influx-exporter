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
  InfluxDB influxdb{config.influxdb_url, config.pihole_host};

  std::cout << "Pihole Influx Exporter started. Hostname: "
            << config.pihole_host << ", polling time: " << config.polling_time
            << "s. \n";

  auto task = [&]() {
    auto maybe_data = pihole.poll_data();
    if (maybe_data.has_value()) {
      influxdb.send_data(maybe_data.value());
    } else {
      std::cout << "Cannot poll data from pihole\n";
    }
  };

  asio::io_context context{};
  Timer timer{context, task, config.polling_time};
  context.run();
  return 0;
}

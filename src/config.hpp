#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>
#include <cstdlib>
#include <string>

struct Config
{
  std::string pihole_host;
  std::string influxdb_url;
  std::string pihole_endpoint;
  std::string pihole_token;
  uint64_t polling_time;

  static Config make_from_env()
  {
    Config config{};
    if (auto const host = std::getenv("PIHOLE_HOST")) {
      config.pihole_host = host;
    }
    if (auto const url = std::getenv("INFLUXDB_URL")) {
      config.influxdb_url = url;
    }
    if (auto const endpoint = std::getenv("PIHOLE_ENDPOINT")) {
      config.pihole_endpoint = endpoint;
    }
    if (auto const token = std::getenv("PIHOLE_TOKEN")) {
      config.pihole_token = token;
    }
    if (auto const polling_time = std::getenv("POLLING_TIME")) {
      config.polling_time = std::stoi(polling_time);
    }
    config.polling_time = config.polling_time > 0 ? config.polling_time : 1;
    return config;
  }
};

#endif

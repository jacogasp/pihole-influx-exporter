#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdlib>
#include <string>

struct Config
{
  std::string hostname;
  std::string influxdb_url;
  std::string pihole_endpoint;
  std::string pihole_token;

  static Config make_from_env()
  {
    Config config{};
    if (auto const hostname = std::getenv("HOSTNAME")) {
      config.hostname = hostname;
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
    return config;
  }
};

#endif

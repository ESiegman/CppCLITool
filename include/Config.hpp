// include/Config.hpp
#pragma once

#include "File.hpp"
#include <print>
#include <string>
#include <yaml-cpp/yaml.h>

class Yaml {
public:
  Yaml();
  void write();

  template <typename T> void setProperty(std::string key, T value) {
    out << YAML::Key << key;
    out << YAML::Value << value;
  }

  template <typename T> T readProperty(std::string key, T defaultValue) {
    try {
      YAML::Node config = YAML::LoadFile(fullPath.string());
      if (config[key]) {
        return config[key].as<T>();
      }
    } catch (const std::exception &e) {
      std::println(stderr, "Error reading config: {}", e.what());
    }
    return defaultValue;
  }

private:
  YAML::Emitter out;
};

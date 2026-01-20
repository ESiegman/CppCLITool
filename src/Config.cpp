// src/Config.cpp
#include "Config.hpp"
#include <fstream>

Yaml::Yaml() { out << YAML::BeginMap; }

void Yaml::write() {
  out << YAML::EndMap;

  std::ofstream fout(fullPath);
  if (fout.is_open()) {
    fout << out.c_str();
    std::println("Configuration saved to {}", fullPath.string());
  } else {
    std::println(stderr, "Failed to open {} for writing", fullPath.string());
  }
}

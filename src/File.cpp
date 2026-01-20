// src/File.cpp
#include "File.hpp"

bool checkConfigDir() { return std::filesystem::exists(folderDir); }

bool createConfigDir() {
  try {
    std::filesystem::create_directory(folderDir);
  } catch (const std::filesystem::filesystem_error &e) {
    return false;
  }
  return true;
}

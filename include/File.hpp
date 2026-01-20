// include/File.hpp
#pragma once

#include <filesystem>
#include <print>

inline const std::filesystem::path folderDir = ".cpp-tools";
inline const std::filesystem::path fileName = "config.yaml";
inline const std::filesystem::path fullPath = folderDir / fileName;

bool checkConfigDir();
bool createConfigDir();

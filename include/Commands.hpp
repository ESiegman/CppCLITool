// include/Commands.hpp
#pragma once

#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>

enum class Command {
  CMake_Config_Debug,
  CMake_Config_Release,
  CMake_Build,
  CMake_Run,
  Fetch_Program_Info,
  Set_Program_Info,
  Manual_Compile_Debug,
  Manual_Compile_Release,
  Manual_Compile_Run,

  CMake,
  Init,
  Help,
  Manual_Compile,

  Debug,
  Release,
  Build,
  Run
};

struct ProgramInfo {
  int cppVersion;
  std::string programVersion;
  bool debug;
  std::string executableName;
};

static ProgramInfo info;

static const std::unordered_map<Command,
                                std::function<std::string(std::string)>>
    commandList = {
        {Command::Manual_Compile_Debug,
         [](std::string) {
           return std::format(
               "g++ -std=c++{} -Og -g3 -Wall -Wextra -Wpedantic -Wshadow "
               "-Wconversion -fsanitize=address,undefined "
               "-fno-omit-frame-pointer -D_GLIBCXX_DEBUG main.cpp -o {}",
               info.cppVersion, info.executableName);
         }},
        {Command::Manual_Compile_Release,
         [](std::string) {
           return std::format(
               "g++ -std=c++{} -O3 -march=native -flto -DNDEBUG -Wall -Wextra "
               "main.cpp -o {}",
               info.cppVersion, info.executableName);
         }},
        {Command::Manual_Compile_Run,
         [](std::string args) {
           return "./" + info.executableName + " " + args;
         }},
        {Command::CMake_Config_Debug,
         [](std::string) {
           return "cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug";
         }},
        {Command::CMake_Config_Release,
         [](std::string) {
           return "cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release";
         }},
        {Command::CMake_Build,
         [](std::string) { return "cmake --build build -j"; }},
        {Command::CMake_Run,
         [](std::string args) {
           return "./build/" + info.executableName + " " + args;
         }},
};

static const std::unordered_map<std::string, Command> strToCommand = {
    {"debug", Command::Debug},
    {"release", Command::Release},
    {"run", Command::Run},
    {"fetch", Command::Fetch_Program_Info},
    {"set", Command::Set_Program_Info},
    {"build", Command::Build},

    {"-c", Command::CMake},
    {"-i", Command::Init},
    {"-h", Command::Help},
    {"-b", Command::Manual_Compile}};

void parseCommands(int argc, char *argv[]);

template <typename T>
T getValidatedInput(
    std::string_view prompt,
    std::string_view errorMessage = "Invalid input. Please try again.\n") {
  T value;
  while (true) {
    std::print("{}", prompt);
    if (std::cin >> value) {
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return value;
    } else {
      std::print("{}", errorMessage);
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

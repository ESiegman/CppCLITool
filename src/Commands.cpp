// src/Commands.cpp
#include "Commands.hpp"
#include "Config.hpp"
#include "File.hpp"

void parseCommands(int argc, char *argv[]) {
  if (argc == 1) {
    std::println("Project: {} v{}", info.executableName, info.programVersion);
    return;
  }

  {
    Yaml reader;
    info.executableName =
        reader.readProperty<std::string>("ExecutableName", "main");
    info.programVersion =
        reader.readProperty<std::string>("ProgramVersion", "1.0.0");
    info.cppVersion = reader.readProperty<int>("CppVersion", 23);
    info.debug = reader.readProperty<bool>("Debug", true);
  }

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (!strToCommand.contains(arg)) {
      std::println(stderr, "Unknown argument: {}", arg);
      continue;
    }

    Command mainCmd = strToCommand.at(arg);

    switch (mainCmd) {
    case Command::Init: {
      bool handled = false;
      if (i + 1 < argc) {
        std::string subArg = argv[i + 1];
        if (subArg == "fetch") {
          i++;
          std::println("\n--- Current Configuration ---");
          std::println("{:<18}: {}", "Executable Name", info.executableName);
          std::println("{:<18}: {}", "Program Version", info.programVersion);
          std::println("{:<18}: C++{}", "C++ Standard", info.cppVersion);
          std::println("{:<18}: {}", "Debug Mode", info.debug ? "ON" : "OFF");
          handled = true;
        } else if (subArg == "set") {
          i++;
          std::println("\n1. Executable Name ({})\n2. Version ({})\n3. C++ "
                       "({})\n4. Debug ({})",
                       info.executableName, info.programVersion,
                       info.cppVersion, info.debug);
          int choice = getValidatedInput<int>("Selection (1-4): ");
          switch (choice) {
          case 1:
            info.executableName = getValidatedInput<std::string>("New name: ");
            break;
          case 2:
            info.programVersion =
                getValidatedInput<std::string>("New version: ");
            break;
          case 3:
            info.cppVersion = getValidatedInput<int>("New C++ standard: ");
            break;
          case 4:
            info.debug = !info.debug;
            break;
          }
          Yaml writer;
          writer.setProperty("ExecutableName", info.executableName);
          writer.setProperty("ProgramVersion", info.programVersion);
          writer.setProperty("CppVersion", info.cppVersion);
          writer.setProperty("Debug", info.debug);
          writer.write();
          handled = true;
        }
      }
      if (!handled) {
        if (i + 1 < argc && (std::string(argv[i + 1]) == "init"))
          i++;
        if (!checkConfigDir())
          createConfigDir();
        info.executableName =
            getValidatedInput<std::string>("Executable name: ");
        info.programVersion = getValidatedInput<std::string>("Version: ");
        info.cppVersion = getValidatedInput<int>("C++ version: ");
        info.debug = (getValidatedInput<char>("Debug (y/n): ") == 'y');
        Yaml writer;
        writer.setProperty("ExecutableName", info.executableName);
        writer.setProperty("ProgramVersion", info.programVersion);
        writer.setProperty("CppVersion", info.cppVersion);
        writer.setProperty("Debug", info.debug);
        writer.write();
      }
      break;
    }

    case Command::Manual_Compile: {
      if (i + 1 < argc) {
        std::string subArg = argv[++i];
        if (subArg == "debug" || subArg == "release") {
          info.debug = (subArg == "debug");
          Yaml writer;
          writer.setProperty("ExecutableName", info.executableName);
          writer.setProperty("ProgramVersion", info.programVersion);
          writer.setProperty("CppVersion", info.cppVersion);
          writer.setProperty("Debug", info.debug);
          writer.write();
        } else if (subArg == "build") {
          auto key = info.debug ? Command::Manual_Compile_Debug
                                : Command::Manual_Compile_Release;
          int status = std::system(commandList.at(key)("").c_str());
          if (status != 0)
            std::println(stderr, "Build failed.");
        } else if (subArg == "run") {
          std::string extraArgs = "";
          while (i + 1 < argc)
            extraArgs += std::string(argv[++i]) + " ";
          int status = std::system(
              commandList.at(Command::Manual_Compile_Run)(extraArgs).c_str());
          if (status != 0)
            std::println(stderr, "Run failed.");
        }
      }
      break;
    }

    case Command::CMake: {
      if (i + 1 < argc) {
        std::string subArg = argv[++i];
        if (subArg == "debug" || subArg == "release") {
          info.debug = (subArg == "debug");
          Yaml writer;
          writer.setProperty("ExecutableName", info.executableName);
          writer.setProperty("ProgramVersion", info.programVersion);
          writer.setProperty("CppVersion", info.cppVersion);
          writer.setProperty("Debug", info.debug);
          writer.write();
        } else if (subArg == "config") {
          auto key = info.debug ? Command::CMake_Config_Debug
                                : Command::CMake_Config_Release;
          int status = std::system(commandList.at(key)("").c_str());
          if (status != 0)
            std::println(stderr, "Config failed.");
        } else if (subArg == "build") {
          int status =
              std::system(commandList.at(Command::CMake_Build)("").c_str());
          if (status != 0)
            std::println(stderr, "Build failed.");
        } else if (subArg == "run") {
          std::string extraArgs = "";
          while (i + 1 < argc)
            extraArgs += std::string(argv[++i]) + " ";
          int status = std::system(
              commandList.at(Command::CMake_Run)(extraArgs).c_str());
          if (status != 0)
            std::println(stderr, "Run failed.");
        }
      }
      break;
    }

    case Command::Help: {
      std::println("\nUsage: cpp-cli [option] [sub-command] [args...]");
      std::println("\nPROJECT MANAGEMENT: -i\n    init, set, fetch");
      std::println(
          "\nMANUAL BUILD: -b\n    debug, release, build, run [args...]");
      std::println("\nCMAKE BUILD: -c\n    debug, release, config, build, run "
                   "[args...]");
      break;
    }

    default:
      std::println("Invalid Command");
      break;
    }
  }
}

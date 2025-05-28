#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include "RUtils/ErrorOr.hpp"



namespace RUtils {
    std::filesystem::path find_executable(std::string command);
    ErrorOr<int> run_process(std::filesystem::path executable, const std::vector<std::string>& args, std::filesystem::path process_current_path = std::filesystem::current_path());
}
#pragma once

#include <cstdint>
#include <filesystem>
#include <future>
#include <string>
#include <vector>

#include "RUtils/ErrorOr.hpp"



namespace RUtils {
    namespace Request {
        struct Result {
            std::vector<uint8_t> buffer;

            int http_code;

            std::string content_type;
            std::string file_type;

            std::string_view as_str();
            ErrorOr<void> to_file(std::filesystem::path filepath);
        };

        ErrorOr<Result> get(const std::string& url);

        std::future<ErrorOr<Result>> get_async(const std::string& url);
    }
}
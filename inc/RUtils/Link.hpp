#pragma once

#include <string>

#include "RUtils/ErrorOr.hpp"



// Very simple parser to split urls into parts
struct Link {
    static RUtils::ErrorOr<Link> parse(const std::string& str);
    std::string to_string();

    std::string protocol, login, password, host, port, resource_path, fragment, query;
};

#include <cstdio>
#include <cstdlib>

#include "RUtils/Helpers.hpp"



void RUtils::unreachable(const std::source_location location) {
    std::fprintf(stderr, "%s:%d:%d: Unreachable!\n", location.file_name(), location.line(), location.column());
    std::fflush(stdout);
    std::fflush(stderr);
    std::abort();
}
#pragma once

#include <source_location>



namespace RUtils {
    void unreachable(const std::source_location location = std::source_location::current());

    // https://en.cppreference.com/w/cpp/utility/variant/visit
    template<class... Ts>
    struct visit_helper : Ts... { using Ts::operator()...; };
}
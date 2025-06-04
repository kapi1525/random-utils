#pragma once

#include <source_location>



namespace RUtils {
    // https://en.cppreference.com/w/cpp/utility/variant/visit
    template<class... Ts>
    struct visit_helper : Ts... { using Ts::operator()...; };
}
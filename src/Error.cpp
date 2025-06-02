#include <cstdio>
#include <format>

#include "RUtils/Error.hpp"



using namespace RUtils;

Error::Error(std::string error, ErrorType type, std::source_location where) {
    this->error = error;
    this->where = where;
    this->type = type;
}


void Error::print() {
    std::puts(std::format("{}:{}: In function \"{}\": {}: {}.", where.file_name(), where.line(), where.function_name(), get_type_str(type), error).c_str());
    std::fflush(stdout);
    std::fflush(stderr);
}


std::string_view Error::get_info() {
    return error;
}

ErrorType Error::get_type() {
    return type;
}


std::string Error::get_type_str(ErrorType type) {
    switch (type) {
    case ErrorType::unknown:
        return "Unknown error";
    case ErrorType::unsupported:
        return "Unsupported";
    case ErrorType::notimplemented:
        return "Not implemented";
    case ErrorType::library:
        return "External library error";
    case ErrorType::program:
        return "External program error";
    case ErrorType::external:
        return "External error";
    case ErrorType::invalid_arg:
        return "Invalid argument";
    case ErrorType::timed_out:
        return "Timed out";
    case ErrorType::io:
        return "IO error";
    case ErrorType::out_of_range:
        return "Out of range";
    default:
        Error("Could'nt find a string for this error type", ErrorType::notimplemented).print();
        return "???";
    }
}
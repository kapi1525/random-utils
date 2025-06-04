#include <cstdio>
#include <cstring>
#include <format>

#include "RUtils/Error.hpp"



using namespace RUtils;

Error::Error(std::string error, ErrorType type, std::source_location where) {
    this->error = error;
    this->where = where;
    this->type = type;

    if(type == ErrorType::external || type == ErrorType::system || type == ErrorType::library || type == ErrorType::program) {
        unreachable("This error type requires an error code", where);
    }
}

Error::Error(std::string error, ErrorType type, int code, std::source_location where) {
    this->error = error;
    this->where = where;
    this->type = type;
    this->code = code;
}


Error Error::from_errno(std::string error, int err, std::source_location where) {
    if(error.empty()) {
        return Error(std::format("{}", error, std::strerror(err)), ErrorType::system, err, where);
    } else {
        return Error(std::format("{}: {}", error, std::strerror(err)), ErrorType::system, err, where);
    }
}


void Error::unreachable(std::string error, std::source_location where) {
    Error(error, ErrorType::unreachable, where).print();
    std::abort();
}

void Error::todo_abort(std::string error, std::source_location where) {
    todo(error, where);
    std::abort();
}

void Error::todo(std::string error, std::source_location where) {
    Error(error, ErrorType::not_implemented, where).print();
}



const char* Error::get_type_str(ErrorType type) {
    switch (type) {
    case ErrorType::unknown:
        return "Unknown error";
    case ErrorType::unsupported:
        return "Unsupported";
    case ErrorType::not_implemented:
        return "Not implemented";
    case ErrorType::unreachable:
        return "Unrechable";
    case ErrorType::logic:
        return "Logic";
    case ErrorType::out_of_range:
        return "Out of range";
    case ErrorType::invalid_argument:
        return "Invalid argument";
    case ErrorType::timed_out:
        return "Timed out";
    case ErrorType::io:
        return "IO error";
    case ErrorType::external:
        return "External";
    case ErrorType::system:
        return "System";
    case ErrorType::library:
        return "Library";
    case ErrorType::program:
        return "Program";
    default:
        todo("Couldn't find a string for this error type");
        return "???";
    }
}

bool Error::type_stores_error_code(ErrorType type) {
    return type == ErrorType::external || type == ErrorType::system || type == ErrorType::library || type == ErrorType::program;
}



void Error::print() {
    std::string msg;
    if(stores_error_code()) {
        msg = std::format("{}:{}: In function \"{}\": {} ({}): {}.", where.file_name(), where.line(), where.function_name(), get_type_str(type), code, error);
    } else {
        msg = std::format("{}:{}: In function \"{}\": {}: {}.", where.file_name(), where.line(), where.function_name(), get_type_str(type), error);
    }
    std::puts(msg.c_str());
    std::fflush(stdout);
    std::fflush(stderr);
}


bool Error::stores_error_code() {
    return type_stores_error_code(type);
}


std::string_view Error::get_info() {
    return error;
}

ErrorType Error::get_type() {
    return type;
}

int Error::get_code(std::source_location where) {
    if(!type_stores_error_code(type)) {
        unreachable("This error type doesn't store an error code", where);
    }
    return code;
}

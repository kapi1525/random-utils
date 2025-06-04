#pragma once

#include <source_location>
#include <string>
#include <string_view>
#include <errno.h>



namespace RUtils {
    enum class ErrorType {
        unknown,
        unsupported,
        not_implemented,
        unreachable,
        logic,              // Programmer failed to write the correct code
        out_of_range,
        invalid_argument,
        timed_out,
        io,

        // those store additional system, library, etc specific error code
        external,
        system,
        library,
        program,
    };

    // Stores an error type and optionaly a system specific error code.
    class Error {
    public:
        Error(std::string error, ErrorType type = ErrorType::unknown, std::source_location where = std::source_location::current());
        Error(std::string error, ErrorType type, int code, std::source_location where = std::source_location::current());

        static Error from_errno(std::string error = "", int err = errno, std::source_location where = std::source_location::current());

        // creates instant crash
        [[noreturn]] static void unreachable(std::string error = "", std::source_location where = std::source_location::current());
        [[noreturn]] static void todo_abort(std::string error = "", std::source_location where = std::source_location::current());

        static void todo(std::string error = "", std::source_location where = std::source_location::current());

        static const char* get_type_str(ErrorType type);
        static bool type_stores_error_code(ErrorType type);

        ~Error() = default;

        void print();

        bool stores_error_code();

        std::string_view get_info();
        ErrorType get_type();
        int get_code(std::source_location where = std::source_location::current());

    private:
        std::string error;
        std::source_location where;
        ErrorType type;
        int code;
    };
}
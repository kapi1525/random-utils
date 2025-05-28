#pragma once

#include <source_location>
#include <string>
#include <string_view>



namespace RUtils {
    enum class ErrorType {
        unknown,
        unsupported,
        notimplemented,
        library,
        program,
        system,
        external,
        invalid_arg,
        logic,          // Programmer failed to write the correct code
        timed_out,
        io,
        out_of_range,
    };

    // Helper class for returning errors.
    class Error {
    public:
        Error(std::string error, ErrorType type = ErrorType::unknown, std::source_location where = std::source_location::current());
        ~Error() = default;

        void print();

        std::string_view get_info();
        ErrorType get_type();

        static std::string get_type_str(ErrorType type);

    private:
        std::string error;
        std::source_location where;
        ErrorType type;
    };
}
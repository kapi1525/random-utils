#pragma once

#include <vector>
#include <variant>
#include <functional>
#include <string>



namespace RUtils {
    // Command line parsing
    struct CommandLineArgDef {
        char short_flag;
        const char* long_flag;
        std::variant<std::function<void()>, std::function<void(std::string)>> callback;  // called when arg is present argument value is passed
        const char* argument_name;
        const char* help_string;
    };

    class CommandLine {
    public:
        const char* program_name;
        std::vector<CommandLineArgDef> arg_definitions;

        // Returns false on invalid command line input, can be used to print help message.
        bool parse(int argc, const char* argv[]);

        void display_help_string();

    private:
        bool is_short_flag(const char* arg);
        bool is_long_flag(const char* arg);
        bool is_a_flag(const char* arg);
    };
}
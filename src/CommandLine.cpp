#include <cstring>

#include "RUtils/CommandLine.hpp"
#include "RUtils/Helpers.hpp"
#include "RUtils/Error.hpp"



#if 0
    #define debug_print(...) std::printf(__VA_ARGS__)
#else
    #define debug_print(...)
#endif

bool RUtils::CommandLine::parse(int argc, const char* argv[]) {
    debug_print("Parsing command line...\n");

    for (int i = 1; i < argc; i++) {
        auto arg = argv[i];
        debug_print("%s\n", arg);

        if(!is_a_flag(arg)) {
            debug_print("Not a flag.\n");
            return false;
        }

        bool handled = false;
        for (auto &&arg_def : arg_definitions) {
            if((is_short_flag(arg) && arg[1] == arg_def.short_flag) || (is_long_flag(arg) && std::strcmp(arg_def.long_flag, arg+2) == 0)) {
                std::visit(RUtils::visit_helper{
                    [](std::monostate arg) {
                        Error::unreachable();
                    },
                    [&](std::function<void()> callback) {
                        debug_print("Calling callback...\n");
                        callback();
                    },
                    [&](std::function<void(std::string)> callback) {
                        if(i + 1 < argc && !is_a_flag(argv[i + 1])) {
                            i++;
                            debug_print("Calling callback with \"%s\" argument...\n", argv[i]);
                            callback(argv[i]);
                        }
                    },
                }, arg_def.callback);
                handled = true;
                break;
            }
        }

        if(!handled) {
            debug_print("Unknown flag \"%s\".\n", arg);
            return false;
        }
    }

    return true;
}

void RUtils::CommandLine::display_help_string() {
    std::printf("Usage: %s [options]\n\n", program_name);
    std::printf("Options:\n");

    int pad = 0;
    for (auto &&arg : arg_definitions) {
        int new_pad = 2;
        if(arg.short_flag) {
            new_pad += 3;    // "-%c "
        }
        if(arg.long_flag) {
            new_pad += 3 + std::strlen(arg.long_flag);      // "--%s "
        }
        if(arg.argument_name) {
            new_pad += 3 + std::strlen(arg.argument_name);  // "<%s> "
        }
        if(new_pad > pad) {
            pad = new_pad;
        }
    }

    for (auto &&arg : arg_definitions) {
        std::printf("  ");
        int to_pad = pad - 2;

        if(arg.short_flag) {
            std::printf("-%c ", arg.short_flag);
            to_pad -= 3;
        }
        if(arg.long_flag) {
            std::printf("--%s ", arg.long_flag);
            to_pad -= 3 + std::strlen(arg.long_flag);
        }
        if(arg.argument_name) {
            std::printf("<%s> ", arg.argument_name);
            to_pad -= 3 + std::strlen(arg.argument_name);
        }

        for (int i = 0; i < to_pad; i++) {
            std::putc(' ', stdout);
        }

        if(arg.help_string) {
            std::printf("%s", arg.help_string);
        }
        std::printf("\n");
    }

    std::printf("\n");
}


bool RUtils::CommandLine::is_short_flag(const char* arg) {
    return std::strlen(arg) == 2 && arg[0] == '-' && arg[1] != '-';
}

bool RUtils::CommandLine::is_long_flag(const char* arg) {
    return std::strlen(arg) > 2 && arg[0] == '-' && arg[1] == '-';
}

bool RUtils::CommandLine::is_a_flag(const char* arg) {
    return is_short_flag(arg) || is_long_flag(arg);
}
#include <cstring>
#include <format>

#include "RUtils/Process.hpp"
#include "RUtils/Platform.hpp"
#include "RUtils/Helpers.hpp"

#ifdef PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define STRICT
    #define NOMINMAX
    #include <windows.h>
#endif

#ifdef PLATFORM_POSIX
    #include <spawn.h>      // posic_spawn
    #include <unistd.h>     // environ
    #include <sys/wait.h>   // waitpid
#endif



// Look for executable in PATH
std::filesystem::path RUtils::find_executable(std::string command) {
    if(std::filesystem::exists(command)) {
        return std::filesystem::absolute(command);
    }

    std::string_view env_path(std::getenv("PATH"));

    if(env_path.empty()) {
        return {};
    }

#ifdef PLATFORM_POSIX
    char env_path_separator = ':';
#elif defined(PLATFORM_WINDOWS)
    char env_path_separator = ';';
#endif

    for (size_t i = 0; i < env_path.size(); i++) {

        std::size_t end = env_path.find(env_path_separator, i);

        if(end == std::string_view::npos) {
            end = env_path.size();
        }

        std::filesystem::path path = env_path.substr(i, end - i);
        i = end;

        if(!std::filesystem::exists(path)) {
            continue;
        }

        for (auto &&i : std::filesystem::directory_iterator(path)) {
            std::error_code err;
            if(!i.is_regular_file(err) || err) {
                continue;
            }

            // FIXME: On windows look for exe files.
            if(i.path().filename().replace_extension("") == command) {
                return i.path();
            }
        }
    }

    return {};
}



RUtils::ErrorOr<int> RUtils::run_process(std::filesystem::path executable, const std::vector<std::string>& args, std::filesystem::path process_current_path) {
#ifdef PLATFORM_WINDOWS
    // TODO: Make this nicer
    std::string temp_cmd;
    temp_cmd += "\"";
    temp_cmd += executable.string();
    temp_cmd += "\"";
    for (auto &&arg : args) {
        auto escape_quotes = [](std::string str) -> std::string {
            std::string temp;
            for (auto& c : str) {
                if(c == '"') {
                    temp += '\\';
                }
                temp += c;
            }
            return temp;
        };

        temp_cmd += "\"";
        temp_cmd += escape_quotes(arg);
        temp_cmd += "\"";
    }

    std::unique_ptr<char[]> command_line = std::make_unique<char[]>(temp_cmd.length() + 1); // +1 for null terminator
    std::memcpy(command_line.get(), temp_cmd.c_str(), temp_cmd.length());

    STARTUPINFO si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);

    if(!CreateProcessA(nullptr, command_line.get(), nullptr, nullptr, false, 0, nullptr, process_current_path.string().c_str(), &si, &pi)) {
        return Error(std::format("CreateProcess failed: {}.", GetLastError()));
    }

    // Wait until child process to exit
    if(WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED) {
        return Error(std::format("WaitForSingleObject failed: {}.", GetLastError()));
    }

    DWORD exit_code = 0;
    if(!GetExitCodeProcess(pi.hProcess, &exit_code)) {
        return Error(std::format("GetExitCodeProcess failed: {}.", GetLastError()));
    }

    if(!CloseHandle(pi.hProcess)) {
        return Error(std::format("CloseHandle failed: {}.", GetLastError()));
    }
    if(!CloseHandle(pi.hThread)) {
        return Error(std::format("CloseHandle failed: {}.", GetLastError()));
    }

    return exit_code;

#elif defined(PLATFORM_POSIX)
    std::size_t argc = args.size() + 1;                                                 // +1 for filename
    std::unique_ptr<const char*[]> argv = std::make_unique<const char*[]>(argc + 1);    // +1 for null terminator

    argv[0] = executable.filename().c_str();

    for (std::size_t i = 1; i < argc; i++) {
        argv[i] = args[i - 1].c_str();
    }

    pid_t pid = fork();
    if(pid == 0) {
        // child process
        std::filesystem::current_path(process_current_path);
        execv(executable.c_str(), (char**)argv.get());
        perror("execv() failed");
        exit(errno);
    } else if(pid < 0) {
        perror("fork() failed");
        return Error(std::format("fork() failed: {}", std::strerror(errno)));
    }

    int status = -1;
    if(waitpid(pid, &status, 0) != pid) {
        perror("waitpid() failed");
    }

    return status;

#endif

    RUtils::unreachable();
    return -1;
}
#pragma once



#ifdef _WIN32
    #define PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    #define PLATFORM_POSIX
#endif
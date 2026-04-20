#include "RUtils/Unicode.hpp"
#include "RUtils/Platform.hpp"

#include <cassert>

#ifdef PLATFORM_WINDOWS
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif



#ifdef PLATFORM_WINDOWS
std::string to_utf8(std::wstring_view utf16_str) {
    int buffer_size = WideCharToMultiByte(
        CP_UTF8,
        0,
        utf16_str.data(),
        utf16_str.size(),
        nullptr,
        0,
        nullptr,
        nullptr
    );

    if(buffer_size <= 0) {
        return "";
    }

    char* buffer = new char[buffer_size];

    int status = WideCharToMultiByte(
        CP_UTF8,
        0,
        utf16_str.data(),
        utf16_str.size(),
        buffer,
        buffer_size,
        nullptr,
        nullptr
    );

    if(status == 0) {
        std::uint32_t err = GetLastError();
        const size_t error_buf_size = 128;
        char error_buf[error_buf_size];

        std::snprintf(error_buf, error_buf_size, "UTF16 to UTF8 text conversion failed: %i.", err);
        throw std::runtime_error(error_buf);
    }

    return std::string(buffer, buffer_size);
}

std::wstring to_utf16(std::string_view utf8_str) {
    int buffer_size = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8_str.data(),
        utf8_str.size(),
        nullptr,
        0
    );

    if(buffer_size <= 0) {
        return L"";
    }

    wchar_t* buffer = new wchar_t[buffer_size];

    int status = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8_str.data(),
        utf8_str.size(),
        buffer,
        buffer_size
    );

    if(status == 0) {
        std::uint32_t err = GetLastError();
        const size_t error_buf_size = 128;
        char error_buf[error_buf_size];

        std::snprintf(error_buf, error_buf_size, "UTF8 to UTF16 text conversion failed: %i.", err);
        throw std::runtime_error(error_buf);
    }

    return std::wstring(buffer, buffer_size);
}
#endif



windows_utf8_console::windows_utf8_console() {
    #ifdef PLATFORM_WINDOWS
    before_codepage = GetConsoleCP();
    before_out_codepage = GetConsoleOutputCP();
    if(before_codepage && before_out_codepage) {
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    }
    #endif
}

windows_utf8_console::~windows_utf8_console() {
    #ifdef PLATFORM_WINDOWS
    if(before_codepage && before_out_codepage) {
        SetConsoleCP(before_codepage);
        SetConsoleOutputCP(before_out_codepage);
    }
    #endif
}

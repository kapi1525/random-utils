#pragma once

#include <string>
#include <string_view>
#include <cstdint>



// Unicode conversion, windows only
std::string to_utf8(std::wstring_view utf16_str);
std::wstring to_utf16(std::string_view utf8_str);


// Windows only, NOOP elsewhere
class windows_utf8_console {
public:
    windows_utf8_console();
    ~windows_utf8_console();

private:
    std::uint32_t before_codepage, before_out_codepage;
};
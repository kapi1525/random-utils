#include "RUtils/Request.hpp"



int main() {
    // this is dumb and doesnt even work lol
    auto a = RUtils::Request::get("https://example.com/");
    return a.http_code == 200;
}
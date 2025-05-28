#include <string>

#include "RUtils/ErrorOr.hpp"



class Foo {
public:
    Foo() = default;
    ~Foo() = default;
    std::string a;
    std::string b;
};

RUtils::ErrorOr<Foo> Get() {
    Foo f;
    f.a = "a";
    f.b = "b";
    return f;
}

int main() {
    Foo f = Get();
    if(f.a != "a") {
        return 1;
    }
    if(f.b != "b") {
        return 2;
    }
    return 0;
}
#include "RUtils/ErrorOr.hpp"



class Foo {
public:
    Foo() = default;
    ~Foo() = default;
    int a = 0;
    int b = 15;
};

RUtils::ErrorOr<Foo> Get() {
    return Foo();
}

int main() {
    return Get().value().a;
}
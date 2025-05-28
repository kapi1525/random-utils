#include <string>

#include "RUtils/ErrorOr.hpp"



std::string a = "a";

int moves = 0;
int copies = 0;

class Foo {
public:
    Foo() = default;
    ~Foo() = default;

    // move
    Foo(const Foo&& other) {
        a = std::move(other.a);
        moves++;
    }

    // copy
    Foo(const Foo& other) {
        a = other.a;
        copies++;
    }

    std::string a;
};

RUtils::ErrorOr<Foo> Get() {
    Foo f;
    f.a = a;
    return f;
}

int main() {
    Foo f = Get();

    std::printf("Moved:  %d times", moves);
    std::printf("Copied: %d times", copies);

    if(moves > 2) {
        return moves;
    }
    return copies;
}
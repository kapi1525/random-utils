#include "RUtils/ErrorOr.hpp"



RUtils::ErrorOr<void> Get() {
    return 2;
}

int main() {
    auto a = Get();
    if(a) {
        return 0;
    }
    return 1;
}
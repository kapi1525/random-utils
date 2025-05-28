#include "RUtils/ErrorOr.hpp"



RUtils::ErrorOr<void> Get() {
    return {};
}

int main() {
    auto a = Get();
    if(a) {
        return 0;
    }
    return 1;
}
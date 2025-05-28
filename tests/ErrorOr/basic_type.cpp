#include "RUtils/ErrorOr.hpp"



RUtils::ErrorOr<int> Get() {
    return 0;
}

int main() {
    return Get().value();
}
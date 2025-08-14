#include "RUtils/Defer.hpp"

#include <cstdlib>
#include <cstdio>



int main() {
    void* a = malloc(10);
    bool freed = false;

    {
        RUtils::Defer( free(a); freed = true; );

        ((int*)a)[0] = 10;
    }

    return !freed;
}
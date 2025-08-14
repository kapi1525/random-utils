#pragma once



// Example:
// RUtils::Defer( free(something); );

#define DEFER4(x, y) x##y
#define DEFER3(x, y) DeferImpl x([&]{y})
#define DEFER2(x, y, z) DEFER3(DEFER4(x, y), z)
#define Defer(x) DEFER2(_defer, __COUNTER__, x)


namespace RUtils {
    template<typename F>
    class DeferImpl {
        public:
        DeferImpl(F f) : f(f) {};
        ~DeferImpl() { f(); };

        protected:
        F f;
    };
}

// #define Defer(x) DeferImpl([&](x))


// RUtils::Defer( free(0); );

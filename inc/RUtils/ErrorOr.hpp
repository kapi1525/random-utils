#pragma once

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string_view>
#include <type_traits>

#include "RUtils/Error.hpp"

#ifdef max
    #undef max
    #warning MICROSOFT WHY
#endif



namespace RUtils {
    // holds either a return value or an error.
    // value type can be void
    template<typename T, typename E = Error>
    class ErrorOr {
        using value_type = T;
        using value_type_not_void = std::conditional<std::is_void_v<value_type>, int, value_type>::type;
        using error_type = E;
        using union_type = uint8_t[std::max(sizeof(value_type_not_void), sizeof(error_type))];

    public:
        ErrorOr() {
            new(&data) value_type_not_void();
            stores_value = true;
        };

        ErrorOr(value_type_not_void&& value) {
            new(&data) value_type_not_void(std::forward<value_type_not_void>(value));
            stores_value = true;
        }

        ErrorOr(error_type&& error) {
            new(&data) error_type(std::forward<error_type>(error));
            stores_value = false;
        }


        // ErrorOr(const ErrorOr& other) {
        //     if(other.stores_value) {
        //         as_value() = other.as_value();
        //     }
        //     stores_value = other.stores_value;
        // }

        // ErrorOr(const ErrorOr&& other) {
        //     if(other.stores_value) {
        //         as_value() = std::move(other.as_value());
        //     }
        //     stores_value = other.stores_value;
        // }


        ~ErrorOr() {
            if(stores_value) {
                as_value().~value_type_not_void();
            } else {
                as_error().~error_type();
            }
        };


        // Get the object, if not present print error and abort.
        template<typename U = value_type>
        operator typename std::enable_if<!std::is_void_v<U>, U>::type&&() {
            return std::forward<U>(value());
        }

        // false = error
        operator bool() {
            return has_value();
        }


        template<typename U = value_type>
        typename std::enable_if<!std::is_void_v<U>, U>::type&& value() {
            if(stores_value) {
                return std::forward<U>(as_value());
            }
            print();
            std::puts("Attempted to get value without handling an error. Aborting...");
            std::fflush(stdout);
            std::fflush(stderr);
            std::abort();
        }

        error_type& error() {
            if(!stores_value) {
                return as_error();
            }
            std::puts("Attempted to get error, but there was no error. Aborting...");
            std::fflush(stdout);
            std::fflush(stderr);
            std::abort();
        }


        bool has_value() {
            return stores_value;
        }

        bool has_error() {
            return !stores_value;
        }


        void print() {
            error().print();
        }

        std::string_view get_info() {
            return error().get_info();
        }

        ErrorType get_type() {
            return error().get_type();
        }

    protected:
        union_type data;

        value_type_not_void& as_value() {
            return *reinterpret_cast<value_type_not_void*>(data);
        }

        error_type& as_error() {
            return *reinterpret_cast<error_type*>(data);
        }

        bool stores_value;
    };

    // Commonly used, template instance gets created once elsewhere.
    extern template class ErrorOr<void>;
}
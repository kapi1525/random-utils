#pragma once
#include <thread>
#include <algorithm>
#include <vector>
#include <mutex>

#include "RUtils/ErrorOr.hpp"



namespace RUtils {
    template<class IteratorType, class FunctionType>
    ErrorOr<void> for_each(IteratorType begin, IteratorType end, FunctionType function) {
        while(begin != end) {
            auto& val = *begin;
            begin++;

            function(val);
        }
        return {};
    }

    // Simmilar to std::for_each, but guaranted to be multithreaded.
    template<class IteratorType, class FunctionType>
    ErrorOr<void> for_each_threaded(IteratorType begin, IteratorType end, FunctionType function, uint32_t max_threads = UINT32_MAX) {
        if(max_threads == 1) {
            return RUtils::for_each(begin, end, function);
        }


        std::mutex iterator_mutex;

        auto worker_func = [&]() {
            for(;;) {
                std::lock_guard<std::mutex> lock(iterator_mutex);
                if(begin == end) {
                    return;
                }
                auto& val = *begin;
                begin++;
                lock.~lock_guard();

                function(val);
            }
        };

        std::vector<std::thread> threads;
        for (uint32_t i = 0; i < std::min(max_threads, std::thread::hardware_concurrency()); i++) {
            threads.emplace_back(worker_func);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        return {};
    }
}
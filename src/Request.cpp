#include <cmath>
#include <fstream>
#include <filesystem>
#include <format>

#include "RUtils/Error.hpp"
#include "curl/curl.h"

#include "RUtils/Request.hpp"



using namespace RUtils;

// TODO:

std::string_view Request::Result::as_str() {
    return {reinterpret_cast<char*>(buffer.data()), buffer.size()};
}

ErrorOr<void> Request::Result::to_file(std::filesystem::path filepath) {
    std::ofstream file(filepath);
    if(file.fail()) {
        return Error("Failed to open a file", ErrorType::io);
    }

    file.write((const char*)buffer.data(), buffer.size());
    if(file.fail()) {
        return Error("File write failed", ErrorType::io);
    }

    file.close();
    if(file.fail()) {
        return Error("File close failed", ErrorType::io);
    }

    return {};
}



// Write function for curl
std::size_t write_callback(char* ptr, std::size_t size, std::size_t nmemb, Request::Result* result) {
    std::size_t bytes_to_write = size * nmemb;
    std::size_t start_index = result->buffer.size();

    result->buffer.reserve(start_index + bytes_to_write);

    for (size_t i = 0; i < bytes_to_write; i++) {
        result->buffer.push_back(ptr[i]);
    }

    return bytes_to_write;
}

// FIXME: rethink
ErrorOr<Request::Result> Request::get(const std::string& url) {
    CURLcode res;   // reused later
    res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if(res != CURLE_OK) {
        return Error(std::format("curl_global_init failed: {}", curl_easy_strerror(res)), ErrorType::library);
    }

    Request::Result result;

    CURL* curl_handle = curl_easy_init();

    if(!curl_handle) {
        return Error("curl_easy_init failed", ErrorType::library);
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);       // Required for thread safety
    // curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, true);

    res = curl_easy_perform(curl_handle);
    if(res != CURLE_OK) {
        return Error(std::format("curl_easy_perform failed: {}", curl_easy_strerror(res)), ErrorType::library);
    }


    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &result.http_code);

    curl_header* header;
    CURLHcode err = curl_easy_header(curl_handle, "Content-Type", 0, CURLH_HEADER, -1, &header);
    if(err != CURLHE_OK) {

    }
    result.content_type = header->value;

    // curl_header* header;
    // CURLHcode err = curl_easy_header(curl_handle, "Content-Type", 0, CURLH_HEADER, -1, &header);

    // std::printf("%s\n", header->value);
    // std::printf("%zu\n", header->amount);
    // err = curl_easy_header(curl_handle, "Content-Disposition", 0, CURLH_HEADER, -1, &header);
    // std::printf("%s\n", header->value);
    // std::printf("%zu\n", header->amount);

    // Error("", ErrorType::notimplemented).print();
    // return "";

    curl_easy_cleanup(curl_handle);

    return result;
}

std::future<ErrorOr<Request::Result>> Request::get_async(const std::string& url) {
    return std::async(std::launch::async, get, url);
}







// std::unique_ptr<RUtils::Request> Request::get(std::string url) {
//     Request* request = new Request;
//     curl_global_init(CURL_GLOBAL_DEFAULT);

//     CURL* curl_handle = curl_easy_init();
//     curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
//     curl_easy_setopt(curl_handle, CURLOPT_HTTPGET, 1L);
//     curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
//     curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
//     curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
//     curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, request);
//     curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
//     curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);       // Required for thread safety
//     // curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, true);

//     request->handle = curl_handle;
//     request->worker_thread = std::thread(&Request::worker, request);

//     return { std::unique_ptr<RUtils::Request>(request) };
// }



// Request::~Request() {
//     CURL* curl_handle = handle;
//     curl_easy_cleanup(curl_handle);
// }



// void Request::wait() {
//     if(worker_thread.joinable()) {
//         worker_thread.join();
//     }
// }



// bool Request::completed() {
//     return result.completed;
// }

// std::shared_ptr<uint8_t[]> Request::result_raw() {
//     wait();

//     if(!result.completed) {
//         abort();
//     }

//     return result.buffer;
// }

// size_t Request::result_raw_size() {
//     wait();

//     return result.count;
// }


// std::string Request::result_str() {
//     return std::string((char*)result_raw().get(), result_raw_size());
// }


// std::string Request::result_content_type() {
//     wait();

//     CURL* curl_handle = handle;
//     curl_header* header;
//     CURLHcode err = curl_easy_header(curl_handle, "Content-Type", 0, CURLH_HEADER, -1, &header);

//     return header->value;
// }

// std::string Request::result_file_type() {
//     wait();

//     CURL* curl_handle = handle;
//     curl_header* header;
//     CURLHcode err = curl_easy_header(curl_handle, "Content-Type", 0, CURLH_HEADER, -1, &header);

//     std::printf("%s\n", header->value);
//     std::printf("%zu\n", header->amount);
//     err = curl_easy_header(curl_handle, "Content-Disposition", 0, CURLH_HEADER, -1, &header);
//     std::printf("%s\n", header->value);
//     std::printf("%zu\n", header->amount);

//     Error("", ErrorType::notimplemented).print();
//     return "";
// }

// ErrorOr<void> Request::result_to_file(std::filesystem::path filepath) {
//     wait();

//     std::ofstream file(filepath);
//     if(file.fail()) {
//         return Error("Failed to open a file", ErrorType::io);
//     }

//     file.write((char*)result.buffer.get(), result.count);
//     if(file.fail()) {
//         return Error("File write failed", ErrorType::io);
//     }

//     file.close();
//     if(file.fail()) {
//         return Error("File close failed", ErrorType::io);
//     }

//     return {};
// }



// int Request::result_code() {
//     wait();

//     CURL* curl_handle = handle;

//     int ret;
//     curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &ret);

//     return ret;
// }



// // Write function for curl
// std::size_t Request::write_callback(char* ptr, std::size_t size, std::size_t nmemb, Request* request) {
//     std::size_t realsize = size * nmemb;

//     // not enough space, reallocate the buffer
//     if(request->result.count + realsize > request->result.size) {
//         auto new_size = request->result.count + realsize + (1024 * 16);
//         auto new_buffer = std::make_shared<std::uint8_t[]>(new_size);

//         // copy old buffer
//         for (size_t i = 0; i < request->result.count; i++) {
//             new_buffer[i] = request->result.buffer[i];
//         }

//         request->result.size = new_size;

//         request->result.buffer.swap(new_buffer);
//     }

//     for (size_t i = 0; i < realsize; i++) {
//         request->result.buffer[i + request->result.count] = ptr[i];
//     }

//     request->result.count += realsize;

//     return realsize;
// }


// void Request::worker() {
//     CURL* curl_handle = handle;
//     CURLcode res = curl_easy_perform(curl_handle);
//     if(res != CURLE_OK) {

//     }
//     result.completed = true;
// }


#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace client {
namespace http {

struct Response {
    int status_code;
    std::vector<std::pair<std::string, std::string>> headers;
    std::vector<uint8_t> content;
};

}  // namespace http
}  // namespace client

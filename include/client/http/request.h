#pragma once

#include <llhttp.h>
#include <string>
#include <utility>
#include <vector>

namespace client {
namespace http {

struct Request {
    ::llhttp_method method;
    std::string path;
    std::vector<std::pair<std::string, std::string>> headers;
    std::vector<uint8_t> content;
};

}  // namespace http
}  // namespace client

#pragma once
#include <conditional_vaiable>
#include <mutex>
#include <string>
#include <utility>
#include <vector>
#include "client/http/connection.h"

namespace client {
namespace http {

class ConnectionPool;

class ConnectionGuard {
   public:
    using OnResponse = Connection::OnResponse;
    using OnError = Connection::OnError;
    void Get(const std::string&, OnResponse, OnError);
    void Get(const std::string&,
             std::vector<std::pair<std::string, std::string>>&&,
             OnResponse,
             OnError);
    void Post(const std::string&, OnResponse, OnError);
    void Post(const std::string&, std::vector<uint8_t>&&, OnResponse, OnError);
    void Post(const std::string&,
              std::vector<std::pair<std::string, std::string>>&&,
              std::vector<uint8_t>&&,
              OnResponse,
              OnError);

   private:
    friend class ConnectionPool;
    ConnectionGuard(Connection* connection, ConnectionPool* pool)
        : connection_(connection), connection_pool_(pool) {}
    // TODO: delete copy and move constructor
    Connection* connection_;
    ConnectionPool* connection_pool_;
};

class ConnectionPool {
   public:
    ConnectionPool(std::string host, std::string port = "443");
    ConnectionGuard Acquire();
    void Release(Connection*);

   public:
    void Shutdown();

   private:
};

}  // namespace http
}  // namespace client

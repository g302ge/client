#pragma once

#include <llhttp.h>
#include <asio.hpp>
#include <asio/ssl.hpp>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include "client/http/request.h"
#include "client/http/response.h"

namespace client {
namespace http {

class Connection : public std::enable_shared_from_this<Connection> {
   public:
    using OnResponse = std::function<void(std::unique_ptr<Response>)>;
    using OnError = std::function<void(const asio::error_code&)>;
    enum State {
        Initialized,
        Idle,
        Active,
        Invalid,
    };

    // connection pool called
   private:
    using OnConnectSuccess = std::function<void(Connection*)>;
    using OnConnectError = std::function<void(Connection*, const asio::error_code&)>;
    using OnConnectionIdleTimeout = std::function<void(Connection*)>;
    Connection(asio::io_context&);
    void SetIdleTimeout(std::chrono::milliseconds, OnConnectionIdleTimeout);
    void DoConnect(const asio::ip::tcp::endpoint&, OnConnectSuccess, OnConnectError);

    // connection inner call
   private:
    void DoWrite();
    void DoRead();
    void EnableIdle();
    void DisableIdle();
    void EnableReadTimeout();
    void DisableReadTimeout();

    // connection guard called
   private:
    void SetOnResponse(OnResponse&&);
    void SetOnError(OnError&&);
    void Execute(Request&&, std::chrono::milliseconds timeout = std::chrono::milliseconds(10000));

   private:
    friend int OnHttpHeaderField(::llhttp_t*, const char*, size_t);
    friend int OnHttpHeaderFieldComplete(::llhttp_t*);
    friend int OnHttpHeaderValue(::llhttp_t*, const char*, size_t);
    friend int OnHttpHeaderValueComplete(::llhttp_t*);
    friend int OnHttpHeadersComplete(::llhttp_t*);
    friend int OnHttpMessageComplete(::llhttp_t*);
    friend int OnHttpBody(::llhttp_t*, const char*, size_t);

   private:
    friend class ConnectionPool;
    friend class ConnectionGuard;
    asio::io_context& io_context_;
    std::unique_ptr<asio::ssl::context> ssl_context_;
    std::unique_ptr<asio::ssl::stream<asio::ip::tcp::socket>> ssl_socket_;
    asio::streambuf read_buffer_;
    asio::streambuf write_buffer_;
    std::pair<std::string, std::string> current_header_;
    std::unique_ptr<Response> response_;
    ::llhttp_t parser_;
    ::llhttp_settings_t parser_settings_;
    asio::steady_timer idle_timer_;
    std::chrono::milliseconds idle_timeout_;
    OnResponse on_response_;
    OnError on_error_;
    OnConnectionIdleTimeout on_idle_timeout_;
};

}  // namespace http
}  // namespace client

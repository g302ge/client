#include "client/http/connection.h"

namespace client {
namespace http {

Connection::Connection(asio::io_context& io_context)
    : io_context_(io_context), idle_timer_(io_context) {
    ssl_context_ = std::make_unique<asio::ssl::context>(asio::ssl::context::sslv23_client);
    ssl_context_->set_verify_mode(asio::ssl::context_base::verify_none);

    parser_.data = static_cast<void*>(this);
}
void Connection::SetIdleTimeout(std::chrono::milliseconds timeout,
                                OnConnectionIdleTimeout handler) {
    idle_timeout_ = timeout;
    on_idle_timeout_ = std::move(handler);
}

void Connection::DoConnect(const asio::ip::tcp::endpoint& endpoint,
                           OnConnectSuccess success,
                           OnConnectError error) {
    auto self = shared_from_this();
    auto socket = asio::ip::tcp::socket(io_context_, endpoint);
    ssl_socket_ = std::make_unique<asio::ssl::stream<asio::ip::tcp::socket>>(std::move(socket),
                                                                             *ssl_context_);
    ssl_socket_->lowest_layer().async_connect(
        endpoint,
        [self, success = std::move(success), error = std::move(error)](
            const asio::error_code& error_code) {
            if (!error_code) {
                self->ssl_socket_->async_handshake(
                    asio::ssl::stream_base::handshake_type::client,
                    [self, success = std::move(success), error = std::move(error)](
                        const asio::error_code& error_code) {
                        if (!error_code) {
                            success(self.get());
                            self->EnableIdle();
                            // TODO: change the state into idle
                        } else {
                            error(self.get(), error_code);
                        }
                    });
            } else {
                error(self.get(), error_code);
            }
        });
}

}  // namespace http
}  // namespace client

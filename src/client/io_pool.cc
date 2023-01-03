#include "client/io_pool.h"
#include <cassert>
#include <thread>

namespace client {
IoPool::IoPool() {
    shutdown_ = false;
    io_threads_num_ = 0;
}

void IoPool::Spawn(Task&& task) {
    // create new io_context and guard
    auto io_context = std::make_shared<asio::io_context>();
    auto io_work_guard =
        std::make_shared<asio::executor_work_guard<asio::io_context::executor_type>>(
            io_context->get_executor());
    auto finish_promise = std::make_shared<std::promise<void>>();
    io_contexts_.push_back(io_context);
    io_work_guards_.push_back(io_work_guard);
    io_promise_.push_back(finish_promise->get_future());
    io_threads_num_.fetch_add(1, std::memory_order_relaxed);
    std::thread([this, task = std::move(task), io_context, finish_promise]() {
        task(*io_context, this->shutdown_);
        finish_promise->set_value_at_thread_exit();
    }).detach();
}
void IoPool::Shutdown(bool join) {
    io_work_guards_.clear();
    if (join) {
        for (auto& future : io_promise_) {
            future.get();
        }
    }
}

void IoPool::Join() {
    for (auto& future : io_promise_) {
        future.get();
    }
}

}  // namespace client

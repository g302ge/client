#pragma once

#include <asio.hpp>
#include <atomic>
#include <functional>
#include <future>
#include <memory>
#include <vector>

namespace client {

class IoPool {
   public:
    using Task = std::function<void(asio::io_context&, std::atomic<bool>&)>;
    IoPool();
    // spawn a new task on a new thread
    void Spawn(Task&&);

    void Shutdown(bool join = true);

    void Join();

    size_t IoThreads() { return io_threads_num_.load(std::memory_order_relaxed); }

   private:
    std::vector<std::shared_ptr<asio::io_context>> io_contexts_;
    std::vector<std::shared_ptr<asio::executor_work_guard<asio::io_context::executor_type>>>
        io_work_guards_;
    std::vector<std::future<void>> io_promise_;
    std::atomic<size_t> io_threads_num_;
    std::atomic<bool> shutdown_;
};

}  // namespace client

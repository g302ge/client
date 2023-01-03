#include "client/io_pool.h"
#include <gtest/gtest.h>
#include <chrono>
#include <iostream>

TEST(io_pool, io_pool_test) {
    client::IoPool pool;

    pool.Spawn([](asio::io_context& io_context, std::atomic<bool>& outer) {
        std::cout << "io task start " << std::endl;
        io_context.run();
        std::cout << "io task  complete" << std::endl;
    });

    // pool.Shutdown(true);
    pool.Join();
}

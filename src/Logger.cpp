#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <atomic>
#include <memory>
#include <chrono>
#include <ctime>

// ManualMutex implementation remains the same
class ManualMutex {
public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)); // Spin until the lock is acquired
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

// Custom mutex guard for automatic lock management
class MutexGuard {
public:
    explicit MutexGuard(ManualMutex& m) : mutex(m) {
        mutex.lock();
    }

    ~MutexGuard() {
        mutex.unlock();
    }

    // Disable copying and assignment
    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;

private:
    ManualMutex& mutex;
};

// Node structure for the linked list
struct LogNode {
    std::string message;
    std::shared_ptr<LogNode> next;
    LogNode(const std::string& msg) : message(msg), next(nullptr) {}
};

// Simple Linked List implementation for the logger
class LogQueue {
public:
    void push(const std::string& message) {
        std::shared_ptr<LogNode> new_node = std::make_shared<LogNode>(message);
        if (!tail) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    std::shared_ptr<LogNode> pop() {
        if (!head) return nullptr;
        std::shared_ptr<LogNode> old_head = head;
        head = head->next;
        if (!head) {
            tail = nullptr;
        }
        return old_head;
    }

    bool empty() const {
        return head == nullptr;
    }

private:
    std::shared_ptr<LogNode> head = nullptr;
    std::shared_ptr<LogNode> tail = nullptr;
};

// Asynchronous Logger using linked list
class AsyncLogger {
public:
    AsyncLogger(const std::string& filename) : stop_logging(false) {
        log_file.open(filename, std::ios::out | std::ios::app);

        for (long i = 0; !log_file.is_open(); i++)
        {   
            std::string filename_i = filename + std::to_string(i);
            log_file.open(filename_i, std::ios::out | std::ios::app);
        }
        /*
        if (!log_file.is_open()) {
            throw std::ios_base::failure("Failed to open log file");
        }*/

        log_thread = std::thread(&AsyncLogger::processLogs, this);
    }

    ~AsyncLogger() {
        stop_logging.store(true, std::memory_order_release);
        if (log_thread.joinable()) {
            log_thread.join();
        }
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(const std::string& message) {
        MutexGuard guard(manual_mutex);
        log_queue.push(message);
    }

    void thread_function(const std::string& message) {
        
    (*this).log(message + " from thread ");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

private:
    std::ofstream log_file;
    LogQueue log_queue;
    ManualMutex manual_mutex;
    std::thread log_thread;
    std::atomic<bool> stop_logging;

    void processLogs() {
        while (!stop_logging.load(std::memory_order_acquire)) {
            {
                MutexGuard guard(manual_mutex);
                auto node = log_queue.pop();
                if (node) {
                    auto now = std::chrono::system_clock::now();
                    auto now_time_t = std::chrono::system_clock::to_time_t(now);
                    log_file << std::ctime(&now_time_t) << ": " << node->message << std::endl;
                }
            }

            // Introduce a small sleep to reduce CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};
/*
void thread_function(AsyncLogger& logger, const std::string& message) {
    for (int i = 0; i < 5; ++i) {
        logger.log(message + " from thread " + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    AsyncLogger logger("async_log_linked_list.txt");

    std::thread t1(thread_function, std::ref(logger), "Message 1");
    std::thread t2(thread_function, std::ref(logger), "Message 2");

    t1.join();
    t2.join();

    return 0;
}*/

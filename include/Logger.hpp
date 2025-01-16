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
    void lock();
    void unlock();

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

// Custom mutex guard for automatic lock management
class MutexGuard {
public:
    explicit MutexGuard(ManualMutex& m);

    ~MutexGuard();

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
    void push(const std::string& message);

    std::shared_ptr<LogNode> pop();

private:
    std::shared_ptr<LogNode> head = nullptr;
    std::shared_ptr<LogNode> tail = nullptr;
};

// Asynchronous Logger using linked list
class AsyncLogger {
public:
    static AsyncLogger& getInstance(const std::string& filename, int file_length, bool new_file_or_overwrite, int log_size) {
        static AsyncLogger instance(filename, file_length, new_file_or_overwrite, log_size);
        return instance;
    }

    void log(const std::string& message);

private:
    std::ofstream log_file;     //the open file which the log is written to
    std::shared_ptr<LogQueue> log_queue = std::make_shared<LogQueue>();     //the linked List which collects messages and is written to log-file
    ManualMutex manual_mutex;   //for file as well as queue
    std::thread log_thread;
    std::atomic<bool> stop_logging; //Shutdown safely flag
    const std::string base_filename; //standard file name before addition of count
    int current_file_length;    //length of the file currently beeing written
    int max_file_length_byte;   //maximum length of one file
    const bool new_file;  //should a new file be created by overflow?
    int max_log_size_byte;   //maximum general size independent of individual file size
    int number_of_file; //current number of log files created
    const std::string data_type;

    AsyncLogger(const std::string& filename, const int file_length_byte, const bool new_file_or_overwrite, int log_size);

    ~AsyncLogger();

    AsyncLogger(const AsyncLogger&) = delete;
    AsyncLogger& operator=(const AsyncLogger&) = delete;

    //File closing routine
    void close_file();

    //File opening routine
    void open_file(const std::string& new_filename);

    //Translate data to JSON
    std::string formatLogAsJSON(const std::string& message, const std::string& exception = "-");

    //adjust string to json format
    std::string escapeJSONString(const std::string& str);

    void processLogs();
};

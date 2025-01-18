#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <memory>
#include <chrono>
#include <ctime>
/****************************************************************************************************** */
/****************************************************************************************************** */
//Create a Class instance of Logger: Logger Log("yourfilename.json", filelenght, newfilecreation?, maxsizeoverall)
//Log by caling Log.log("message", "Exceptions")
//Regularly call Log.processLogs(); to compile and write to file
/****************************************************************************************************** */
/****************************************************************************************************** */

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
    std::string exception;
    std::shared_ptr<LogNode> next;
    LogNode(const std::string& msg, const std::string& exp) : message(msg), exception(exp), next(nullptr) {}
};

// Simple Linked List implementation for the logger
class LogQueue {
public:
    void push(const std::string& message, const std::string& exception);

    std::shared_ptr<LogNode> pop();

    bool itemInQueue();

private:
    std::shared_ptr<LogNode> head = nullptr;
    std::shared_ptr<LogNode> tail = nullptr;
};

// Abstract base class for data formatting
class DataFormatter {
public:
    virtual ~DataFormatter() = default;
    virtual std::string formatData(const std::string& message, const std::string& exception) = 0;
};

// JSON Data Formatter
class JsonDataFormatter : public DataFormatter {
public:
    //Translate data to JSON
    std::string formatData(const std::string& message, const std::string& exception) override;

private:
    //adjust string to json format
    std::string escapeJSONString(const std::string& str);
};

// XML Data Formatter
class XmlDataFormatter : public DataFormatter {
public:
    std::string formatData(const std::string& message, const std::string& exception) override;
};

// Factory class for creating data formatters
class DataFormatterFactory {
public:
    static std::unique_ptr<DataFormatter> createFormatter(const std::string& type);
};

// Asynchronous Logger using linked list
class Logger {
public:
    // static Logger& getInstance(const std::string& filename, const unsigned int file_length, const bool new_file_or_overwrite, const unsigned int log_size) {
    //     static Logger instance(filename, file_length, new_file_or_overwrite, log_size);
    //     return instance;
    // }

    Logger(const std::string& filename, const unsigned int file_length_byte, const bool new_file_or_overwrite, unsigned int log_size);

    ~Logger();

    void log(const std::string& message, const std::string& exception = "-");
    void processLogs();

private:
    std::ofstream log_file;     //the open file which the log is written to
    std::shared_ptr<LogQueue> log_queue = std::make_shared<LogQueue>();     //the linked List which collects messages and is written to log-file
    std::unique_ptr<DataFormatter> formatter;
    ManualMutex manual_mutex;   //for file as well as queue
    const std::string base_filename; //standard file name before addition of count
    unsigned int current_file_length;    //length of the file currently beeing written
    unsigned int max_file_length_byte;   //maximum length of one file
    const bool new_file;  //should a new file be created by overflow?
    unsigned int max_log_size_byte;   //maximum general size independent of individual file size
    unsigned int number_of_file; //current number of log files created
    std::string data_type;
    std::string xml_header = "<?xml version=" + std::to_string(1.0) + " encoding=" + "UTF-8" + " ?>\n";

    //File closing routine
    void close_file();

    //File opening routine
    void open_file(const std::string& new_filename);

    void writetoFile(const std::string& Log);
};

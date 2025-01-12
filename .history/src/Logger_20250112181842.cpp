#include "Logger.hpp"

void ManualMutex::lock() {
    while (flag.test_and_set(std::memory_order_acquire)); // Spin until the lock is acquired
}

void ManualMutex::unlock() {
    flag.clear(std::memory_order_release);
}

MutexGuard::MutexGuard(ManualMutex& m) : mutex(m) {
    mutex.lock();
}

MutexGuard::~MutexGuard() {
    mutex.unlock();
}

void LogQueue::push(const std::string& message) {
    std::shared_ptr<LogNode> new_node = std::make_shared<LogNode>(message);
    if (!tail) {
        head = tail = new_node;
    } else {
        tail->next = new_node;
        tail = new_node;
    }
}

std::shared_ptr<LogNode> LogQueue::pop() {
    if (!head) return nullptr;
    std::shared_ptr<LogNode> old_head = head;
    head = head->next;
    if (!head) {
        tail = nullptr;
    }
    return old_head;
}

void AsyncLogger::log(const std::string& message) {
    //convert Data to json format
    const std::string frm_message = formatLogAsJSON(message);

    //Check message and file length
    int frm_message_length = frm_message.size();
    current_file_length += frm_message_length;

    //Check if filesize ok
    if (frm_message_length + 2 > max_file_length_byte) {
            std::cout << "file size too small!" << std::endl;
    }
    //Check if length with file end is suitable
    else if ( (current_file_length + 1) < max_file_length_byte) {
        std::cout << "normal"<< std::endl;
        MutexGuard guard(manual_mutex);
        log_queue->push(frm_message);
    }
    //Create new file if too long and allowed
    else if (new_file) {
        std::cout << "new " + std::to_string(number_of_file) << std::endl;
        //MutexGuard guard(manual_mutex);
        close_file();
        current_file_length = 1;
        number_of_file++;
        if (number_of_file * max_file_length_byte >= max_log_size_byte) {
            std::cout << "overflow files " + std::to_string(current_file_length) << std::endl;
            number_of_file = 0;
        }
        std::string new_filename = base_filename.substr(0, base_filename.length()-6) + std::to_string(number_of_file) + ".json";
        open_file(new_filename);
        log_queue->push(frm_message);
    }
    //Create Loop to start of Log
    else if (current_file_length +1 >= max_file_length_byte) {
        MutexGuard guard(manual_mutex);
        log_file.seekp(1); // Move to start after opening bracket '['
        current_file_length = 1 + frm_message_length;
        std::cout << "Loop " + std::to_string(current_file_length) << std::endl;
        log_queue->push(frm_message);
        // Ensure that we wrap correctly
        if (log_file.tellp() >= static_cast<std::streampos>(max_file_length_byte)) {
            log_file.seekp(1); // Wrap around to the start
            current_file_length = 1;
        }
    }
    else {
        std::cout << "General log size Error" << std::endl;
    }
}

AsyncLogger::AsyncLogger(const std::string& filename, const int file_length_byte, const bool new_file_or_overwrite, int log_size)
: stop_logging(false), base_filename(filename), max_file_length_byte(file_length_byte), new_file(new_file_or_overwrite), max_log_size_byte(log_size), number_of_file(0) {
    open_file(filename);
    if (max_log_size_byte < max_file_length_byte) { //Check file sizes and set to smaller if log is smaller
        max_file_length_byte = max_log_size_byte;
    }
    if (!log_file.is_open()) {
        throw std::ios_base::failure("Failed to open log file");
    }
    log_thread = std::thread(&AsyncLogger::processLogs, this);
}

AsyncLogger::~AsyncLogger() {
    stop_logging.store(true, std::memory_order_release);
    if (log_thread.joinable()) {
        log_thread.join();
    }
    MutexGuard guard(manual_mutex);
    if (log_file.is_open()) {
        close_file();
    }
}

//File closing routine
void AsyncLogger::close_file() {
    log_file.seekp(-3, std::ios_base::end);
    log_file << "]";
    log_file.close();
}

//File opening routine
void AsyncLogger::open_file(const std::string& new_filename) {
    log_file.open(new_filename, std::ios::out);
    log_file.seekp(0);
    log_file << "["; //Start of JSON Array
}

//Translate data to JSON
std::string AsyncLogger::formatLogAsJSON(const std::string& message, const std::string& exception) {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&now_time_t);
    time_str.pop_back(); // Remove newline character
    std::string json_log = "{";

    // Manually construct the JSON string
    // if(current_file_length > 5) {
    //     json_log = ",{";
    // }
    // else {
    //     json_log = "{";
    // }
    json_log += "\"timestamp\": \"" + time_str + "\",";
    json_log += "\"message\": \"" + escapeJSONString(message) + "\",";
    json_log += "\"exception\": \"" + escapeJSONString(exception) + "\"";
    json_log += "},";
    return json_log;
}

//adjust string to json format
std::string AsyncLogger::escapeJSONString(const std::string& str) {
    std::string escaped_str;
    for (char c : str) {
        switch (c) {
            case '\"': escaped_str += "\\\""; break;
            case '\\': escaped_str += "\\\\"; break;
            case '\b': escaped_str += "\\b"; break;
            case '\f': escaped_str += "\\f"; break;
            case '\n': escaped_str += "\\n"; break;
            case '\r': escaped_str += "\\r"; break;
            case '\t': escaped_str += "\\t"; break;
            default:
                escaped_str += c;
        }
    }
    return escaped_str;
}

void AsyncLogger::processLogs() {
    while (!stop_logging.load(std::memory_order_acquire)) {
        {
            MutexGuard guard(manual_mutex);
            auto node = log_queue->pop();
            if (node) {
                log_file << node->message << std::endl;
            }
        }

        // Introduce a small sleep to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}


void thread_function(AsyncLogger& logger, const std::string& message) {
for (int i = 0; i < 5; ++i) {
    logger.log(message + " from thread " + std::to_string(i));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
}

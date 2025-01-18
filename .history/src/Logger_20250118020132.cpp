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

void LogQueue::push(const std::string& message, const std:: string& exception) {
    std::shared_ptr<LogNode> new_node = std::make_shared<LogNode>(message, exception);
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

bool LogQueue::itemInQueue() {
    if (!head) {
        return false;
    } else {
        return true;
    }
}

std::string XmlDataFormatter::formatData(const std::string& message, const std::string& exception) {
    // Simple XML formatting
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&now_time_t);
    time_str.pop_back(); // Remove newline character
    // XML formatting for logging
    std::string formattedData =
        "<log>\n"
        "    <date>" + time_str.substr(0, time_str.find(":")-2) + "</date>\n"
        "    <time>" + time_str.substr(time_str.find(":")-2, time_str.size()) + "</time>\n"
        "    <message>" + message + "</message>\n"
        "    <exception>" + exception + "</exception>\n"
        "</log>\n";
    return formattedData;
}


//Translate data to JSON
std::string JsonDataFormatter::formatData(const std::string& message, const std::string& exception) {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&now_time_t);
    time_str.pop_back(); // Remove newline character
    std::string json_log = "{";
    json_log += "\"timestamp\": \"" + time_str + "\",";
    json_log += "\"message\": \"" + escapeJSONString(message) + "\",";
    json_log += "\"exception\": \"" + escapeJSONString(exception) + "\"";
    json_log += "},\n";
    return json_log;
}

//adjust string to json format
std::string JsonDataFormatter::escapeJSONString(const std::string& str) {
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

std::unique_ptr<DataFormatter> DataFormatterFactory::createFormatter(const std::string& type) {
    if (type == ".xml") {
        return std::make_unique<XmlDataFormatter>();
    } else if (type == ".json") {
        return std::make_unique<JsonDataFormatter>();
    }
    return nullptr;
}

Logger::Logger(const std::string& filename, const unsigned int file_length_byte, const bool new_file_or_overwrite, unsigned int log_size)
: base_filename(filename), max_file_length_byte(file_length_byte), new_file(new_file_or_overwrite), max_log_size_byte(log_size), number_of_file(0) {
    //Check file sizes and set to smaller if log is smaller
    if (max_log_size_byte < max_file_length_byte) {
        max_file_length_byte = max_log_size_byte;
    }

    //Determine file type
    if (base_filename.find(".json") != std::string::npos) {
        data_type = ".json";
    }
    else if (base_filename.find(".xml") != std::string::npos) {
        data_type = ".xml";
    }
    else {
        data_type = ".json";
        std::cout << "File type not supported! .json auto selected" << std::endl;
    }

    formatter = DataFormatterFactory::createFormatter(data_type);

    open_file(filename);
}

Logger::~Logger() {
    while(log_queue->itemInQueue()){
        processLogs();
    }
    if (log_file.is_open()) {
        close_file();
    }
}

void Logger::log(const std::string& message, const std::string& exception) {
    MutexGuard guard(manual_mutex);
    log_queue->push(message, exception);
}

//File opening routine
void Logger::open_file(const std::string& new_filename) {
    if(log_file.is_open()) {
        log_file.close();
    }

    MutexGuard guard(manual_mutex);
    log_file.open(new_filename, std::ios::out);
    log_file.seekp(0);
    if (data_type == ".json"){
        log_file << "["; //Start of JSON Array
        current_file_length = 1;
    }
    else if (data_type == ".xml") {
        log_file << xml_header;
        current_file_length = xml_header.size();
    }
}

//File closing routine
void Logger::close_file() {
    MutexGuard guard(manual_mutex);
    if ( data_type == ".json"){
        log_file.seekp(-3, std::ios_base::end);
        log_file << "]"; //End of JSON Array
    }
    log_file.close();
}

void Logger::writetoFile(const std::string& Log) {
    MutexGuard guard(manual_mutex);
    log_file << Log;
}

void Logger::processLogs() {
    if (log_queue->itemInQueue()){
        auto node = log_queue->pop();
        if (node) {
            //Convert Data to format
            const std::string frm_message = formatter->formatData(node->message, node->exception);

            //Check message and file length
            unsigned frm_message_length = frm_message.size();
            current_file_length += frm_message_length;

            //Check if filesize ok
            if (frm_message_length + 2 > max_file_length_byte) {
                    std::cout << "file size too small!" << std::endl;
            }

            //Check if length with file end is suitable
            else if ( (current_file_length + 1) < max_file_length_byte) {
                writetoFile(frm_message);
            }

            //Create new file if too long and allowed
            else if (new_file) {
                number_of_file++;
                //Check maximum total Log size and loop if necessary
                if (number_of_file * max_file_length_byte >= max_log_size_byte) {
                    std::cout << "overflow files " + std::to_string(current_file_length) << std::endl;
                    number_of_file = 0;
                }
                //Get number of file
                std::string number = std::to_string(number_of_file);
                //Create new file name
                std::string new_filename = base_filename.substr(0, base_filename.length()-(unsigned int)(number.size() + data_type.size())) + number + data_type;
                //Close full file
                close_file();
                //Open new file and start writing
                open_file(new_filename);
                //Reset Length counter and increment number of file
                current_file_length += frm_message_length;
                writetoFile(frm_message);
            }

            //Create Loop to start of Log
            else if (current_file_length +1 >= max_file_length_byte) {
                if (data_type == ".xml") {
                    unsigned int start_pos = xml_header.size();
                    log_file.seekp(start_pos);
                    current_file_length = start_pos + frm_message_length;
                } else {            // Move to start after opening bracket '['
                    log_file.seekp(1);
                    current_file_length = 1 + frm_message_length;
                }
                writetoFile(frm_message);
            }
            else {
                std::cout << "General log size Error" << std::endl;
            }
        }
    }
}

void thread_function(Logger& logger, const std::string& message) {
    for (int i = 0; i < 5; ++i) {
        logger.log(message + " Message " + std::to_string(i));
    logger.processLogs();
    }
}

int main() {
    //Logger& logger = Logger::getInstance("log_0.json", 2000, true, 10000);
    Logger logger("log_0.json", 2000, true, 10000);

    std::thread t1(thread_function, std::ref(logger), "Thread 1");
    std::thread t2(thread_function, std::ref(logger), "Thread 2");
    std::thread t3(thread_function, std::ref(logger), "Thread 3");
    std::thread t4(thread_function, std::ref(logger), "Thread 4");
    std::thread t5(thread_function, std::ref(logger), "Thread 5");
    std::thread t6(thread_function, std::ref(logger), "Thread 6");

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    return 0;
}

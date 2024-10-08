#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <mutex>
#include <thread>
#include <deque>
#include <atomic>

class Logger{
public: 
    Logger(std::string log_filename = "log.txt");
    ~Logger();

    void write(const std::string &msg);
    void print();

private:
    std::string filename;
    std::ofstream file;
    std::mutex mtx;

    std::thread writer_thread;
    std::condition_variable cv;
    std::deque<std::string> buffer;
    std::atomic<bool> is_running{false};

    void writer();
    void put(const std::string &msg);

};

#endif
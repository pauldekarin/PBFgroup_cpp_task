#include "../logger.hpp"

Logger::Logger(std::string log_filename):
filename(log_filename),
is_running(true)
{
    this->file.open(this->filename, std::fstream::app);
    this->writer_thread = std::thread{&Logger::writer, this};
}

Logger::~Logger(){
    if(this->file.is_open()){
        this->file.close();
    }

    this->is_running.store(false, std::memory_order_relaxed);
    this->cv.notify_all();

    if(this->writer_thread.joinable()){
        this->writer_thread.join();
    }
}

void Logger::writer(){
    while(this->is_running.load(std::memory_order_relaxed)){
        std::unique_lock<std::mutex> lock(this->mtx);
        this->cv.wait(lock);

        while(!this->buffer.empty()){
            this->put(this->buffer.front());
            this->buffer.pop_front();
        }
    }
}

void Logger::put(const std::string &msg){
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(tp);
    tm *lt = std::localtime(&t);


    std::ostringstream os;
    os 
        << "[" 
        << std::put_time(lt, "%Y-%m-%d %H:%M:%S")
        << "." 
        << std::setw(3) 
        << std::setfill('0')
        << std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() %  1000
        << "] "
        << msg
        << "\n";
    this->file.write(os.str().c_str(), os.str().size());
}
void Logger::write(const std::string &msg){
    if(!this->file.is_open()) return;
    std::lock_guard<std::mutex> lock(this->mtx);
    this->buffer.push_front(msg);
    this->cv.notify_all();
}

void Logger::print(){

}


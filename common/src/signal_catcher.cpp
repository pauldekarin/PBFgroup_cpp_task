#include "../signal_catcher.hpp"

SignalCatcher::SignalCatcher(){
    signal(SIGINT, SignalCatcher::sigint_catcher);
}

SignalCatcher* SignalCatcher::get_instance(){
    if(instance == nullptr){
        instance = new SignalCatcher();
    }

    return instance;
}

void SignalCatcher::bind(std::function<void(int signum)> func){
    instance->sigint_handler = std::move(func);

}

void SignalCatcher::sigint_catcher(std::int32_t signum){
    if(instance->sigint_handler){
        instance->sigint_handler(signum);
    }else{
        std::exit(signum);
    }
}

SignalCatcher* SignalCatcher::instance = new SignalCatcher();
#ifndef SIGNAL_CATCHER_HPP
#define SIGNAL_CATCHER_HPP

#include <iostream>
#include <memory>
#include <functional>
#include <csignal>

class SignalCatcher{
public:
    static SignalCatcher* get_instance();
    void bind(std::function<void(std::int32_t signum)>);

private:
    SignalCatcher();
    static SignalCatcher* instance;
    std::function<void(std::int32_t)> sigint_handler;
    static void sigint_catcher(std::int32_t signum);
};

#endif
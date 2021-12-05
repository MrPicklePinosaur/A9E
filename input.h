#ifndef __INPUT_H__
#define __INPUT_H__

#include <memory>
#include <thread>

class CursesInput
{
    std::unique_ptr<std::thread> listen_thread;
public:
    CursesInput();
    ~CursesInput();
    void ListenInput();
};

#endif // __INPUT_H__


#include <memory>
#include <thread>
#include <iostream>

#include "input.h"

CursesInput::CursesInput():
    listen_thread{std::make_unique<std::thread>(&CursesInput::ListenInput, this)} {}

CursesInput::~CursesInput() {}

void
CursesInput::ListenInput()
{
    while (true) {

        // not using the builtin curses getch since that doesn't have multithread support
        int c = getchar();
        if (c < 0) continue;
        std::cout << "got char " << c << std::endl;

    }
}

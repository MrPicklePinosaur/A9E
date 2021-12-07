
#include <memory>
#include <thread>
#include <iostream>

#include "input.h"

CursesInput::CursesInput():
    Input{}, listen_thread{std::make_unique<std::thread>(&CursesInput::ListenInput, this)} {}

CursesInput::~CursesInput() {}

void
CursesInput::ListenInput()
{
    while (true) {

        // not using the builtin curses getch since that doesn't have multithread support
        int c = getchar();
        if (c < 0) continue;

        SetKeyDown(c);
    }
}

bool
CursesInput::GetKeyDown(char c)
{
    return key_map.test(static_cast<size_t>(c));
}

void
CursesInput::ClearKeyMap()
{
    key_map.reset();
}

void
CursesInput::SetKeyDown(char c)
{
    key_map.set(static_cast<size_t>(c));
}

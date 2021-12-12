
#include <iostream>
#include "inputer.h"

CursesInputer::CursesInputer():
    Inputer{}, listen_thread{std::make_unique<std::thread>(&CursesInputer::ListenInputer, this)} {}

CursesInputer::~CursesInputer()
{
    listen = false;
    listen_thread->join();
}

void
CursesInputer::ListenInputer()
{
    while (listen) {

        // not using the builtin curses getch since that doesn't have multithread support
        int c = getchar();
        if (c < 0) continue;

        SetKeyDown(c);
    }
}

bool
CursesInputer::GetKeyDown(char c)
{
    return key_map.test(static_cast<size_t>(c));
}

void
CursesInputer::ClearKeyMap()
{
    key_map.reset();
}

void
CursesInputer::SetKeyDown(char c)
{
    key_map.set(static_cast<size_t>(c));
}


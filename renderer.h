
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <memory>
#include <ncurses.h>
#include "window.h"

class CursesRenderer
{
    std::unique_ptr<GameWindow> game_win;
    std::unique_ptr<Window> status_win;

public:
    CursesRenderer();
    ~CursesRenderer();
private:
    void curses_init();
    void curses_exit();
};

#endif // __RENDERER_H__

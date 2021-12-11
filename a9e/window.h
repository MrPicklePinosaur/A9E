#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <ncurses.h>

// raii wrapper around ncurses's WINDOW
class Window
{
protected:
    WINDOW* win;
public:
    Window(int w, int h, int y, int x);
    ~Window();
    WINDOW* getwin();
};

Window::Window(int h, int w, int y, int x):
    win{newwin(h, w, y, x)}
{
    refresh();
}

Window::~Window()
{
    delwin(win);
}

WINDOW*
Window::getwin()
{
    return win;
}

#endif // __WINDOW_H__

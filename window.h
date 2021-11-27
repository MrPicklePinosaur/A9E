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

#endif // __WINDOW_H__

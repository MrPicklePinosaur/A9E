#ifndef __A9E_WINDOW_H__
#define __A9E_WINDOW_H__

#include <ncurses.h>

// raii wrapper around ncurses's WINDOW
class Window
{
protected:
    WINDOW* win;
public:
    Window(int h, int w, int y, int x);
    ~Window();
    WINDOW* getwin();
};

#endif // __A9E_WINDOW_H__

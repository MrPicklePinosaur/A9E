
#include "window.h"

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


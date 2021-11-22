
#include <ncurses.h>
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

void
Window::render()
{
    pre_render();
    wrefresh(win);
}

void Window::pre_render() {}
void Window::main_render() {}

GameWindow::GameWindow(int w, int h, int y, int x):
    Window{w, h, y, x}
{
}

GameWindow::~GameWindow() {}

void
GameWindow::pre_render()
{
    box(win, 0, 0);
}

void
GameWindow::main_render()
{
}



#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <memory>
#include <ncurses.h>
#include "window.h"

class CursesRenderer
{
    std::unique_ptr<Window> game_win;
    std::unique_ptr<Window> status_win;

public:
    CursesRenderer();
    ~CursesRenderer();
    void DrawChar(char c, int x, int y);
    void DrawBox(char c, int x, int y, int w, int h);
    void WriteStatus(const std::string& s, int line);
    void RenderGameScreen();
    void RenderStatusScreen();
    void ClearGameScreen();
    void ClearStatusScreen();
private:
    void curses_init();
    void curses_exit();
};

#endif // __RENDERER_H__

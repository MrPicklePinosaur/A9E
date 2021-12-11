#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <memory>
#include <ncurses.h>
#include <string>

#include "window.h"
#include "config.h"

class Renderer
{
public:
    Renderer() {}
    virtual ~Renderer() {}
    virtual void DrawChar(char c, int x, int y) = 0;
    virtual void DrawBox(char c, int x, int y, int w, int h) = 0;
    virtual void WriteStatus(const std::string& s, int line) = 0;
    virtual void RefreshGameScreen() = 0;
    virtual void RefreshStatusScreen() = 0;
    virtual void DrawGameScreen() = 0;
    virtual void DrawStatusScreen() = 0;
    virtual void ClearGameScreen() = 0;
    virtual void ClearStatusScreen() = 0;
};

class CursesRenderer final : public Renderer
{
    std::unique_ptr<Window> game_win;
    std::unique_ptr<Window> status_win;
public:
    CursesRenderer();
    ~CursesRenderer();
    void DrawChar(char c, int x, int y) override;
    void DrawBox(char c, int x, int y, int w, int h) override;
    void WriteStatus(const std::string& s, int line) override;
    void RefreshGameScreen() override;
    void RefreshStatusScreen() override;
    void DrawGameScreen() override;
    void DrawStatusScreen() override;
    void ClearGameScreen() override;
    void ClearStatusScreen() override;
private:
    void CursesInit();
    void CursesExit();
};

CursesRenderer::CursesRenderer(): Renderer{}
{
    CursesInit();

    game_win = std::make_unique<Window>(SCREEN_HEIGHT-STATUS_HEIGHT, SCREEN_WIDTH, 0, 0);
    status_win = std::make_unique<Window>(STATUS_HEIGHT, SCREEN_WIDTH, SCREEN_WIDTH-STATUS_HEIGHT, 0);

    refresh();
}

CursesRenderer::~CursesRenderer()
{
    CursesExit();
}

void
CursesRenderer::DrawChar(char c, int x, int y)
{
    if (x < 0 || y < 0) return; 
    if (x > SCREEN_WIDTH || y > SCREEN_HEIGHT) return; // TODO account for borders and stuff

    mvwprintw(game_win->getwin(), y, x, std::string(1, c).c_str());
}

void
CursesRenderer::DrawBox(char c, int x, int y, int w, int h)
{
    for (int i = 0; i < h; ++i)
        mvwprintw(game_win->getwin(), y+i, x, std::string(w, c).c_str());
}

void
CursesRenderer::WriteStatus(const std::string& s, int line)
{
    // clear line beforehand
    wmove(status_win->getwin(), line, 0);
    wclrtoeol(status_win->getwin());

    mvwprintw(status_win->getwin(), line, 0, s.c_str());
}

void
CursesRenderer::RefreshGameScreen()
{
    wrefresh(game_win->getwin());
}

void
CursesRenderer::RefreshStatusScreen()
{
    wrefresh(status_win->getwin());
}

void
CursesRenderer::DrawGameScreen()
{
    box(game_win->getwin(), 0, 0);
}

void
CursesRenderer::DrawStatusScreen()
{
}

void
CursesRenderer::ClearGameScreen()
{
    werase(game_win->getwin());
}

void
CursesRenderer::ClearStatusScreen()
{
    werase(status_win->getwin());
}

void
CursesRenderer::CursesInit()
{
    // need to make sure this isn't called twice

    // some startup options for curses
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
}

void
CursesRenderer::CursesExit()
{
    endwin();
}

#endif // __RENDERER_H__

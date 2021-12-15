
#include <ncurses.h>
#include "renderer.h"

CursesRenderer::CursesRenderer(bool enable_color, int screen_width, int screen_height, int status_height):
    Renderer{}, color_mode{enable_color}, screen_width{screen_width}, screen_height{screen_height}, status_height{status_height}
{
    CursesInit();

    if (enable_color) CursesColorInit();

    game_win = std::make_unique<Window>(screen_height-status_height, screen_width, 0, 0);
    status_win = std::make_unique<Window>(status_height, screen_width, screen_height-status_height, 0);

    refresh();
}

CursesRenderer::~CursesRenderer()
{
    CursesExit();
}

void
CursesRenderer::DrawChar(char c, int x, int y, RenderStyleId s)
{
    if (x < 0 || y < 0) return;
    // TODO account for borders and stuff
    /* if (x > screen_width || y > screen_height) return; */

    EnableRenderStyle(s);
    mvwprintw(game_win->getwin(), y, x, std::string(1, c).c_str());
    DisableRenderStyle(s);
}

void
CursesRenderer::DrawBox(char c, int x, int y, int w, int h, RenderStyleId s)
{
    EnableRenderStyle(s);
    for (int i = 0; i < h; ++i)
        mvwprintw(game_win->getwin(), y+i, x, std::string(w, c).c_str());
    DisableRenderStyle(s);
}

void
CursesRenderer::DrawText(const std::string& text, int x, int y, RenderStyleId s)
{
    mvwprintw(game_win->getwin(), y, x, text.c_str());
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
CursesRenderer::WriteStatus(const std::string& s, int x, int y)
{
    mvwprintw(status_win->getwin(), y, x, s.c_str());
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
CursesRenderer::RegisterColorPair(ColorPairId id, Color fg_color, Color bg_color)
{
    init_pair(id, fg_color, bg_color);
}


void
CursesRenderer::RegisterStyle(RenderStyleId id, RenderStyle style)
{
    if (id == 0) return;
    style_map[id] = style;
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
CursesRenderer::CursesColorInit()
{
    if (FALSE == has_colors()) throw "Your terminal does not support color";
	start_color();
}

void
CursesRenderer::CursesExit()
{
    endwin();
}

void
CursesRenderer::EnableRenderStyle(RenderStyleId id)
{
    if (id == 0) return; // render style id of 0 is special - it does nothing
    const RenderStyle& style = style_map.at(id);
    attron(COLOR_PAIR(style.c_id));
    attron(style.attr);
}

void
CursesRenderer::DisableRenderStyle(RenderStyleId id)
{
    if (id == 0) return;
    const RenderStyle& style = style_map.at(id);
    attroff(COLOR_PAIR(style.c_id));
    attroff(style.attr);
}


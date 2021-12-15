#ifndef __A9E_RENDERER_H__
#define __A9E_RENDERER_H__

#include <map>
#include <memory>
#include <string>

#include "window.h"

using RenderStyleId = int;

class Renderer
{
public:
    Renderer() {}
    virtual ~Renderer() {}
    virtual void DrawChar(char c, int x, int y, RenderStyleId s = 0) = 0;
    virtual void DrawBox(char c, int x, int y, int w, int h, RenderStyleId s = 0) = 0;
    virtual void DrawText(const std::string& text, int x, int y, RenderStyleId s = 0) = 0;
    virtual void WriteStatus(const std::string& s, int line) = 0;
    virtual void WriteStatus(const std::string& s, int x, int y) = 0;
    virtual void RefreshGameScreen() = 0;
    virtual void RefreshStatusScreen() = 0;
    virtual void DrawGameScreen() = 0;
    virtual void DrawStatusScreen() = 0;
    virtual void ClearGameScreen() = 0;
    virtual void ClearStatusScreen() = 0;

    virtual int GetScreenWidth() const = 0;
    virtual int GetScreenHeight() const = 0;
    virtual int GetStatusHeight() const = 0;
};

class CursesRenderer final : public Renderer
{
public:
    using ColorPairId = int;
    // TODO: maybe use enum class
    enum Color { BLACK = 0, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };
    enum Attr { NORMAL = A_NORMAL, STANDOUT = A_STANDOUT, UNDERLINE = A_UNDERLINE, REVERSE = A_REVERSE, BLINK = A_BLINK, DIM = A_DIM, BOLD = A_BOLD, PROTECT = A_PROTECT, INVIS = A_INVIS, ALTCHARSET = A_ALTCHARSET, CHARTEXT = A_CHARTEXT };

    struct RenderStyle {
        ColorPairId c_id;
        Attr attr;
    };

private:
    std::unique_ptr<Window> game_win;
    std::unique_ptr<Window> status_win;

    std::map<RenderStyleId,RenderStyle> style_map;

    bool color_mode;
    int screen_width, screen_height, status_height;

public:
    CursesRenderer(bool enable_color, int screen_width = 80, int screen_height = 25, int status_height = 3);
    ~CursesRenderer();
    void DrawChar(char c, int x, int y, RenderStyleId s = 0) override;
    void DrawBox(char c, int x, int y, int w, int h, RenderStyleId s = 0) override;
    void DrawText(const std::string& text, int x, int y, RenderStyleId s = 0) override;
    void WriteStatus(const std::string& s, int line) override;
    void WriteStatus(const std::string& s, int x, int y) override;
    void RefreshGameScreen() override;
    void RefreshStatusScreen() override;
    void DrawGameScreen() override;
    void DrawStatusScreen() override;
    void ClearGameScreen() override;
    void ClearStatusScreen() override;

    inline int GetScreenWidth() const { return screen_width; }
    inline int GetScreenHeight() const { return screen_height; }
    inline int GetStatusHeight() const { return status_height; }

    void RegisterColorPair(ColorPairId id, Color fg_color, Color bg_color);
    void RegisterStyle(RenderStyleId id, RenderStyle style);

private:
    void CursesInit();
    void CursesColorInit();
    void CursesExit();
    void EnableRenderStyle(RenderStyleId id);
    void DisableRenderStyle(RenderStyleId id);
};

#endif // __A9E_RENDERER_H__

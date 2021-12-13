#ifndef __A9E_RENDERER_H__
#define __A9E_RENDERER_H__

#include <memory>
#include <string>

#include "window.h"

class Renderer
{
public:
    Renderer() {}
    virtual ~Renderer() {}
    virtual void DrawChar(char c, int x, int y) = 0;
    virtual void DrawBox(char c, int x, int y, int w, int h) = 0;
    virtual void DrawText(const std::string& text, int x, int y) = 0;
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
};

class CursesRenderer final : public Renderer
{
    std::unique_ptr<Window> game_win;
    std::unique_ptr<Window> status_win;

    int screen_width, screen_height, status_height;
public:
    CursesRenderer(bool enable_color, int screen_width = 80, int screen_height = 25, int status_height = 3);
    ~CursesRenderer();
    void DrawChar(char c, int x, int y) override;
    void DrawBox(char c, int x, int y, int w, int h) override;
    void DrawText(const std::string& text, int x, int y) override;
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
private:
    void CursesInit();
    void CursesColorInit();
    void CursesExit();
};

#endif // __A9E_RENDERER_H__

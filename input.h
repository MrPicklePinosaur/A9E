#ifndef __INPUT_H__
#define __INPUT_H__

#include <memory>
#include <thread>
#include <bitset>

class Input
{
public:
    virtual ~Input() {}
    virtual bool GetKeyDown(char c) = 0;
    virtual void ClearKeyMap() = 0;
};

class CursesInput final : Input
{
    std::unique_ptr<std::thread> listen_thread;
    std::bitset<256> key_map;
public:
    CursesInput();
    ~CursesInput();
    bool GetKeyDown(char c) override;
    void ClearKeyMap() override;
private:
    void ListenInput();
    void SetKeyDown(char c);
};

#endif // __INPUT_H__

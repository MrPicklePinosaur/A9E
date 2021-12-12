#ifndef __A9E_INPUTER_H__
#define __A9E_INPUTER_H__

#include <memory>
#include <thread>
#include <bitset>

class Inputer
{
public:
    virtual ~Inputer() {}
    virtual bool GetKeyDown(char c) = 0;
    virtual void ClearKeyMap() = 0;
};

class CursesInputer final : public Inputer
{
    std::unique_ptr<std::thread> listen_thread;
    std::bitset<256> key_map;
    bool listen = true;
public:
    CursesInputer();
    ~CursesInputer();
    bool GetKeyDown(char c) override;
    void ClearKeyMap() override;
private:
    void ListenInputer();
    void SetKeyDown(char c);
};

#endif // __A9E_INPUTER_H__

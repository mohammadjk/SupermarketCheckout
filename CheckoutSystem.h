#ifndef CHECKOUT_SYSTEM_H_
#define CHECKOUT_SYSTEM_H_

#include "CommonTypes.h"

class CheckoutSystem
{
public:
    CheckoutSystem() : m_isRunning(false) {}

    bool Init();
    void Run();
    void Close();

    void EventHandler(const Event& event);

private:
    bool InitStorageManager();
    bool InitUI();

    bool m_isRunning;
};

#endif //CHECKOUT_SYSTEM_H_
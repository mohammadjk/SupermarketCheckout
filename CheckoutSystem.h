#ifndef CHECKOUT_SYSTEM_H_
#define CHECKOUT_SYSTEM_H_

#include "CommonTypes.h"

class BasketManagerIF;
class StorageManagerIF;
class CheckoutUI;

class CheckoutSystem
{
public:
    CheckoutSystem();

    bool Init();
    void Run();
    void Close();

    void EventHandler(const Event& event);

private:
    bool InitStorageManager();
    bool InitUI();

    bool m_isRunning;

    BasketManagerIF*    m_pBasketManagerIF;
    StorageManagerIF*   m_pStorageManagerIF;
    CheckoutUI*         m_pUI;  
};

#endif //CHECKOUT_SYSTEM_H_
#ifndef CHECKOUT_UI_H_
#define CHECKOUT_UI_H_

#include "NonCopyable.h" 
#include "CommonTypes.h"

#include <functional>
#include <thread>

class StorageManagerIF;
class BasketManagerIF;

class CheckoutUI : NonCopyable
{
    using EventCallback = std::function<void(const Event& event)>;

public:
    struct Config
    {
        /** configs */ 
    };

public:
    static CheckoutUI& GetInstance() {
        static CheckoutUI sys;
        return sys;
    }

    ~CheckoutUI(){}

    /** Initialises the UI. */
    bool Init(const CheckoutUI::Config& cfg);

    /** Starts the UI thread. */
    bool Start();

    /** Stops the UI thread. */
    void Stop();

    /** Register a BasketManager with the UI. */
    void RegisterBasketManager(BasketManagerIF* basketManager) {m_pBasketManager = basketManager;}

    /** Register a StorageManager with the UI. */
    void RegisterStorageManager(StorageManagerIF* storageManager) {m_pStorageManager = storageManager;}

    /** Register a callback to send UI events to the main app to be handled. */
    void RegisterEventHandler(EventCallback callback) {m_eventCallback = std::move(callback);}

private:
    CheckoutUI() 
        : m_isInit(false)
        , m_pStorageManager(nullptr)
        , m_pBasketManager(nullptr) 
        {}

    void UIThreadFunc();
    

    bool                m_isInit;

    std::thread         m_uiThread;
    EventCallback       m_eventCallback;
    
    BasketManagerIF*    m_pBasketManager;
    StorageManagerIF*   m_pStorageManager;
};

#endif // CHECKOUT_UI_H_
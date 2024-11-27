#ifndef CHECKOUT_UI_H_
#define CHECKOUT_UI_H_

#include "NonCopyable.h" 
#include "CommonTypes.h"

#include <functional>
#include <thread>

class StorageManager;
class BasketManager;

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

    /** Register a StorageManager with the UI. */
    void RegisterStorageManager(StorageManager* storageManager) {m_pStorageManager = storageManager;}

    /** Register a BasketManager with the UI. */
    void RegisterBasketManager(BasketManager* basketManager) {m_pBasketManager = basketManager;}

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
    
    StorageManager*     m_pStorageManager;
    BasketManager*      m_pBasketManager;
};

#endif // CHECKOUT_UI_H_
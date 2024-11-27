#include "CheckoutSystem.h"

#include <thread>
#include <functional>
#include <iostream>

#include "StorageManager.h"
#include "BasketManager.h"
#include "CheckoutUI.h"


/** Helper functions */
namespace
{
    /** Returns string representation of a discountType. */
    const char* EventIdToString(EventId eid) 
    {
        switch(eid)
        {
            case EventId::eCheckoutStarted:     return "Checkout Started";
            case EventId::eCheckoutCompleted:   return "Checkout Completed";
            case EventId::eUIClosed:            return "UI Closed";
            
            default: return "Unkown";
        };
    }
}

/** App initialisation. */
bool CheckoutSystem::Init() 
{    
    /** Configure components */
    if (!InitStorageManager()) {
        std::cerr << "Failed to access database.\n";
        return false;
    }

    if (!InitUI()) {
        std::cerr << "Failed to initialise the UI.\n";
        return false;
    }

    return true;
}

void CheckoutSystem::Run()
{    
    /** Launch UI */
    if (!(m_isRunning = CheckoutUI::GetInstance().Start())) {
        std::cerr << "Failed to start the UI\n";
        return;
    }
    
    while (m_isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    CheckoutUI::GetInstance().Stop();
}

void CheckoutSystem::Close()
{
}

// Private interface

bool CheckoutSystem::InitStorageManager()
{
    StorageManager::Config dbCfg;
    return StorageManager::GetInstance().Init(dbCfg);
}

bool CheckoutSystem::InitUI()
{
    CheckoutUI::Config uiCfg;
    if (!CheckoutUI::GetInstance().Init(uiCfg)) {
        return false;
    }

    /** Register a StorageManager with UI */
    CheckoutUI::GetInstance().RegisterStorageManager(&StorageManager::GetInstance());

    /** Register a BasketManager with UI */
    CheckoutUI::GetInstance().RegisterBasketManager(&BasketManager::GetInstance());
    
    /** Register an event handler for UI events */
    CheckoutUI::GetInstance().RegisterEventHandler(std::bind(&CheckoutSystem::EventHandler, this, std::placeholders::_1));

    return true;
}

 /** Event handler */
void CheckoutSystem::EventHandler(const Event& event)
{
    std::cout << "EventReceived - EventID[" << std::to_string(event.id) << "] Event[" << EventIdToString(event.id) << "]\n";

    switch(event.id)
    {
        case EventId::eCheckoutStarted: {
            /** Handle event */
        }
        break;

        case EventId::eCheckoutCompleted: {
            /** Handle event */
            BasketManager::GetInstance().Reset();
        }
        break;

        case EventId::eUIClosed: {
            m_isRunning = false;
        }
        break;

        default: 
        {
        }
        break;
    };
}



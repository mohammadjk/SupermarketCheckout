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

CheckoutSystem::CheckoutSystem() 
    : m_isRunning(false)
    , m_pBasketManagerIF(&BasketManager::GetInstance())
    , m_pStorageManagerIF(&StorageManager::GetInstance()) 
    , m_pUI(&CheckoutUI::GetInstance())
{
    /** Register a StorageManager with UI */
    m_pUI->RegisterStorageManager(m_pStorageManagerIF);

    /** Register a BasketManager with UI */
    m_pUI->RegisterBasketManager(m_pBasketManagerIF);
    
    /** Register an event handler for UI events */
    m_pUI->RegisterEventHandler(std::bind(&CheckoutSystem::EventHandler, this, std::placeholders::_1));
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
    if (!(m_isRunning = m_pUI->Start())) {
        std::cerr << "Failed to start the UI\n";
        return;
    }
    
    while (m_isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    m_pUI->Stop();
}

void CheckoutSystem::Close()
{
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
            m_pBasketManagerIF->Reset();
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

// Private interface

bool CheckoutSystem::InitStorageManager()
{
    StorageManager::Config dbCfg;
    return m_pStorageManagerIF->Init(dbCfg);
}

bool CheckoutSystem::InitUI()
{
    CheckoutUI::Config uiCfg;
    return m_pUI->Init(uiCfg);
}



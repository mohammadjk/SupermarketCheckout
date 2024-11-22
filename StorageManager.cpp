#include "StorageManager.h"

StorageManager& StorageManager::GetInstance() {
    static StorageManager storageMngr;
    return storageMngr;
}

StorageManager::~StorageManager() 
{
}

bool StorageManager::Init(const StorageManager::Config& cfg) 
{
    /** Configure StorageManager using the cfg provided. */

    // Open connection to a db
    if (!Open(cfg.dbPath, m_dbHandle)) {
        /** Log errors */
        return false;
    }

    if (!Read(m_dbHandle)) {
        /** Log errors */
        return false;
    }

    if(!ParseData()) {
        /** Log errors */
        return false;
    }

    return (m_isInit = true); 
}


StorageManager::StorageManager() : m_isInit(false), m_dbHandle(nullptr) 
{
}


bool StorageManager::Open(const std::string& dbPath, DBHandle& handle) 
{
    return true;
}

bool StorageManager::Read(const DBHandle& handle) 
{
     return true;
}

bool StorageManager::ParseData() 
{
    // We are hard-coding the store items here, but in real-life it should be loaded into the syetem from a permanent storage.
    // In addition it should be a read-only view at this level, since we don't want to add any nre items from a checkout system.
    m_storeItems = ItemsList{
            {10001, "Milk",   1.80, DiscountType::eNone},
            {10002, "Butter", 2.75, DiscountType::eBuy3Get1Free},
            {10003, "Cheese", 2.90, DiscountType::eBuy3CheapestFree},
            {10004, "Honey",  7.99, DiscountType::eBuy3CheapestFree},
            {10005, "Coffee", 9.20, DiscountType::eBuy3CheapestFree},
            {10006, "Bread",  3.60, DiscountType::eBuy3Get1Free}
    };

    return true;
}
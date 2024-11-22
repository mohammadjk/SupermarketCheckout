#ifndef STORAGE_MANAGER_H_
#define STORAGE_MANAGER_H_

#include "NonCopyable.h"
#include "CommonTypes.h"

/** 
 * Singleton class - It provides an interface into a permanent storage.
 */
class StorageManager : NonCopyable
{
    using DBHandle = void*;
public:
    struct Config
    {
        std::string dbPath;
        /** Other configs */
    };

public:
    /** Factory function */
    static StorageManager& GetInstance();
    ~StorageManager();

    /** Initialises the StorageManager using a config. */
    bool Init(const StorageManager::Config& cfg);

    inline bool IsInit() const { return m_isInit; } 
    inline const ItemsList& GetStoreItemsList() const {return m_storeItems;}

private:
    /** Hide C'tor from outside. */
    StorageManager();

    /** Opens a connection to a DB. */
    bool Open(const std::string& dbPath, DBHandle& handle);

    /** Reads from the DB. */
    bool Read(const DBHandle& handle);

    /** Parse data read from DB into containers. */
    bool ParseData();


    bool       m_isInit;
    DBHandle   m_dbHandle;
    ItemsList  m_storeItems;
};

#endif // STORAGE_MANAGER_H_
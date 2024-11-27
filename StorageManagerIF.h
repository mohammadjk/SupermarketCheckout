#ifndef STORAGE_MANAGER_IF_H_
#define STORAGE_MANAGER_IF_H_

class StorageManagerIF
{
public:
    struct Config
    {
        std::string dbPath;
        /** Other configs */
    };

    virtual ~StorageManagerIF() = default;

    virtual bool Init(const StorageManagerIF::Config& cfg) = 0;
    virtual inline bool IsInit() const = 0;
    virtual inline const ItemsList& GetStoreItemsList() const = 0;
};

#endif // STORAGE_MANAGER_IF_H_
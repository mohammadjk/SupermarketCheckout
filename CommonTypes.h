#ifndef COMMON_TYPES_H_
#define COMMON_TYPES_H_

#include <string>
#include <vector>
#include <memory>
#include <map>

enum EventId
{
    eCheckoutStarted   = 0, // Checkout session started.
    eCheckoutCompleted,     // Checkout session completed.
    eUIClosed               // UI is closed.
};

struct Event
{
    EventId id;
};

enum DiscountType
{
    eNone = 0,          // No discount.
    eBuy3Get1Free,      // Buy three of the same item, pay for two.
    eBuy3CheapestFree   // Buy three eligible items, cheapest one is free.
};

struct Item
{
    unsigned int    uid;
    std::string     name;
    float           price;
    DiscountType    discountType;
};

struct DiscountItem
{
    unsigned int uid;
    std::string name;
    float discount;
};

using ItemsList = std::vector<Item>;

using BasketItem  = std::pair<Item, unsigned int>; // Pair of Item--Quantity
using BasketItems = std::map<unsigned int, BasketItem>; // Item uid --> Besket Item

using DiscountItems = std::vector<DiscountItem>;

#endif // COMMON_TYPES_H_
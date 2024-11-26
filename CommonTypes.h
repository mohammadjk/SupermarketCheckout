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

    bool operator<(const Item& other) const {return uid < other.uid;}
};

using ItemsList = std::vector<Item>;
using BasketItems = std::map<Item, unsigned int>; // Item--->Quantity
using DiscountItems = std::map<Item, float>; // Item--->Discount amount

#endif // COMMON_TYPES_H_
#include "BasketManager.h"

#include <numeric>
#include <algorithm>

BasketManager& BasketManager::GetInstance() {
    static BasketManager sys;
    return sys;
}

void BasketManager::Start()
{
}

void BasketManager::Stop()
{
}
void BasketManager::Reset()
{
    m_basketItems.clear();
}

void BasketManager::AddToBasket(Item item)
{
    // Check if the item already exists in the basket
    auto it = m_basketItems.find(item.uid);

    // If already exists, increment the quantity, otherwise add a new item with quantity set to 1.
    if (it != m_basketItems.end()) {
        ++it->second.second;
    } else {
        m_basketItems[item.uid] = std::make_pair(item, 1);
    }
}

float BasketManager::GetTotalPrice() const
{
    return std::accumulate(m_basketItems.cbegin(), m_basketItems.cend(), 0.0f, 
        [](float sum, const auto& pair){
            const BasketItem& item = pair.second;
             return sum+(item.first.price * item.second);
        });
}

float BasketManager::GetTotalDiscount() const
{
    float totalDiscount {};
    auto discountItems = GetDiscountItems();
    for (const auto& item : discountItems) {
        totalDiscount += item.discount;
    }
    return totalDiscount;
}

unsigned int BasketManager::GetTotalItemCount() const
{
    return std::accumulate(m_basketItems.cbegin(), m_basketItems.cend(), 0u, 
        [](unsigned int count, const auto& pair){
            const BasketItem& item = pair.second;
             return count+item.second;
        });
}

DiscountItems BasketManager::GetDiscountItems() const
{
    /** 
     * There are two types of discount:
     *      1) eBuy3Get1Free -  Buy three of the same item, pay for two.
     *      2) 2) eBuy3CheapestFree - Buy three eligible items, cheapest one is free.
     * Each one has to be calculated separately and then added to the discount list.
     **/ 
    
    DiscountItems  discountItems;

    // Store items that are  eligible for group discount
    std::vector<BasketItem> groupDiscountList;

    for (const auto& [uid, itemPair] : m_basketItems)
    {
        const Item& item = itemPair.first;
        auto quantity = itemPair.second;

        if (item.discountType == DiscountType::eNone) {
            continue;
        }

        if (item.discountType == DiscountType::eBuy3Get1Free && quantity >= 3) 
        {
            // For this discount type, for every 3 items one of them will be free.
            float discount = item.price*(quantity/3);
            discountItems.push_back({item.uid, item.name, discount});

        } 
        else if (item.discountType == DiscountType::eBuy3CheapestFree) 
        {
            // This discount type assumes that quantities of the same item contribute to discount eligibility.
            groupDiscountList.insert(groupDiscountList.end(), quantity, {item, 1});
        }
    }

    // For DiscountType::eBuy3CheapestFree discount type we need at least three eligible items.
    if (groupDiscountList.size() >= 3) 
    {
        // First sort the list in ascending order according to price.
        std::sort(groupDiscountList.begin(), groupDiscountList.end(), 
            [](const BasketItem& left, const BasketItem& right){
                return left.first.price < right.first.price;
        });

        // Get a Count of how many items should be discounted.
        unsigned int count = groupDiscountList.size() / 3;

        for (unsigned int i{}; i < count; ++i) {
            const Item& item = groupDiscountList[i].first;
            float discount = item.price;
            discountItems.push_back({item.uid, item.name, discount });
        }        
    }
    
    return discountItems;
}

void BasketManager::CalculateDiscount(DiscountType discountType) const
{

}
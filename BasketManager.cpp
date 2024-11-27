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
    auto it = m_basketItems.find(item);

    // If already exists, increment the quantity, otherwise add a new item with quantity set to 1.
    if (it != m_basketItems.end()) {
        ++it->second;
    } else {
        m_basketItems[item] = 1;
    }
}

float BasketManager::GetTotalPrice() const
{
    return std::accumulate(m_basketItems.cbegin(), m_basketItems.cend(), 0.0f, 
        [](float sum, const auto& pair){
            const auto& [item, quantity] = pair;
             return sum+(item.price * quantity);
        });
}

float BasketManager::GetTotalDiscount() const
{
    float totalDiscount {};
    auto discountItems = GetDiscountItems();
    std::for_each(discountItems.cbegin(), discountItems.cend(), 
        [&totalDiscount](const auto& pair){totalDiscount+=pair.second;});
    return totalDiscount;
}

unsigned int BasketManager::GetTotalItemCount() const
{
    return std::accumulate(m_basketItems.cbegin(), m_basketItems.cend(), 0u, 
        [](unsigned int count, const auto& pair){return count+pair.second;});
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
    ItemsList groupDiscountList;

    for (const auto& [item, quantity] : m_basketItems)
    {
        if (item.discountType == DiscountType::eNone) {
            continue;
        }

        if (item.discountType == DiscountType::eBuy3Get1Free && quantity >= 3) {
            // For this discount type, for every 3 items one of them will be free.
            discountItems[item] = item.price*(quantity/3);
        } 
        else if (item.discountType == DiscountType::eBuy3CheapestFree) {
            // This discount type assumes that quantities of the same item contribute to discount eligibility.
            groupDiscountList.insert(groupDiscountList.end(), quantity, item);
        }
    }

        // First sort the list in ascending order according to price.
    std::sort(groupDiscountList.begin(), groupDiscountList.end(), 
        [](const Item& left, const Item& right){return left.price < right.price;});

    // Get a count of how many items should be discounted.
    unsigned int count = groupDiscountList.size() / 3;

    std::for_each(groupDiscountList.cbegin(), groupDiscountList.cbegin()+count, 
        [&discountItems](const Item& item){discountItems[item] += item.price;});
    
    return discountItems;
}
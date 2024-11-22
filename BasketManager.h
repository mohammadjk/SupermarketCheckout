#ifndef BASKET_MANAGER_H_
#define BASKET_MANAGER_H_

#include "NonCopyable.h"
#include "CommonTypes.h"

class BasketManager : NonCopyable
{
public:
    static BasketManager& GetInstance();

    ~BasketManager() {}

    void Start();
    void Stop();

    void Reset();
    
    void AddToBasket(Item pItem);

    float GetTotalPrice() const;
    float GetTotalDiscount() const;
    unsigned int GetTotalItemCount() const;
   
    DiscountItems GetDiscountItems() const;
    const BasketItems& GetBasketItems() const {return m_basketItems;}

    void CalculateDiscount(DiscountType discountType) const;

private:
    BasketManager() = default;

    BasketItems     m_basketItems;
};

#endif // BASKET_MANAGER_H_
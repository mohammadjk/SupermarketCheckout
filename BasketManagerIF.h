#ifndef BASKET_MANAGER_IF_H_
#define BASKET_MANAGER_IF_H_

#include "CommonTypes.h"

class BasketManagerIF
{
public:
    virtual ~BasketManagerIF() = default;

    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Reset() = 0;
    
    virtual void AddToBasket(Item pItem) = 0;

    virtual float GetTotalPrice() const = 0;
    virtual float GetTotalDiscount() const = 0;
    virtual unsigned int GetTotalItemCount() const = 0;
    virtual DiscountItems GetDiscountItems() const = 0;
    virtual BasketItems GetBasketItems() const = 0;
};

#endif // BASKET_MANAGER_IF_H_
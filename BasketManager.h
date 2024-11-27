#ifndef BASKET_MANAGER_H_
#define BASKET_MANAGER_H_

#include "NonCopyable.h"
#include "BasketManagerIF.h"

class BasketManager : public BasketManagerIF, NonCopyable 
{
public:
    static BasketManager& GetInstance();

    ~BasketManager() override = default;

    void Start() override;
    void Stop() override;
    void Reset() override;
    
    void AddToBasket(Item pItem) override;

    float GetTotalPrice() const override;
    float GetTotalDiscount() const override;
    unsigned int GetTotalItemCount() const override;
    DiscountItems GetDiscountItems() const override;
    BasketItems GetBasketItems() const override {return m_basketItems;}

private:
    BasketManager() = default;

    BasketItems     m_basketItems;
};

#endif // BASKET_MANAGER_H_
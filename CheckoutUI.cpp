#include "CheckoutUI.h"

#include <iostream>
#include <iomanip>
#include <thread>
#include <algorithm>
#include <functional>

#include "StorageManager.h"
#include "BasketManager.h"


/** Helper functions */
namespace
{

const char* k_Divider {"--------------------------------------------------------------\n"};

/** Returns string representation of a discountType. */
const char* DiscountTypeToString(DiscountType discountType) 
{
    switch(discountType)
    {
        case DiscountType::eNone:               return "None";
        case DiscountType::eBuy3Get1Free:       return "Buy 3 Get One Free";
        case DiscountType::eBuy3CheapestFree:   return "Buy 3 Cheapest One Free";
        
        default: return "Unkown";
    };
}

/** Print store items. (i.e. Items available in the store) */
void PrintItems(const ItemsList& items)
{
    std::cout << "ID\tItem\tPrice(GBP)\tOffer\n"  
              << k_Divider;

    unsigned int index {};
    for (auto item : items) {
        std::cout << "[" << ++index << "]:";
        std::cout << std::fixed << std::setprecision(2) << std::setfill('0');
        std::cout << "\t" << item.name << "\t" << item.price << " GBP\t" << DiscountTypeToString(item.discountType) << "\n";
    }
    std::cout << k_Divider;
}

void PrintBasket(const BasketItems& basket) 
{
    std::cout << "Item\tUnit Price\tQuantity\tTotal Price\tOffer\n" 
              << k_Divider;

    for (const auto& [uid, itemPair] : basket) 
    {
        const Item& item = itemPair.first;
        unsigned int quantity = itemPair.second;
        float totalPrice = item.price * quantity;

        std::cout << std::fixed << std::setprecision(2) << std::setfill('0');
        std::cout << item.name << "\t" << item.price << " GBP\t" << quantity << "\t\t" << totalPrice << " GBP\t" << DiscountTypeToString(item.discountType) << "\n";
    }
    std::cout << k_Divider;
}

void PrintDiscount(const DiscountItems& discountItems)
{
    if (discountItems.empty()) {
        return;
    }

    std::cout << "Item\tDiscount\n" 
              << k_Divider;

    for (auto item : discountItems) {
        std::cout << std::fixed << std::setprecision(2) << std::setfill('0');
        std::cout << item.name << "\t-" << item.discount << " GBP\n";
    }
    std::cout << k_Divider;

}

void PrintTotal(float totalPrice, unsigned int totalCount, float totalDiscount)
{
    std::cout << "Total items:\t " << totalCount << "\n";
    std::cout << "Total Price:\t " << totalPrice << " GBP\n";
    
    if (totalDiscount != 0.0f) {
        std::cout << "Total Discount:\t " << totalDiscount << " GBP\n"; 
    }

    std::cout << "Sum Total:\t " << (totalPrice - totalDiscount) << " GBP\n"
              << k_Divider;
}

void PrintReceipt(const BasketItems& basketItems, const DiscountItems& discountItems, float totalPrice, unsigned int totalCount, float totalDiscount)
{
    auto now = std::chrono::system_clock::now();
    std::time_t currentDateTime = std::chrono::system_clock::to_time_t(now);
    std::cout << "Date:\t" << std::put_time(std::localtime(&currentDateTime), "%Y/%m/%d %H:%M:%S") << std::endl;
    
    PrintBasket(basketItems);
    PrintDiscount(discountItems);
    PrintTotal(totalPrice, totalCount, totalDiscount);
}

void ProcessPayment()
{
    /** Payment handling code through a payment processor object should go here. */

    std::cout << "Enter 0 to confirm payment\n";

    do
    {
        int choice {};
        std::cin >> choice;

        if (choice == 0)
            break;
        
        std::cout << "Invalid input.\n";
    } while (true);
}

} // anonymous namespace

bool CheckoutUI::Init(const CheckoutUI::Config& cfg)
{
    /** Config the UI as needed with cfg. */
    return (m_isInit = true);
}

bool CheckoutUI::Start()
{
    if (!m_isInit) {
        std::cerr << "UI not initialised\n";
        return false;
    }

    if (!m_pStorageManager) {
        std::cerr << "StorageManager not registered.\n";
        return false;
    }

    if (!m_pBasketManager) {
        std::cerr << "BasketManager not registered.\n";
        return false;
    }

    std::thread tUI(std::bind(&CheckoutUI::UIThreadFunc, this));
    tUI.detach();

    return true;
}

void CheckoutUI::Stop()
{
}

void CheckoutUI::UIThreadFunc()
{
    enum MenuChoice {
        eStart = 1,
        eExit
    };
    
    bool bFinished { false };

    // Get a list of all items available in the store. (view only)
    const ItemsList& storeItems = m_pStorageManager->GetStoreItemsList();

    while (!bFinished)
    {
        std::cout << "\n\nPlease select from one of the following options: \n";
        std::cout << "[1] Start\n";
        std::cout << "[2] Exit\n";
        
        int choice {};
        std::cin >> choice;

        switch(choice)
        {
            case MenuChoice::eStart: 
            {
                // Send an event: Checkout Started
                m_eventCallback(Event{EventId::eCheckoutStarted});
                
                std::cout << k_Divider;

                while (true)
                {
                    std::cout << "Please add items to your basket from the list below.\n";
                    std::cout << "\n---<STORE ITEMS>---\n\n";
                    PrintItems(storeItems);
                    std::cout << "[0]:\tCheckout\n";

                    int choice {};
                    std::cin >> choice;

                    if (choice == 0)
                        break;

                    if (choice > storeItems.size()) {
                        std::cout << "Invalid choice. Please select from the available choices.\n";
                        continue;
                    }

                    m_pBasketManager->AddToBasket(storeItems.at(choice-1));
                }

                const auto& basketItems = m_pBasketManager->GetBasketItems();
                if (basketItems.empty()) {
                    std::cout << "Basket is empty.\nNothing to checkout.\n";
                    break;
                }
                
                const float totalPrice = m_pBasketManager->GetTotalPrice();
                auto totalCount = m_pBasketManager->GetTotalItemCount();
                auto totalDiscount = m_pBasketManager->GetTotalDiscount();
                auto discountItems = m_pBasketManager->GetDiscountItems();

                std::cout << "Checking out. \n";

                std::cout << "\n---<BASKET ITEMS>---\n\n";
                PrintBasket(basketItems);
                
                std::cout << "\n---<DISCOUNTED ITEMS>---\n\n";
                PrintDiscount(discountItems);
                PrintTotal(totalPrice, totalCount, totalDiscount);

                ProcessPayment();

                std::cout << "payment successful.\n";
                std::cout << "Printing receipt:\n\n\n";

                PrintReceipt(basketItems, discountItems, totalPrice, totalCount, totalDiscount);

                // Send an event: Checkout Completed
                m_eventCallback(Event{EventId::eCheckoutCompleted});
            }
            break;
            
            case MenuChoice::eExit: {
                m_eventCallback(Event{EventId::eUIClosed});
                bFinished = true;
            } 
            break;
            
            default: {
                std::cout << "Invalid input! Please a valid option.\n";
            }
            break;
        };
    }
    std::cout << "Thank you for using our service. Goodbye!\n";
}
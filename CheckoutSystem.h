#ifndef CHECKOUT_SYSTEM_H_
#define CHECKOUT_SYSTEM_H_

class CheckoutSystem
{
public:
    bool Init();
    void Run();
    void Close();

private:
    bool InitStorageManager();
    bool InitUI();
};

#endif //CHECKOUT_SYSTEM_H_
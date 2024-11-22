#include "CheckoutSystem.h"

int main()
{
    CheckoutSystem mainApp;

    if (!mainApp.Init()) {
        return -1;
    }

    mainApp.Run();

    mainApp.Close();

    return 0;
}
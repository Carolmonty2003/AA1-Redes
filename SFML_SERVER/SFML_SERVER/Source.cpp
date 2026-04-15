#include <iostream>
#include "NetworkManager.h"

constexpr unsigned short SERVER_PORT = 55000;

int main()
{
    NetworkManager networkManager;

    if (!networkManager.Start(SERVER_PORT))
    {
        return -1;
    }

    while (true)
    {
        networkManager.Update();
    }

    return 0;
}
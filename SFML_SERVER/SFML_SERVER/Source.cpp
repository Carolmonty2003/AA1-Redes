#include <iostream>
#include "NetworkManager.h"
#include "DatabaseConnector.h"
constexpr unsigned short SERVER_PORT = 55000;

int main()
{
    NetworkManager networkManager;
    DatabaseConnector databaseConnector;
    databaseConnector.ConnectDatabase();
    //databaseConnector.GetAllPlayers();
    if (!networkManager.Start(SERVER_PORT))
    {
        return -1;
    }

    while (true)
    {
        networkManager.Update();
    }
    databaseConnector.DisconnectDatabase();
    return 0;
}
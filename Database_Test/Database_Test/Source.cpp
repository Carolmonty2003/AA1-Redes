#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>

#define SERVER "127.0.0.1:3306"
#define USERNAME "root"
#define PASSWORD "enti"

void main()
{
    sql::Driver* driver;
    sql::Connection* con;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(SERVER, USERNAME, PASSWORD);
        std::cout << "Connection done." << std::endl;

        con->close();
        std::cout << "Connection close." << std::endl;

    }
    catch (sql::SQLException e)
    {
        std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
        system("pause");
        exit(1);
    }

    delete con;
}
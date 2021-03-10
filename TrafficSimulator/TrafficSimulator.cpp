#include <iostream>

#include "View/WorkWindow.h";
#include "Control/Logger.h";

/**
 * @brief 
 * @return 
*/
int main(int argc, char* args[])
{
    Logger::start();

    WorkWindow workWindow;
    int windowState = workWindow.open();

    return 0;
}
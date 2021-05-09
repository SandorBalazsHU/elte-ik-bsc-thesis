#include <iostream>

#include "View/WorkWindow.h"
#include "Control/Logger.h"

int main(int argc, char* args[])
{
    Logger::startFileLogging();
    Logger::log("Program started!");
    int state = 0;

    WorkWindow workWindow;
    state = workWindow.open();

    if (state != 0) {
        Logger::error("Application start error", state);
        Logger::stopFileLogging();
        return state;
    }

    Logger::log("Program terminated correctly!");
    Logger::stopFileLogging();
    return state;
}
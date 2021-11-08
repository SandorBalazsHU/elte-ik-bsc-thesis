/**
 * @name Traffic Simulation
 * @file Camera.cpp
 * @class TrafficSimulator
 * @author Sándor Balázs - AZA6NL
 * @date 2021.11.08.
 * @brief Main file.
 * Contact: sandorbalazs9402@gmail.com
*/
#include <iostream>

#include "View/WorkWindow.h"
#include "Control/Logger.h"

/**
 * @brief Program main function.
 * @param argc Consol Argument count.
 * @param args Cosol arguments array.
 * @return program exit status.
*/
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
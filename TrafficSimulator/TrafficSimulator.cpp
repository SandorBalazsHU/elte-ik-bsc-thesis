/**
 * @name Traffic Simulation
 * @file TrafficSimulator.cpp
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
 * Possible error codes:
 * 0 - Everything is fine.
 * 1 - SDL Initialization error.
 * 2 - OpenGL 3D Window creation error.
 * 3 - imGUI GUI system initialization error.
 * 4 - OpenGL Context creation error.
 * 5 - GLEW system initialization error.
 * 6 - OpenGL starting or version error.
 * 7 - Shader compiling or starting error.
 * @return The starting process status number. 0 is OK Other ID-s mean errors.
 * @param argc Console Argument count.
 * @param args Console arguments array.
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
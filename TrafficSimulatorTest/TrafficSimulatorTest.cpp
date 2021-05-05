/**
 * @name Traffic Simulation.
 * @file TrafficSimulatorTest.cpp
 * @class TrafficSimulatorTest
 * @author Sándor Balázs - AZA6NL
 * @date 2021.05.01.
 * @brief GTest main file for TrafficSimulator project testing.
 * Contact sandorbalazs9402@gmail.com
*/

#include <gtest/gtest.h>
#include "pch.h"

/**
 * @brief TraficSimulatorTest GTest main function. Run all test.
 * @param argc Command line parameter's number.
 * @param args Command line parameters.
 * @return The program return int.
*/
int main(int argc, char* args[]) {
    ::testing::InitGoogleTest(&argc, args);
    RUN_ALL_TESTS();
    return 0;
}
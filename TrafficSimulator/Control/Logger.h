#pragma once

#include <iostream>

class Logger {
public:
	static void start();
	static std::string currentDateTime();
};
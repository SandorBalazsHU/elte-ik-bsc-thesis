#pragma once
#include "WorkWindow.h"

class Render {
public:
	Render(WorkWindow*);
	~Render(void);
	int render();
private:
	WorkWindow* window;
};
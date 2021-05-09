#pragma once
#include "WorkWindow.h"
#include "Camera.h"
#include "Utilities/ProgramObject.h"

class Render {
public:
	Render(void);
	~Render(void);
	void render();
	void bind(WorkWindow* window);
	void faceTestOn();
	void faceTestOff();
	void shaderUpdate();

private:
	WorkWindow* window;
	Camera* camera;
	ProgramObject* shader;
};
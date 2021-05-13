#pragma once
#include "WorkWindow.h"
#include "Camera.h"
#include "Utilities/ProgramObject.h"

class Render {
public:
	Render(void);
	~Render(void);
	void render();
	void setShader(glm::mat4 worldMatrix, glm::mat4 color);
	void bind(WorkWindow* window);
	void faceTestOn();
	void faceTestOff();
	void shaderCameraUpdate();
	void clearScrean();
	void setTexture(Texture2D texture);

private:
	WorkWindow* window;
	Camera* camera;
	ProgramObject* shader;
};
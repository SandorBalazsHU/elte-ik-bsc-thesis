#pragma once
#include "WorkWindow.h"
#include "Utilities/VertexArrayObject.h"
#include "Camera.h"
#include "Utilities/ProgramObject.h"

class Render {
public:
	Render(void);
	~Render(void);
	void bind(WorkWindow* window);
	void faceTestOn();
	void faceTestOff();
	void shaderCameraUpdate();
	void clearScrean();
	void setTexture(Texture2D texture);
	void render();
	void shaderPreDrawingUpdate(glm::mat4 worldMatrix, glm::mat4 color);
	void drawMesh(std::unique_ptr<Mesh> mesh);
	void drawVao(VertexArrayObject& vao, int trianglesNumber);

private:
	WorkWindow* window;
	Camera* camera;
	ProgramObject* shader;
};
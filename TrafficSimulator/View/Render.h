#pragma once
class WorkWindow;

#include "Camera.h"
#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"
#include "Utilities/VertexArrayObject.h"

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
	WorkWindow* workingWindow;
	SDL_Window* window;
	Camera* camera;
	ProgramObject* shader;
};
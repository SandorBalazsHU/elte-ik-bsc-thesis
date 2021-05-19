#pragma once
class WorkWindow;
class GUI;

#include "Camera.h"
#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"
#include "Utilities/VertexArrayObject.h"
#include "../Control/object3DLoader.h"

class Render {
public:
	Render(void);
	~Render(void);
	void bind(WorkWindow* window);
	void faceTestOn();
	void faceTestOff();
	void shaderCameraUpdate();
	void clearScrean();
	void setTexture(Texture2D& texture);
	void render();
	void shaderPreDrawingUpdate(glm::mat4 worldMatrix, glm::vec4 color);
	void drawMesh(std::unique_ptr<Mesh>& mesh);
	void drawVao(VertexArrayObject& vao, int trianglesNumber);
	void rendering();
	void setWindowTitle(std::string title);

	//test
	//Texture2D				texture;
	object3Dloader			loader;
	std::unique_ptr<Mesh>	ball;
	void test();

private:
	WorkWindow* workingWindow;
	SDL_Window* window;
	Camera* camera;
	ProgramObject* shader;
	GUI* gui;
};
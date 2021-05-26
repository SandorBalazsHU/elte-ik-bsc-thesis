#pragma once
class WorkWindow;
class GUI;

#include "Camera.h"
#include "Utilities/ProgramObject.h"
#include "Utilities/TextureObject.h"
#include "Utilities/Mesh_OGL3.h"
#include "Utilities/VertexArrayObject.h"
#include "Objects/ObjectStorage.h"

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
	void vsyncOn();
	void vsyncOff();
	void multisamplingOn();
	void multisamplingOff();
	void wireframeOn();
	void wireframeOff();
	void setWindowIcon(SDL_Surface* windowIcon);
	void addObject(int id);

private:
	WorkWindow* workingWindow;
	SDL_Window* window;
	Camera* camera;
	ProgramObject* shader;
	GUI* gui;
	ObjectStorage* objectStorage;
	std::vector<Object3D> renderableObjects;

	//test
	void test();
	bool onece = true;

	void renderScrean();
};
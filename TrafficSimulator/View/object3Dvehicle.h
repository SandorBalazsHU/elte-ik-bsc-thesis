#pragma once
#include "Object3D.h"

class object3Dvehicle: public Object3D {
public:
	void setLightTexture(int);
	void setBreakTexture(int);
	void setRightTexture(int);
	void setLeftTexture (int);

	Texture2D getLightTexture();
	Texture2D getBreakTexture();
	Texture2D getRightTexture();
	Texture2D getLeftTexture ();
protected:
	int	baseTexture;
	int lightTexture;
	int breakTexture;
	int rightTexture;
	int leftTexture;
};
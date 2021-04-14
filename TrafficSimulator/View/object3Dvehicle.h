#pragma once
#include "object3D.h"

class object3Dvehicle: public object3D {
public:
	void setLightTexture(Texture2D);
	void setBreakTexture(Texture2D);
	void setRightTexture(Texture2D);
	void setLeftTexture (Texture2D);

	Texture2D getLightTexture();
	Texture2D getBreakTexture();
	Texture2D getRightTexture();
	Texture2D getLeftTexture ();
protected:
	Texture2D	baseTexture;
	Texture2D	lightTexture;
	Texture2D	breakTexture;
	Texture2D	rightTexture;
	Texture2D	leftTexture;
};
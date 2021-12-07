#version 460

//Pipeline inputs
in vec3 vertexInPosition;
in vec3 vertexInNormal;
in vec2 vertexInTexture;

//Pipeline outputs
out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 fragmentTexture;


//Shader parameters
uniform mat4 worldMatrix;
uniform mat4 worldInverseTransposeMatrix;
uniform mat4 projectionViewWorldMatrix;

void main() {
	gl_Position			= projectionViewWorldMatrix * vec4(vertexInPosition, 1);
	fragmentPosition	= (worldMatrix * vec4(vertexInPosition, 1)).xyz;
	fragmentNormal		= (worldInverseTransposeMatrix * vec4(vertexInNormal, 0)).xyz;
	fragmentTexture		= vertexInTexture;
}
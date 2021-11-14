#version 460
// VBO-ból érkezõ változók
in vec3 vertexInPosition;
in vec3 vertexInNormal;
in vec2 vertexInTexture;

//A pipeline-ban tovább adandó értékek
out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec2 fragmentTexture;


//shader külsõ paraméterei
uniform mat4 worldMatrix;
uniform mat4 worldInverseTransposeMatrix;
uniform mat4 projectionViewWorldMatrix;

void main() {
	gl_Position = projectionViewWorldMatrix * vec4(vertexInPosition, 1);

	fragmentPosition = (worldMatrix * vec4(vertexInPosition, 1)).xyz;
	fragmentNormal  = (worldInverseTransposeMatrix * vec4(vertexInNormal, 0)).xyz;
	fragmentTexture = vertexInTexture;
}
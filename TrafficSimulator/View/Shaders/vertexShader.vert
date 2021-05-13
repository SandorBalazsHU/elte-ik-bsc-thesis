// VBO-ból érkezõ változók
in vec3 vertexInPosition;
in vec3 vertexInNormal;
in vec2 vertexInTexture;

//A pipeline-ban tovább adandó értékek
out vec3 vertexOutPosition;
out vec3 vertexOutNormal;
out vec2 vertexOutTexture;


//shader külsõ paraméterei
uniform mat4 worldMatrix;
uniform mat4 worldInverseTransposeMatrix;
uniform mat4 projectionViewMatrix;

void main() {
	gl_Position = projectionViewMatrix * vec4(vertexInPosition, 1);

	vertexOutPosition = (worldMatrix * vec4(vertexInPosition, 1)).xyz;
	vertexOutNormal  = (worldInverseTransposeMatrix * vec4(vertexInNormal, 0)).xyz;
	vertexOutTexture = vertexInTexture;
}
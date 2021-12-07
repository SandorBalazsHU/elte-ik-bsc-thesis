#version 460

//Pipeline inputs
in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 fragmentTexture;

//Pipeline output
out vec4 fragmentColor;

//Shader parameters
	//Positions
	uniform vec3 cameraPosition = vec3( 0.0f, 15.0f, 15.0f );
	uniform vec3 lightPosition = vec3( 55.0f, 55.0f, 55.0f );

	//Lights
	uniform vec4 ambientLight  = vec4(0.1f,  0.1f, 0.1f, 1.0f);
	uniform vec4 diffuseLight  = vec4(0.75f, 0.75f, 0.75f, 1.0f);
	uniform vec4 specularLight = vec4(1.0f,  1.0f, 1.0f, 1.0f);
	uniform float specularPower = 32.0f;

	//Materials
	uniform vec4 ambientMaterial  = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	uniform vec4 diffuseMaterial  = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	uniform vec4 specularMaterial = vec4(1.0f, 1.0f, 1.0f, 0.0f);

	//Texture
	uniform sampler2D currentTexture;

//Shader calculator
void main() {
	// Ambient light calculation
	vec4 ambientColor = ambientLight * ambientMaterial;

	// Diffuse light calculation
	vec3  fragmentNormalVector = normalize(fragmentNormal);
	vec3  lightDirection       = normalize(lightPosition - fragmentPosition);
	float diffuseDirection     = clamp(dot(lightDirection, fragmentNormalVector), 0.0f, 1.0f);
	vec4  diffuseColor         = vec4(diffuseLight.rgb * diffuseMaterial.rgb * diffuseDirection, diffuseMaterial.a);

	//Spotlight calculation
	vec4 specularColor = vec4(0.0f);
	if ( diffuseDirection > 0.0f ) {
		vec3 viewDirection      = normalize(cameraPosition - fragmentPosition);
		vec3 reflectDirection   = reflect(-lightDirection, fragmentNormalVector);
		float specularComponent = pow(clamp(dot(viewDirection, reflectDirection), 0.0f, 1.0f ), specularPower);
		specularColor = specularLight * specularMaterial * specularComponent;
	}
	fragmentColor = (ambientColor + diffuseColor + specularColor ) * texture(currentTexture, fragmentTexture.st);
}
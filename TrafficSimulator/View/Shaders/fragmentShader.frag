in vec3 fragmentPosition;
in vec3 fragmentNormal;
in vec2 fragmentTexture;

out vec4 fragmentColor;

uniform vec3 cameraPosition = vec3(0, 15, 15);
uniform vec3 lightPosition = vec3( 55, 55, 55 );

uniform vec4 ambientLight = vec4(0.1f, 0.1f, 0.1f, 1);
uniform vec4 diffuseLight = vec4(0.75f, 0.75f, 0.75f, 1);
uniform vec4 specularLight = vec4(1, 1, 1, 1);

uniform vec4 ambientMaterial = vec4(1, 1, 1, 0);
uniform vec4 diffuseMaterial = vec4(1, 1, 1, 1);
uniform vec4 specularMaterial = vec4(1, 1, 1, 0);
uniform float specularPower = 32;
uniform sampler2D currentTexture;

void main() {
	// ambiens szín számítása
	vec4 ambientColor = ambientLight * ambientMaterial;

	// diffúz szín számítása
	vec3 fragmentNormalVector = normalize(fragmentNormal);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diffuseDirection = clamp(dot(lightDirection, fragmentNormalVector), 0.0f, 1.0f);
	vec4 diffuseColor = vec4(diffuseLight.rgb * diffuseMaterial.rgb * diffuseDirection, diffuseMaterial.a);

	// fényfoltképzõ szín
	vec4 specularColor = vec4(0);
	if ( diffuseDirection > 0 ) {
		vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
		vec3 reflectDirection = reflect(-lightDirection, fragmentNormalVector);
		float specularComponent = pow(clamp(dot(viewDirection, reflectDirection), 0.0f, 1.0f ), specularPower);
		specularColor = specularLight * specularMaterial * specularComponent;
	}

	fragmentColor = (ambientColor + diffuseColor + specularColor ) * texture(currentTexture, fragmentTexture.st);
}
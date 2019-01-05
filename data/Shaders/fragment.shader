#version 460 core

// Interpolated values from the vertex shaders
in vec3 positionW;
in vec3 normalC;
in vec3 eyeDirectionC;
in vec3 lightDirectionC;
in vec2 uv;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform mat4 MV;
uniform vec3 lightPositionW;
uniform float lightPower;
uniform vec3 lightColor;
uniform vec3 materialDiffuseColor;
uniform vec3 materialSpecularColor;
uniform vec3 materialAmbientColor;
uniform sampler2D textureSampler;
uniform sampler2D alphaSampler;

void main()
{
	vec3 texColor;
	texColor = materialDiffuseColor;
	//if (textureSampler != NULL)
	//{
		texColor = texture(textureSampler, uv).rgb;
	//}

	float distance = length( lightPositionW - positionW );
	
	vec3 n = normalize( normalC );
	vec3 l = normalize( lightDirectionC );
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	vec3 E = normalize(eyeDirectionC);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = vec4(//texColor;
		materialAmbientColor * texColor +
		texColor * lightColor * lightPower * cosTheta / (distance*distance) +
		materialSpecularColor * lightColor * lightPower * pow(cosAlpha,5) / (distance*distance), 1);

}
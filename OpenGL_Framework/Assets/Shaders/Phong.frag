#version 420

uniform mat4 CameraPosition;
uniform vec3 LightAmbient;

// scalars
uniform float LightSpecularExponent;

uniform sampler2D uTex;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

struct LightSource
{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float Attenuation_Constant, Attenuation_Linear, Attenuation_Quadratic;
};

const int numberOfLights = 5;


LightSource light0 = LightSource(
	vec4(0.0, 3.0, 0.0, 1.0),
	vec4(0.7, 0.4, 0.7, 1.0),
	vec4(0.8, 0.5, 0.8, 1.0),
	1.0, 0.1, 0.01
);

LightSource light1 = LightSource(
	vec4(6.0, 3.0, -6.0, 1.0),
	vec4(0.7, 0.4, 0.4, 1.0),
	vec4(0.8, 0.5, 0.5, 1.0),
	1.0, 0.1, 0.01
);

LightSource light2 = LightSource(
	vec4(6.0, 3.0, 6.0, 1.0),
	vec4(0.4, 0.7, 0.4, 1.0),
	vec4(0.5, 0.8, 0.5, 1.0),
	1.0, 0.1, 0.01
);

LightSource light3 = LightSource(
	vec4(-6.0, 3.0, 6.0, 1.0),
	vec4(0.4, 0.4, 0.7, 1.0),
	vec4(0.5, 0.5, 0.8, 1.0),
	1.0, 0.1, 0.01
);

LightSource light4 = LightSource(
	vec4(-6.0, 3.0, -6.0, 1.0),
	vec4(0.4, 0.7, 0.7, 1.0),
	vec4(0.5, 0.8, 0.8, 1.0),
	1.0, 0.1, 0.01
);

void main()
{
	LightSource lights[numberOfLights];

	lights[0] = light0;
	lights[1] = light1;
	lights[2] = light2;
	lights[3] = light3;
	lights[4] = light4;


	// account for rasterizer interpolating
	vec3 normal = normalize(norm);

	outColor.rgb = LightAmbient;

	vec4 lightPosition;
	vec3 lightVec;
	float dist;
	vec3 lightDir;

	// Compute lighting for all light sources
	for(int i = 0; i < numberOfLights; i++)
	{
		lightPosition = lights[i].position;
		lightVec = (CameraPosition * lightPosition).xyz - pos;
		dist = length(lightVec);
		lightDir = lightVec / dist;

		float NdotL = dot(normal, lightDir);
		if (NdotL > 0.0)
		{
			float attenuation = 1.0 / (lights[i].Attenuation_Constant 
									+ (lights[i].Attenuation_Linear * dist) 
									+ (lights[i].Attenuation_Quadratic * dist * dist));

			// Calculate specular contribution
			outColor.rgb += lights[i].diffuse.rgb * NdotL * attenuation;

			// Blinn-Phong half vector
			float NdotHV = max(dot(normal, normalize(lightDir + normalize(-pos))), 0.0);

			// Calculate diffuse contribution
			outColor.rgb += lights[i].specular.rgb * pow(NdotHV, LightSpecularExponent) * attenuation;		
		}

	}

	vec4 textureColor = texture(uTex, texcoord);

	outColor.rgb *= textureColor.rgb;
	outColor.a = textureColor.a;
	
}
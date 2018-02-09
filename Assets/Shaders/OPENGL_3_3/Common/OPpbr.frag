#version 330 core

out vec4 FragColor;

in vec2 vUV;
in vec3 vWorldPos;
in vec3 vNormal;

// material parameters
uniform sampler2D uAlbedoMap;
uniform sampler2D uNormalMap;
uniform sampler2D uMetallicMap;
uniform sampler2D uRoughnessMap;
uniform sampler2D uAOMap;

// lights
uniform vec3 uLightPositions[4];
uniform vec3 uLightColors[4];

uniform vec3 uCamPos;

const float PI = 3.14159265359;

// ----------------------------------------------------------------------------
// Easy trick to get tangent-vNormals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do vNormal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the vNormal mapping tutorial.
vec3 getvNormalFromMap()
{
    vec3 tangentvNormal = texture(uNormalMap, vUV).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(vWorldPos);
    vec3 Q2  = dFdy(vWorldPos);
    vec2 st1 = dFdx(vUV);
    vec2 st2 = dFdy(vUV);

    vec3 N   = normalize(vNormal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentvNormal);
}


float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    // material properties
    vec3 uAlbedo = pow(texture(uAlbedoMap, vUV).rgb, vec3(2.2));
    float uMetallic = texture(uMetallicMap, vUV).r;
    float uRoughness = texture(uRoughnessMap, vUV).r;
    float uAmbientOcclusion = texture(uAOMap, vUV).r;
    // input lighting data
    vec3 N = getvNormalFromMap();

	// vec3 N = normalize(vNormal); // normal vector
	vec3 V = normalize(uCamPos - vWorldPos); // view vector

	// TODO: (garrett) better descibe what F0 actually is
	vec3 F0 = vec3(0.4); // Metallic
	// linearly inerpolate between F0 and uAlbedo
	// by the amount of metallic, so fully metalic
	// will only show the albedo
	F0 = mix(F0, uAlbedo, uMetallic);


	// Calculate Reflectance equation per light
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i) 
    {
		// per light radiance
		vec3 L = normalize(uLightPositions[i] - vWorldPos); // light vector
		vec3 H = normalize(V + L); // halfway vector

		float distance = length(uLightPositions[i] - vWorldPos);
		float attenuation = 1.0 / ( distance * distance );
		vec3 radiance = uLightColors[i] * attenuation;

		// brdf ( cook-torrance)
		float NDF = DistributionGGX(N, H, uRoughness);
		float G = GeometrySmith(N, V, L, uRoughness);
		vec3 F = fresnelSchlickRoughness(max(dot(H, V), 0.0), F0, uRoughness);

		vec3 kS = F; // energy of light that gets reflected
		vec3 kD = vec3(1.0) - kS; // light that gets refracted
		kD *= 1.0 - uMetallic;
		
		float NdotV = max(dot(N,V), 0.0);
		float NdotL = max(dot(N,L), 0.0);

		vec3 nominator = NDF * G * F;
		float denominator = 4.0 * NdotV * NdotL;
		vec3 specular = nominator / max(denominator, 0.001);

		// outgoing radiance
		Lo += (kD * uAlbedo / PI + specular) * radiance * NdotL;
	}

	vec3 baseAmbient = vec3(0.3);
	vec3 ambient = baseAmbient * uAlbedo * uAmbientOcclusion;
	vec3 color = ambient + Lo;

	// gamma correction
	float gamma = 2.2;
	color = color / ( color + vec3(1.0));
	color = pow(color, vec3(1.0/gamma));

	FragColor = vec4(color, 1.0);
}
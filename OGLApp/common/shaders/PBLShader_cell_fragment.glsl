
#version 330 core

uniform vec4 _Color;
uniform vec3 _SpecularColor;
uniform vec3 _AmbientColor;

uniform vec3 _LightColor;
uniform vec3 _lightDirection;
uniform float _LightRadius;

uniform mat4 _projectionMatrix;
uniform mat4 _viewMatrix;

uniform float _Roughness;
uniform float _Metallic;

uniform sampler2D _texture;
uniform sampler2D _normalTexture;
uniform sampler2D _roughnessTexture;
uniform sampler2D _metallicTexture;
uniform sampler2D _shadowTexture;

in vec3 normalDirection;
in vec4 vertPosition;
in vec4 eyePosition;
in vec3 tangentDirection;
in vec3 bitangentDirection;
in vec4 lightPosition;
in vec4 positionLightSpace;
in vec2 uv;

const float screenGamma = 2.2;

out vec4 out_Color;

// Specular Distribution Algoritms
float TrowbridgeReitzNormalDistribution(float roughness, float NdotH) {
  float roughnessSqr = roughness * roughness;
  float distribution = NdotH * NdotH * (roughnessSqr - 1.0) + 1.0;
  return roughnessSqr / (3.1415926535 * distribution * distribution);
}

// Geometric Shadowing Algorithms
// float KurtGeometricShadowingFunction (float roughness, float NdotL, float NdotV, float VdotH) {
// 	return NdotL * NdotV / (VdotH * pow( NdotL * NdotV, roughness ));
// }

float ModifiedKelemenGeometricShadowingFunction (float roughness, float NdotV, float NdotL) {
	float c = 0.797884560802865; // sqrt(2 / Pi)
	float k = roughness * roughness * c;
	float gH = NdotV  * k + (1 - k);
	return (gH * gH * NdotL);
}

// Fresnel Algorith
float MixFunction(float i, float j, float x) {
	 return  j * x + i * (1.0 - x);
}

float SchlickFresnel(float i){
    float x = clamp(1.0 - i, 0.0, 1.0);
    float x2 = x * x;
    return x2 * x2 * x;
}

float SchlickFresnelFuntion (float roughness, float NdotL, float NdotV, float LdotH) {
    float FresnelLight = SchlickFresnel(NdotL); 
    float FresnelView = SchlickFresnel(NdotV);
    float FresnelDiffuse90 = 0.5 + 2.0 * LdotH * LdotH * roughness;
    return mix(1, FresnelDiffuse90, FresnelLight) * mix(1, FresnelDiffuse90, FresnelView);
}

float ShadowCalculation(vec4 positionLS, float bias) {
  // perform perspective divide
  // This returns the fragment's light-space position in the range [-1,1].
  vec3 projCoords = positionLS.xyz / positionLS.w;
  // Now [0, 1]
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(_shadowTexture, projCoords.xy).r;
  float currentDepth = projCoords.z - bias;
   // check whether current frag pos is in shadow
  float shadow = step(currentDepth, closestDepth);
  
  return shadow;
}

void main() {
  // Normal normalized
  vec3 textureNormal = texture(_normalTexture, uv).rgb * 2 - 1;
  vec3 normal = normalize(normalDirection);
  normal = (tangentDirection * textureNormal.r)
         + (bitangentDirection * textureNormal.g)
         + (normal * textureNormal.b);
  normal = normalize(normal);
  vec3 normalColor = (_viewMatrix * vec4(normal, 1)).rgb;
  normalColor = (1 - normalColor) * 0.5;
  
  vec3 lightDirection = (lightPosition - vertPosition).xyz;
  // vec3 lightDirection = normalize(mix(lightPosition, lightPosition - vertPosition, lightPosition.w)).xyz;

  float distance = length(lightDirection);
  lightDirection = normalize(lightDirection);

  vec3 eyeDirection = normalize(eyePosition.xyz - vertPosition.xyz);

  // vec3 lightReflectDirection = reflect( -lightDirection.xyz, normal );

  // vec3 eyeReflectDirection = normalize(reflect( -eyeDirection, normal ));
  vec3 halfDirection = normalize(eyeDirection + lightDirection); 

  float roughness = _Roughness * texture(_roughnessTexture, uv).r;
  float metalness = (_Metallic * texture(_metallicTexture, uv).r);

  roughness = roughness * roughness;
  
  vec3 diffuseColor = _Color.rgb * (1 - metalness);
  diffuseColor *= texture(_texture, uv).rgb;
  // diffuseColor = diffuseColor * normal;
  vec3 specularColor = mix(vec3(1), _SpecularColor, metalness);

  // Surface operations
  float NdotL = max(0.0, dot(                normal, lightDirection ));
  float NdotH = max(0.0, dot(                normal, halfDirection  ));
  float NdotV = max(0.0, dot(                normal, eyeDirection   ));
  // float VdotH = max(0.0, dot(          eyeDirection, halfDirection  ));
  float LdotH = max(0.0, dot(        lightDirection, halfDirection  ));
  // float LdotV = max(0.0, dot(        lightDirection, eyeDirection   )); 
  // float RdotV = max(0.0, dot( lightReflectDirection, eyeDirection   ));

  float attenuation = clamp(_LightRadius / (distance * distance), 0, 1);
  attenuation *= ShadowCalculation(positionLightSpace, 0.0001);
  attenuation *= (-dot(_lightDirection * 2, lightDirection) - 1.4) * 6;
  attenuation = clamp(attenuation, 0, 1);
  vec3 difusse = NdotL * _LightColor * attenuation;
  difusse *= step(0.5, length(difusse));

  vec3 SpecularDistribution = specularColor * attenuation;
  SpecularDistribution *= max(TrowbridgeReitzNormalDistribution(roughness, NdotH), 0.0);

  float GeometricShadow = max(ModifiedKelemenGeometricShadowingFunction(roughness, NdotV, NdotL), 0.0);

  float Fresnel = max(SchlickFresnelFuntion(roughness, NdotL, NdotV, LdotH), 0.0);

  vec3 specularity = (SpecularDistribution * Fresnel * GeometricShadow) / (4 * ( NdotL * NdotV ));
  specularity *= step(0.5, length(specularity));

  out_Color = vec4(difusse * diffuseColor + _AmbientColor * diffuseColor, 1);
  out_Color += vec4(max( specularity * NdotV, 0 ), 0); // * NdotL, 0);
  // out_Color = vec4(1) * 1 - smoothstep(0, 12, length(vertPosition - eyePosition));
  // out_Color = vec4(normalColor + max(specularity * NdotV, 0), 1);
  // out_Color = vec4((1 - tangentDirection) * 0.5, 1);
}
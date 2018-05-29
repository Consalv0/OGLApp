
#version 330 core

uniform vec3 _baseColor;
uniform vec3 _lightColor;
uniform vec3 _ambientColor;
uniform vec3 _cameraPosition;
uniform sampler2D _texture;

in vec3 normalInterp;
in vec3 vertPosition;
in vec3 eyeDirection;
in vec3 lightDirection;
in vec2 UV;

const float shininess = 0.1;
const float screenGamma = 2.2;

out vec4 out_Color;

void main() {
  vec3 normal = normalize(normalInterp);
  vec3 normalColor = (normal + 1) * 0.5;
  
  float distance = length(lightDirection);

  float lambertian = max(dot(lightDirection, normal), 0.0);
  float specular = 0.0;

	// Blinn - Phong
	// vec3 halfDir = normalize(lightDir + viewDir);
	// float specIntensity = max(dot(normal, halfDir), 0.0);
	// specular = pow(specIntensity, shininess / 4.0);

	vec3 viewDir = normalize(-vertPosition);
	vec3 halfDir = normalize(lightDirection + normalize( eyeDirection ));
  float specIntensity = max( dot( halfDir, normal ), 0.0 );
	specular = pow(specIntensity, shininess * 1000.0) * lambertian;
  
  vec3 colorLineal = _baseColor * _ambientColor +
                     _baseColor * lambertian * _lightColor * length(_lightColor) / (distance * distance); // +
					           // vec3(1) * specular * _lightColor * length(_lightColor) / (distance * distance);

  vec3 colorGammaCorrected = pow(colorLineal, vec3( 1.0 / screenGamma ));

  out_Color = vec4(colorLineal, 1.0);

  /////////
  // out_Color = _colorBase * (diffuse + specular);

  /////////
  // vec3 normalColor = (normalDir + 1) * 0.5;
  // float light = dot(inormalDir, normalize(lightDir));
  // light = (( light - 0.5 ) * 2);
  // 
  // float dirLight = dot(normalDir, normalize(lightDir));
  // dirLight = (( dirLight - 0.5 ) * 2);
  // 
  // float vlight = dot(inormalDir, normalize(viewDir));
  // vlight = ( 1 - (( vlight - 0.65 ) * 10) );
  // 
  // // color = vec3(1 - colorBase) * 0.2;
  // // color = color * (-light + 0.3);
  // // color = color + ( vlight * light * 2) * normalize(colorBase);
  // color = vec3(1) * dirLight;
  // // color = color + max(light + 0.5, 0);
}
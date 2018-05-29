
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_UV;

uniform mat4 _projectionMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _modelMatrix;
uniform mat4 _normalMatrix;
uniform vec3 _lightPosition;

out vec3 normalInterp;
out vec3 vertPosition;
out vec3 eyeDirection;
out vec3 lightDirection;
out vec2 UV;

void main() {
  // Output position of the vertex, in clip space : MVP * position
  gl_Position =  _projectionMatrix * _viewMatrix * _modelMatrix * vec4(in_Position, 1);
  
  // Position of the vertex, in worldspace : M * position
  vertPosition = (_modelMatrix * vec4( in_Position, 1 )).xyz;

  // Vector that goes from the vertex to the camera, in camera space.
  // In camera space, the camera is at the origin (0,0,0).
  vec3 position_cameraspace = ( _viewMatrix * _modelMatrix * vec4( in_Position, 1 )).xyz;
  eyeDirection = vec3(0,0,0) - position_cameraspace;

  // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
  vec3 lightPosition_cameraspace = ( _viewMatrix * vec4(_lightPosition,1)).xyz;
  lightDirection = lightPosition_cameraspace + eyeDirection;

  // Normal of the the vertex, in camera space
  normalInterp = vec3(_normalMatrix * vec4(in_NormalFlat, 0.0)); 

/////
  // gl_Position = _projectionMatrix * _modelViewMatrix * vec4(in_Position, 1.0);
  // vec4 vertPos4 = _modelViewMatrix * vec4(in_Position, 1.0);
  // vertPosition = vec3(vertPos4) / vertPos4.w;
  // normalInterp = vec3(_normalMatrix * vec4(in_NormalFlat, 0.0));

//////
  // mat4 MVP = _projectionMatrix * _viewMatrix;
  // gl_Position = MVP * vec4(in_Position, 1.0);
  // 
  // vec3 vertView = (_projectionMatrix * vec4(in_Position, 1)).xyz;
  // 
  // out_Ambient = vec3(0.5);
  // vec3 lightDirection = normalize(_lightPosition - vertView);
  // out_Diffuse = 0.7 * _lightColor * max(dot(in_NormalFlat, lightDirection), 0);
  // 
  // vec3 eyeView = normalize(_cameraPosition - vertView);
  // vec3 halfWay = normalize(lightDirection + eyeView);
  // out_Specular = 0.3 * _lightColor * pow(max(dot(in_NormalFlat, halfWay), 0), 0.5);

///////
  // gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
  // vec4 normal_view = MVP * vec4(vertexNormal, 1);
  // vec4 inormal_view = MVP * vec4(ivertexNormal, 1);
  // 
  // normalDir = normalize(normal_view).xyz;
  // inormalDir = normalize(inormal_view).xyz;
  // viewDir = normalize(gl_Position - MVP * vec4(cameraPosition, 1)).xyz;
  // viewDir = normalize(vertexPosition_modelspace - cameraPosition);
}
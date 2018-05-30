
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoords;
layout(location = 3) in vec3 in_Tangent;

uniform mat4 _projectionMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _modelMatrix;
uniform mat4 _normalWorldMatrix;
uniform mat4 _lightMatrix;

uniform vec3 _eyePosition;
uniform vec3 _lightPosition;

out vec3 normalDirection;
out vec4 vertPosition;
out vec4 eyePosition;
out vec3 tangentDirection;
out vec3 bitangentDirection;

// out vec3 eyeDirection;
out vec4 positionLightSpace;
out vec4 lightPosition;
out vec2 uv;

void main() {
  gl_Position = _projectionMatrix * _viewMatrix * _modelMatrix * vec4(in_Position, 1.0);

  // Normal direction in inverse world space?
  normalDirection = normalize((_normalWorldMatrix * vec4( in_Normal, 1.0 ))).xyz;
  tangentDirection = normalize((_normalWorldMatrix * vec4( in_Tangent, 1.0 ))).xyz;
  bitangentDirection = cross(tangentDirection, normalDirection); //  * in_Tangent.w);

  // Vertex position in world space.
  vertPosition = _modelMatrix * vec4( in_Position, 1.0 );

  eyePosition = vec4( _eyePosition, 1.0 );

  // Vector that goes from the vertex to the light, in world space.
  lightPosition = vec4(_lightPosition, 1.0 );

  // Light space matrix
  positionLightSpace = _lightMatrix * vertPosition;

  // UV
  uv = in_TexCoords;
}
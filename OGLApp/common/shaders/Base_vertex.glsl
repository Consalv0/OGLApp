
#version 330 core

const int MAX_JOINTS = 50;
const int MAX_WEIGHTS = 3; 

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoords;
layout(location = 3) in vec3 in_Tangent;
layout(location = 4) in ivec4 in_JointIDs;
layout(location = 5) in vec4 in_Weights;

uniform mat4 _projectionMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _modelMatrix;
uniform mat4 _normalWorldMatrix;
uniform mat4 _lightMatrix;
uniform mat4 _jointTransforms[MAX_JOINTS];

uniform vec3 _eyePosition;
uniform vec3 _lightPosition;

out vec4 vertPosition;
out vec3 normalDirection;
out vec3 tangentDirection;
out vec3 bitangentDirection;

out vec4 eyePosition;
out vec4 positionLightSpace;
out vec4 lightPosition;
out vec2 uv;

void main() {
  vertPosition = vec4(in_Position, 1.0);
  normalDirection = in_Normal;

  // For each weight
  for (int i = 0; i < MAX_WEIGHTS; i++) {
    mat4 jointTransform = _jointTransforms[in_JointIDs[i]];
    vec4 pose = (jointTransform * vec4(in_Position, 1.0)) * in_Weights[i];
    vec4 normal = (jointTransform * vec4(in_Normal, 1.0)) * in_Weights[i];
    vertPosition += pose;
    normalDirection += normal.xyz;
  }

  // Now set the position in view space
  vertPosition = _modelMatrix * vertPosition;
  gl_Position = _projectionMatrix * _viewMatrix * vertPosition;

  // Texture space
  normalDirection = normalize(_normalWorldMatrix * vec4( in_Normal, 1.0 )).xyz; 
  tangentDirection = normalize((_normalWorldMatrix * vec4( in_Tangent, 1.0 ))).xyz;
  bitangentDirection = cross(tangentDirection, normalDirection); //  * in_Tangent.w);

  // Vertex position in world space.
  eyePosition = vec4( _eyePosition, 1.0 );

  // Vector that goes from the vertex to the light, in world space.
  lightPosition = vec4(_lightPosition, 1.0 );

  // Light space matrix
  positionLightSpace = _lightMatrix * vertPosition;

  // UV
  uv = in_TexCoords;
}
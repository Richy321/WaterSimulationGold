//////////////////////////////////////////////////////////////////////////////////////////
//
// Default vertex shader for materials. Extend this to deal with bump mapping, defered rendering, shadows etc.
//

//uniforms
uniform float waves[100];

// matrices
uniform mat4 modelToProjection;
uniform mat4 modelToCamera;

// attributes from vertex buffer
attribute vec4 pos;
attribute vec2 uv;
attribute vec3 normal;
attribute vec4 color;

// outputs
varying out vec3 normal_;
varying out vec2 uv_;
varying out vec4 color_;
varying out vec3 model_pos_;
varying out vec3 camera_pos_;

void main() 
{
  gl_Position = modelToProjection * pos;
  vec3 tnormal = (modelToCamera * vec4(normal, 0.0)).xyz;
  vec3 tpos = (modelToCamera * pos).xyz;
  normal_ = tnormal;
  uv_ = uv;
  color_ = color;
  camera_pos_ = tpos;
  model_pos_ = pos.xyz;
}

float SumSineWaveFunction()
{
	return 0;
}


vec3 GerstnerWaveFunction(vec3 pos)
{
	return vec3(0,0,0);
}



attribute vec3 a_posL;
attribute vec3 a_colorL;
attribute vec2 a_uv;

//varying variables
varying vec2 v_uv;
varying vec4 v_positionClip; // For wireframe
varying vec3 worldPosition; // For fog
varying vec3 cameraPosition;

//uniform variables
uniform mat4 uMVP;
uniform mat4 uModelMatrix; // For fog (need position of 
						   // Model in World)
uniform vec3 uCameraPosition;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = uMVP * posL;

	//Fog
	worldPosition = (uModelMatrix * posL).xyz; // For fog interpolation

	cameraPosition = uCameraPosition;

	v_uv = a_uv; // for coordonate interpolation
	v_positionClip = gl_Position; // Pass position in clip space 
								  // for wireframe computation
}
   
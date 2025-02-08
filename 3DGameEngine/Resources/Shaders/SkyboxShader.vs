attribute vec3 a_posL;
attribute vec3 a_colorL;

//varying variables
varying vec3 v_coord;

//uniform variables
uniform mat4 uMVP;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);
	gl_Position = uMVP * posL;
	v_coord = a_posL; // for coordonate interpolation
}
   
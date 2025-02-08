attribute vec3 a_posL;
attribute vec3 a_colorL;
attribute vec2 a_uv;
attribute vec2 a_uv2;

//varying variables
varying vec2 v_uv;
varying vec2 v_uv2;
varying vec4 c_blend;

//uniform variables
uniform mat4 uMVP;
uniform vec3 u_height;
uniform sampler2D u_texture3; //the blend texture

void main()
{
	c_blend = texture2D(u_texture3, a_uv2);
	
	vec3 newPos = a_posL;
	newPos.y += c_blend.r * u_height.r +
				c_blend.g * u_height.g + 
				c_blend.b * u_height.b;

	vec4 posL = vec4(newPos, 1.0);
	gl_Position = uMVP * posL;
	v_uv = a_uv; // for coordonate interpolation
	v_uv2 = a_uv2;
}
   
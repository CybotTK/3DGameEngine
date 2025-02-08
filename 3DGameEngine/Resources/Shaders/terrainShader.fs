precision mediump float;

//normal variables
vec4 color;

//varying variables
varying vec2 v_uv;
varying vec2 v_uv2;

//uniform variables
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;

void main()
{
	vec4 c_dirt = texture2D(u_texture0, v_uv); //dirt
	vec4 c_rock = texture2D(u_texture1, v_uv); //rock
	vec4 c_grass = texture2D(u_texture2, v_uv); //grass

	vec4 c_blend = texture2D(u_texture3, v_uv2); //terrrain blend

	vec4 c_final = c_blend.r * c_rock +
				   c_blend.g * c_grass +
				   c_blend.b * c_dirt;
    
	c_final.a = 1.0;

	gl_FragColor = c_final;
}

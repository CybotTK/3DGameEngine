precision mediump float;

//normal variables
vec4 color;

//varying variables
varying vec2 v_uv;
varying vec2 v_uv2;
varying vec3 worldPosition;
varying vec3 cameraPosition;

//uniform variables
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;
uniform float u_fogSmallRadius;
uniform float u_fogBigRadius;
uniform vec3 u_fogColor;

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
	
	//fog
	float dist = distance(cameraPosition, worldPosition);
    float alpha = clamp((dist - u_fogSmallRadius)/(u_fogBigRadius-u_fogSmallRadius), 0.0, 1.0);	

	gl_FragColor = alpha * vec4(u_fogColor, 1.0) + (1.0 - alpha) * c_final;
	//gl_FragColor = c_final;
}

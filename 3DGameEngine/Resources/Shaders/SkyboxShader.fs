precision mediump float;

//normal variables
vec4 color;

//varying variables
varying vec3 v_coord;

//uniform variables
uniform samplerCube u_texture0;

void main()
{
color = textureCube(u_texture0, v_coord); // texture2D function returns the colo found at the uv coords
//color = vec4(1.0, 0.0, 0.0, 1.0);

if (color.a < 0.1) discard; // Default texture-based rendering
   gl_FragColor = color;

}

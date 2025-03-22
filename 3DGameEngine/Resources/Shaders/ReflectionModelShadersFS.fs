precision mediump float;

//normal variables
vec4 color;

//varying variables
varying vec2 v_uv;
varying vec3 v_normW;
varying vec4 v_positionClip; // Clip space position for wireframe calculation
varying vec3 worldPosition;
varying vec3 cameraPosition;

//uniform variables
uniform sampler2D u_texture0;
uniform samplerCube u_texture1; // reflection texture skybox
uniform bool u_wireframe; // Add a uniform to toggle wireframe mode
uniform float u_fogSmallRadius;
uniform float u_fogBigRadius;
uniform vec3 u_fogColor;
uniform float u_reflection;

void main()
{
    color = texture2D(u_texture0, v_uv); // texture2D function returns the colo found at the uv coords
    
    float dist = distance(cameraPosition, worldPosition);
    float alpha = clamp((dist - u_fogSmallRadius)/(u_fogBigRadius-u_fogSmallRadius), 0.0, 1.0);

    if (color.a < 0.1) discard; // Default texture-based rendering

    vec4 c_own = alpha * vec4(u_fogColor, 1.0) + (1.0 - alpha) * color; // goes from foggy to visible

    vec3 eyeCam = cameraPosition - worldPosition;
    vec3 dirReflection = reflect (normalize(eyeCam), normalize(v_normW));

    vec4 c_reflection= textureCube(u_texture1, dirReflection);

    vec4 c_final = c_reflection * u_reflection + c_own * (1.0 - u_reflection);

    c_final.a = 1.0;

    gl_FragColor = c_final;
}

precision mediump float;

//normal variables
vec4 color;

//varying variables
varying vec2 v_uv;
varying vec3 worldPosition;
varying vec3 cameraPosition;

//uniform variables
uniform sampler2D u_texture0; // fire mask
uniform sampler2D u_texture1; // fire texture
uniform sampler2D u_texture2; // displacement texture
uniform float u_Time;
uniform float u_DispMax;
uniform float u_fogSmallRadius;
uniform float u_fogBigRadius;
uniform vec3 u_fogColor;

void main()
{
    vec2 disp = texture2D(u_texture2, vec2(v_uv.x, v_uv.y + u_Time)).rg;
    vec2 offset = (disp * 2.0 - 1.0) * u_DispMax;

    vec2 v_uv_displaced = v_uv + offset;

    vec4 c_fire = texture2D(u_texture1, v_uv_displaced);
    vec4 c_alpha = texture2D(u_texture0, v_uv);

    // Multiply alpha with every color from mask
    {
        c_fire.a = c_fire.a * c_alpha.r;
        c_fire.a = c_fire.a * c_alpha.b;
        c_fire.a = c_fire.a * c_alpha.g;
    }

    // Fog calculations
    float dist = distance(cameraPosition, worldPosition);
    float alpha = clamp((dist - u_fogSmallRadius)/(u_fogBigRadius-u_fogSmallRadius), 0.0, 1.0);

    if (c_fire.a < 0.1) discard; // Default texture-based rendering

    gl_FragColor = alpha * vec4(u_fogColor, 1.0) + (1.0 - alpha) * c_fire; // goes from foggy to visible
    
}

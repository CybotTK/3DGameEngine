precision mediump float;

//normal variables
vec4 color;

//varying variables
varying vec2 v_uv;
varying vec4 v_positionClip; // Clip space position for wireframe calculation
varying vec3 worldPosition;
varying vec3 cameraPosition;

//uniform variables
uniform sampler2D u_texture0;
uniform bool u_wireframe; // Add a uniform to toggle wireframe mode
uniform float u_fogSmallRadius;
uniform float u_fogBigRadius;
uniform vec3 u_fogColor;

void main()
{
    color = texture2D(u_texture0, v_uv); // texture2D function returns the colo found at the uv coords
    //color = vec4(v_uv, 0.0, 1.0);
    //if(color.a < 0.1) discard;

    //gl_FragColor = color;

    // Wireframe effect
    if (u_wireframe == true) {
        vec2 f = fract(v_positionClip.xy * 10.0); // Simulate edges
        float edge = min(f.x, f.y);              // Highlight edges
        edge = min(edge, 1.0 - max(f.x, f.y));  // Combine with reverse edges
        edge = smoothstep(0.0, 0.05, edge);     // Smooth edges

        vec4 wireframeColor = vec4(0.0, 0.0, 1.0, 0.5); // Blue, semi-transparent
        gl_FragColor = mix(color, wireframeColor, edge);
    } else {
        u_fogSmallRadius;
        u_fogBigRadius;
        u_fogColor;
        if (color.a < 0.1) discard; // Default texture-based rendering
        //gl_FragColor = 0.5 * vec4(u_fogColor, 1.0) + 0.5 * color;
        gl_FragColor = color;
    }
}

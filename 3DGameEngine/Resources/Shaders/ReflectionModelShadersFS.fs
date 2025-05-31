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

uniform vec3 u_ambiLight;
uniform float u_ratioAmbiLight;

// Lights uniform
uniform int     u_numLights;
uniform vec3    u_ambiLight;
uniform float   u_ratioAmbiLight;
uniform vec3    u_lightDir[10];
uniform vec3    u_lightPos[10];
uniform vec3    u_colorLightDiff[10];
uniform vec3    u_colorLightSpec[10];
uniform float   u_specPower[10];
uniform int     u_lightType[10];  // 0 = Directional, 1 = Point, 2 = Spotlight
uniform float   u_lightRange[10]; // Range for point/spot lights
uniform float   u_spotInnerAngle[10]; // Inner cone angle for spotlights (cosine)
uniform float   u_spotOuterAngle[10]; // Outer cone angle for spotlights (cosine)
uniform float   u_kSpec;
uniform float   u_kDiff;

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
    
    vec4 c_finalReflection = c_reflection * u_reflection + c_own * (1.0 - u_reflection);

    c_final.a = 1.0;

    float dist = distance(cameraPosition, worldPosition);
    float alpha = clamp((dist - u_fogSmallRadius) / (u_fogBigRadius - u_fogSmallRadius), 0.0, 1.0);

    if (color.a < 0.1) discard; 

    vec4 c_obj = c_finalReflection;

    vec4 c_ambiLight = vec4(u_ambiLight, 1.0);
    vec4 comp_ambiLight = c_obj * c_ambiLight;

    vec3 N = normalize(v_normW);
    vec3 E = normalize(cameraPosition - worldPosition);

    vec4 totalLight = vec4(0.0);
    vec4 colDiff;
    float diffFactor;
    float attenuation;
    vec3 L;
    vec4 colSpec;
    float specFactor;
    float distToLight;

    for (int count = 0; count < 10; count++) {
        if (count >= u_numLights) break;

        attenuation = 1.0;

        if (u_lightType[count] == 0) { 
            // **Directional Light**
            L = normalize(u_lightDir[count]);

        } else if (u_lightType[count] == 1) { 
            // **Point Light**
            L = normalize(u_lightPos[count] - worldPosition);
            distToLight = distance(u_lightPos[count], worldPosition);
            attenuation = clamp(1.0 - (distToLight / u_lightRange[count]), 0.0, 1.0);

        } else if (u_lightType[count] == 2) { 
            // **Spotlight**
            L = normalize(u_lightPos[count] - worldPosition);
            distToLight = distance(u_lightPos[count], worldPosition);
            attenuation = clamp(1.0 - (distToLight / u_lightRange[count]), 0.0, 1.0);

            // Calculate spotlight angle falloff
            float spotEffect = dot(normalize(-u_lightDir[count]), L);
            float spotIntensity = smoothstep(u_spotOuterAngle[count], u_spotInnerAngle[count], spotEffect);
            attenuation *= spotIntensity;
        }

        // Diffuse component
        diffFactor = max(dot(N, -L), 0.0);
        colDiff = c_obj * vec4(u_colorLightDiff[count], 1.0) * diffFactor * attenuation;

        // Specular component
        vec3 R = reflect(L, N);
        specFactor = pow(max(dot(R, E), 0.0), u_specPower[count]);
        colSpec = vec4(u_colorLightSpec[count], 1.0) * specFactor * attenuation;

        totalLight += u_kDiff * colDiff + u_kSpec * colSpec;
    }

    vec4 c_light = u_ratioAmbiLight * comp_ambiLight + (1.0 - u_ratioAmbiLight) * totalLight;
    
    gl_FragColor = c_final;
}

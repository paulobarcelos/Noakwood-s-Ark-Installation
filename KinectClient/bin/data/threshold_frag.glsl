#version 120
#extension GL_ARB_texture_rectangle : enable  

uniform sampler2DRect tex0;
uniform float brightPassThreshold;
void main(void) {
    vec3 luminanceVector = vec3(0.2125, 0.7154, 0.0721);
    vec4 sample = texture2DRect(tex0, gl_TexCoord[0].st);

   	float luminance = dot(luminanceVector, sample.aaa);
    luminance = max(0.0, luminance - brightPassThreshold);
    sample.rgba *= sign(luminance);

    gl_FragColor = sample;
}
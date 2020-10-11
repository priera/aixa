#version 330 core

out vec4 color;
in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 alphaMap = texture(text, TexCoords);

    vec4 sampled;
    if (alphaMap.r < 0.5) {
        sampled = vec4(1.0, 1.0, 1.0f, 0.0);
    } else {
        sampled = vec4(1.0, 1.0, 1.0f, 1.0);
    }

    color = vec4(textColor, 1.0) * sampled;
}
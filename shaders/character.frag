#version 330 core

out vec4 color;
in vec2 texCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 alphaMap = texture(text, texCoords);

    color = vec4(textColor, alphaMap.r);
}
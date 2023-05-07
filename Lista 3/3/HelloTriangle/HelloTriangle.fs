//Código fonte do Fragment Shader (em GLSL)
#version 400
out vec4 color;
in vec4 finalColor;
in vec2 texCoord;

uniform sampler2D tex_buffer;

void main()
{
    color = texture(tex_buffer,texCoord);
}
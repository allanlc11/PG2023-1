//Código fonte do Fragment Shader (em GLSL)
#version 400
out vec4 color;
in vec4 finalColor;
void main()
{
    color = finalColor;
}
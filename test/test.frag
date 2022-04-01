#version 330 core
out vec4 FragColor;  
in vec3 result_color;

void main()
{
    FragColor = vec4(result_color, 1.0);
}

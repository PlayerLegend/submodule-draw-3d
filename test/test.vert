#version 330 core
layout (location = 0) in vec3 attribute_position;
layout (location = 1) in vec3 attribute_normal;

uniform mat4 uniform_model_rotation;
uniform mat4 uniform_mvp_transform;
out vec3 result_color;

void main()
{
    gl_Position = uniform_mvp_transform * vec4(attribute_position, 1.0);
    
    vec3 transform_normal = vec3(uniform_model_rotation * vec4(attribute_normal, 1.0));

    result_color = (dot(transform_normal, normalize(vec3(0,1,0))) + 1) * vec3(1,1,1) / 2;
}

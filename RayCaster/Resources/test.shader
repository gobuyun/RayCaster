[vert]
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 fColor;

void main()
{
    fColor = vec3(1.0, 0.0, 0.0);
	gl_Position = vec4(aPos, 1.0);
}






[frag]
#version 330 core
out vec4 FragColor;

in vec3 fColor;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = vec4(fColor, 1.0);
}

[vert]
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUVCoord;

uniform highp mat4 modelViewMat4;
uniform highp mat4 projectionMat4;
uniform highp mat4 normalMat4;
uniform highp int picked;

out vec3 fColor;
out vec2 uvCoord;
out vec3 normal;

void main()
{
    normal = (normalMat4 * vec4(aNormal, 1.0f)).xyz;
    uvCoord = aUVCoord;
    vec3 normalColor = vec3(0.85, 0.80, 0.85) * (1.0-picked);
    vec3 pickedColor = vec3(1.0, 0.0, 0.0) * (picked - 0.0);
    vec3 useColor = normalColor + pickedColor;
    vec3 diffuse = useColor * abs(dot(normal, vec3(0.0, 0.0, 1.0))) * 0.7;
    vec3 ambient = useColor * 0.3;
    fColor = ambient + diffuse;
    gl_Position = projectionMat4 * modelViewMat4 * vec4(aPos, 1.0f);
    //gl_Position = vec4(aPos, 1.0f);
}






[frag]
#version 330 core
out vec4 FragColor;

in vec3 fColor;
in vec2 uvCoord;
in vec3 normal;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = vec4(fColor, 1.0);
}

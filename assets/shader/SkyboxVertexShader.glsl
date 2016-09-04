uniform mat4 viewmatrix;
uniform mat4 projmatrix;

varying vec3 texCoords;

void main(void)
{


    gl_Position = gl_ProjectionMatrix * viewmatrix * gl_Vertex;

    vec4 position = gl_Vertex;
    texCoords = position.xyz;
}


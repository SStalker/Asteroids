uniform mat4 viewmatrix;
uniform mat4 projmatrix;

varying vec3 texCoords;

void main(void)
{

    mat4 vm = viewmatrix;
    vm[3][0] = vm[3][1] = vm[3][2] = 0.0;

    gl_Position = gl_ProjectionMatrix * vm * gl_Vertex;

    vec4 position = gl_Vertex;
    texCoords = position.xyz;
}


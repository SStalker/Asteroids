varying vec3 texCoords;

void main(void)
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec4 position = (gl_ModelViewMatrix * gl_Vertex);
    texCoords = position.xyz;
}

varying vec3 texCoords;

void main(void)
{
    gl_Position = gl_ProjectionMatrix * gl_Vertex ;
    vec4 position = (gl_ModelViewMatrix * gl_Vertex);
    texCoords = position.xyz;
}

varying vec3 Normal;
varying vec3 Position;
varying vec2 Texcoord;

void main(void)
{
    Position = (gl_ModelViewMatrix * gl_Vertex).xyz;
    Normal = gl_NormalMatrix*gl_Normal;
    Texcoord = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

varying vec3 texCoords;

uniform samplerCube skybox;

void main(void)
{
    gl_FragColor = textureCube(skybox, texCoords);
}

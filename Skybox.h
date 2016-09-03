#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"


#define GL_DEBUG

#ifdef GL_DEBUG
#define CHECKGLSKY CheckGLErrorsSkybox();
#else
#define CHECKGLSKY ;
#endif

void CheckGLErrorsSkybox();


class Skybox: public Texture
{
private:
    vector<string> Textures;
    ShaderProgram sp;
    const char* VertexShader;
    const char* FragmentShader;
    float skyboxSize = 500.f;
    GLuint skyboxVertexBuffer;

public:
    Skybox();
    Skybox(vector<string> textures, const char* vertexShader, const char* fragmentShader);
    Skybox(vector<string> textures, const char* vertexShader, const char* fragmentShader, float size);
    bool loadSkybox();
    void loadShader();
    void createVertices();

    void apply() const;
    void draw();
};

#endif // SKYBOX_H

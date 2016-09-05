/*
 * Der Code dieser Klasse bassiert teilweise auf Ideen von
 * http://learnopengl.com/#!Advanced-OpenGL/Cubemaps
 * die an unsere Umgebung angepasst sind.
 */

#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "global.h"

class Skybox: public Texture
{
private:
    vector<string> Textures;
    ShaderProgram sp;
    const char* VertexShader;
    const char* FragmentShader;
    float skyboxSize = 500.f;
    GLuint skyboxVertexBuffer;
    GLuint skyboxArrayBuffer;
    bool loadSkybox();
    void loadShader();
    void createVertices();
    void apply() const;

public:
    Skybox(vector<string> textures, const char* vertexShader, const char* fragmentShader);
    Skybox(vector<string> textures, const char* vertexShader, const char* fragmentShader, float size);

    void draw();
};

#endif // SKYBOX_H

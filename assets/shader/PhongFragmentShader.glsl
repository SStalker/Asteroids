uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 DiffColor;
uniform vec3 SpecColor;
uniform vec3 AmbientColor;
uniform float SpecExp;

uniform sampler2D DiffuseTexture;

varying vec3 Normal;
varying vec3 Position;
varying vec2 Texcoord;

float sat(float a)
{
    return clamp(a, 0.0f, 1.0f);
}

void main(void)
{
    vec3 N = normalize(Normal);
    vec3 L = normalize(LightPos-Position);
    vec3 E = normalize(-Position);
    vec3 R = reflect(-L, N);

    vec3 DiffuseComponent = DiffColor * sat(dot(N,L));
    vec3 SpecularComponent = SpecColor * pow( sat(dot(E,R)), SpecExp);

    vec3 DiffuseTexColor = texture2D(DiffuseTexture, Texcoord).rgb;
    DiffuseComponent *= DiffuseTexColor;

    vec3 AmbientComponent = AmbientColor*DiffuseTexColor;

    vec3 fragColor = DiffuseComponent + SpecularComponent + AmbientComponent;

    gl_FragColor = vec4(DiffuseComponent + SpecularComponent + AmbientComponent, 0.0);
}

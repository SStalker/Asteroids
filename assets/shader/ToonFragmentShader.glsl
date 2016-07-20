const float QUANT = 3.0;

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


float QuantizeValue(float f)
{
        f = floor(f*QUANT);
        f /= QUANT;
        return f;
}

float CalcOutline(vec3 N, vec3 E)
{
	float Outline = abs(dot(N,E));

	if(Outline < cos(3.14*80.0/180.0))
		Outline = 0.0;
	else
		Outline = 1.0;

	return Outline;
}

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

	float DiffFactor = sat(dot(N,L));
	DiffFactor = QuantizeValue(DiffFactor);
	DiffFactor = max(DiffFactor, 0.3);
	vec3 DiffuseComponent = DiffColor * DiffFactor;

	float SpecFactor = pow( sat(dot(E,R)), SpecExp );
	SpecFactor = QuantizeValue(SpecFactor);
	vec3 SpecularComponent = SpecColor * SpecColor;

        vec3 DiffuseTexColor = texture2D(DiffuseTexture, Texcoord).rgb;
        DiffuseComponent *= DiffuseTexColor;

        vec3 AmbientComponent = AmbientColor*DiffuseTexColor;

	float Outline = CalcOutline(N, E);

	gl_FragColor = Outline * vec4(DiffuseComponent + SpecularComponent + AmbientComponent, 0);
}

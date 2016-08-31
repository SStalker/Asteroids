CC=g++
OPTS =  -I"libs/irrKlang-64bit-1.5.0/include" -L"/usr/lib" libs/irrKlang-64bit-1.5.0/bin/linux-gcc-64/libIrrKlang.so -pthread

all:
	$(CC) -g -lGL -lGLU -lglut -DGL_GLEXT_PROTOTYPES -std=c++11 \
	main.cpp \
	Vector.cpp \
	Color.cpp \
	Matrix.cpp \
	ShaderProgram.cpp \
	Model.cpp \
	Material.cpp \
	Texture.cpp \
	Camera.cpp \
	Rgbimage.cpp \
	Spaceship.cpp \
	Quaternion.cpp \
	GameObject.cpp \
	Asteroid.cpp \
	Projectile.cpp \
	Planet.cpp \
	Game.cpp \
	-o main $(OPTS)

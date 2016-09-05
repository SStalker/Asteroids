CC=g++
OPTS = \
-I"libs/irrKlang-64bit-1.5.0/include" -L"/usr/lib" libs/irrKlang-64bit-1.5.0/bin/linux-gcc-64/libIrrKlang.so  \
-I"libs/libdrawtext/src" -L"libs/libdrawtext" libs/libdrawtext/libdrawtext.so libs/libdrawtext/libdrawtext.so.0 \
-pthread

all:
	$(CC) -g -lGL -lGLU -lGLEW -lglut -Wl,-rpath=libs/libdrawtext -ldrawtext -DGL_GLEXT_PROTOTYPES -std=c++11 \
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
	GameObject.cpp \
	Asteroid.cpp \
	Projectile.cpp \
	Planet.cpp \
	Skybox.cpp \
	Game.cpp \
	CollisionDetection.cpp \
	SoundManager.cpp \
	-o Asteroids $(OPTS)

mac: $(CC) -g -w -framework OpenGL -framework GLUT -std=c++11 \
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
	GameObject.cpp \
	Asteroid.cpp \
	Projectile.cpp \
	Planet.cpp \
	Skybox.cpp \
	Game.cpp \
	CollisionDetection.cpp \
	SoundManager.cpp \
	-o Asteroids $(OPTS)
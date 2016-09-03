#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <irrKlang.h>
#include <iostream>

#pragma comment(lib, "irrKlang.lib")

class SoundManager{
private:
  irrklang::ISoundEngine* engine;

  SoundManager();
  ~SoundManager();

public:
  static SoundManager* getInstance();
  void init();
  void playBackgroundMusic();
  void playShootingSound();
};

#endif

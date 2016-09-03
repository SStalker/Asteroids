#include "SoundManager.h"

SoundManager::SoundManager(){}
SoundManager::~SoundManager(){}

SoundManager* SoundManager::getInstance()
{
    static SoundManager instance;
    return &instance;
}

void SoundManager::init()
{
  engine = irrklang::createIrrKlangDevice();
  if (!engine)
  {
    std::cout << "Could not startup engine\n";
  }
}

void SoundManager::playBackgroundMusic()
{
  // play some sound stream, looped
  engine->play2D("assets/sounds/background.ogg", true);
}

void SoundManager::playShootingSound()
{
  // play some sound stream, nonlooped
  engine->play2D("assets/sounds/shoot.ogg", false);
}

void SoundManager::playExplosionSound()
{
  // play some sound stream, nonlooped
  engine->play2D("assets/sounds/explode.wav", false);
}

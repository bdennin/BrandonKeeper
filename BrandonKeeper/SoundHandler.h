#ifndef SOUNDHANDLER_HPP
#define SOUNDHANDLER_HPP

#include <SFML\Audio.hpp>

#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace sf;

class SoundHandler
{

private:

	static const int GENERIC_SOUND_COOLDOWN = 20;
	static const int CRITICAL_SOUND_COOLDOWN = 50;
	static const int MESSAGE_SOUND_COOLDOWN = 10000;
	static const int SPELL_SOUND_COOLDOWN = 30;
	static const int URGENT_SOUND_COOLDOWN = 10;

	static Clock* criticalTimer;
	static Clock* messageTimer;
	static Clock* spellTimer;
	static Clock* soundTimer;
	static Clock* urgentTimer;
	static map<string, Music*> musicMap;
	static map<string, SoundBuffer*> soundMap;
	static vector<string> songs;
	static vector<string> sounds;
	static Music* music;
	static Sound* urgentSound;
	static Sound* criticalSound;
	static Sound* genericSound;
	static Sound* message;
	static Sound* spellSound;

public:

	static void initializeSounds();
	static void destroySounds();
	static void startMenuMusic();
	static void startGameMusic();
	static void playUrgentSound(string name);
	static void playCriticalSound(string name);
	static void playMessage(string name);
	static void playSound(string name);
	static void playSpellSound(string name);
	static void setVolume(float volume);
};

#endif
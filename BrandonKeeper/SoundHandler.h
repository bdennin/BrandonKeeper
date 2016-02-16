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

	static const int MAX_SIMULTANEOUS_SOUNDS = 3;

	static map<string, Music*> musicMap;
	static map<string, SoundBuffer*> soundMap;
	static vector<string> songs;
	static vector<string> sounds;
	static Music* music;
	static Sound* message;
	static Sound* criticalSound;
	static Sound* spellSound;
	static vector<Sound*> sound;
	
	static int soundCounter;

public:

	static void initializeSounds();
	static void destroySounds();
	static void startMenuMusic();
	static void startGameMusic();
	static void playCriticalSound(string name);
	static void playMessage(string name);
	static void playSound(string name);
	static void playSpellSound(string name);
	static void setVolume(float volume);
};

#endif
#include "SoundHandler.h"

map<string, Music*> SoundHandler::musicMap;
map<string, SoundBuffer*> SoundHandler::soundMap;

vector<string> SoundHandler::songs;
vector<string> SoundHandler::sounds;

Music* SoundHandler::music;
Sound* SoundHandler::criticalSound;
Sound* SoundHandler::message;
Sound* SoundHandler::spellSound;
vector<Sound*> SoundHandler::sound;

int SoundHandler::soundCounter = 0;

void SoundHandler::initializeSounds()
{
	criticalSound = new Sound();
	message = new Sound();
	spellSound = new Sound();

	for (int i = 0; i < MAX_SIMULTANEOUS_SOUNDS; i++)
		sound.push_back(new Sound());

	SoundBuffer* buffer;
	Music* music;
	string name;

	sounds.push_back("gold_drop_1");
	sounds.push_back("gold_drop_2");
	sounds.push_back("gold_drop_3");
	sounds.push_back("imp_mine_1");
	sounds.push_back("imp_mine_2");
	sounds.push_back("imp_mine_3");
	sounds.push_back("imp_mine_4");
	sounds.push_back("imp_mine_5");
	sounds.push_back("imp_mine_6");
	sounds.push_back("imp_spawn");
	sounds.push_back("imp_step_1");
	sounds.push_back("imp_step_2");
	sounds.push_back("imp_step_3");
	sounds.push_back("imp_step_4");
	sounds.push_back("option_failure");
	sounds.push_back("option_select_1");
	sounds.push_back("option_select_2");
	sounds.push_back("tile_build");
	sounds.push_back("tile_cave-in_1");
	sounds.push_back("tile_cave-in_2");
	sounds.push_back("tile_cave-in_3");
	sounds.push_back("tile_select");
	sounds.push_back("tile_sell");
	sounds.push_back("voice_bigger_treasury");
	sounds.push_back("voice_game_loaded");
	sounds.push_back("voice_tunneled_new_area");


	int length = sounds.size();

	for (int i = 0; i < length; i++)
	{
		buffer = new SoundBuffer();
		name = sounds[i];
		buffer->loadFromFile("Resources/" + name + ".ogg");
		soundMap[name] = buffer;
	}

	songs.push_back("game_music");
	songs.push_back("menu_music");
	length = songs.size();

	for (int i = 0; i < length; i++)
	{
		music = new Music();
		name = songs[i];
		music->openFromFile("Resources/" + name + ".ogg");
		musicMap[name] = music;
	}
}

void SoundHandler::destroySounds()
{
	SoundBuffer* buffer;
	Music* music;
	string name;

	int length = sounds.size();

	for (int i = 0; i < length; i++)
	{
		name = sounds[i];
		buffer = soundMap[name];
		delete buffer;
	}

	length = songs.size();

	for (int i = 0; i < length; i++)
	{
		name = songs[i];
		music = musicMap[name];
		delete music;
	}

	for (int i = 0; i < MAX_SIMULTANEOUS_SOUNDS; i++)
		delete sound[i];

	delete criticalSound;
	delete message;
	delete spellSound;
}

void SoundHandler::startGameMusic()
{
	int offset;
	int random = rand() % 4;

	switch (random)
	{
		case 0:
		{
			offset = 0;
			break;
		}
		case 1:
		{
			offset = 326;
			break;
		}
		case 2:
		{
			offset = 633;
			break;
		}
		case 3:
		{
			offset = 933;
			break;
		}
		default:
			break;
	}

	music = musicMap["game_music"];
	music->setLoop(true);
	music->play();
	music->setPlayingOffset(seconds(offset));
}

void SoundHandler::startMenuMusic()
{
	music = musicMap["menu_music"];
	music->setLoop(true);
	music->play();
}

void SoundHandler::playCriticalSound(string name)
{
	criticalSound->setBuffer(*soundMap[name]);
	criticalSound->play();
}

void SoundHandler::playMessage(string name)
{
	message->setBuffer(*soundMap[name]);
	message->play();
}

void SoundHandler::playSound(string name)
{
	int i = soundCounter;
	sound[i]->setBuffer(*soundMap[name]);
	sound[i]->play();

	soundCounter++;

	if (soundCounter >= MAX_SIMULTANEOUS_SOUNDS)
		soundCounter = 0;
}

void SoundHandler::playSpellSound(string name)
{
	spellSound->setBuffer(*soundMap[name]);
	spellSound->play();
}

void SoundHandler::setVolume(float volume)
{
	music->setVolume(volume);

	for (int i = 0; i < SoundHandler::MAX_SIMULTANEOUS_SOUNDS; i++)
		sound[i]->setVolume(volume);
}
#include <wiringPi.h> //Wiring Pi
#include <stdio.h>
#include <SDL2/SDL.h> //For Sound Library (The core of it)
#include <SDL2/SDL_mixer.h> //For Sound Library (The actual sound stuff we need)
#include <signal.h>


//Define the pins we used in terms of wiring pi's language
#define ledStrip 0
#define laserSensor 1


volatile int keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
}


int main(void){
signal(SIGINT, intHandler);
	// When initialize wiring failed, print message to screen
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}

	//Declare pins as input or output
	pinMode(ledStrip, OUTPUT);
	pinMode(laserSensor, INPUT);
	
    	//HIGH is off, so set the lights off to start	
	digitalWrite(ledStrip, HIGH);


	//Initialize the sound libray itself
	if (SDL_Init(SDL_INIT_AUDIO) < 0) { //Check if library sets up right, if not return error
		printf("SDL could not initialize! %s\n", SDL_GetError());
		return 1;
	}

	// Initialize the part of the sound library that plays sounds
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { //Check if library sets up right, if not return error
		printf("SDL_mixer could not initialize! %s\n", Mix_GetError());
		SDL_Quit();
		return 1;
	}

	// Load WAV file for Cheering Sound
	Mix_Chunk *cheerSound = Mix_LoadWAV("cheerSound.wav");
	if (!cheerSound) { //Check if sound is loaded good, if not return error
		printf("Failed to load sound.wav! %s\n", Mix_GetError());
		Mix_CloseAudio();
		SDL_Quit();
		return 1;
	}


	//Declare these variables so we make sure the sound doesnt spam
	int lastLaserState = 1;

	while(keepRunning/*1*/){

		//BUTTON

		//Update button state
		int currentLaserState = digitalRead(laserSensor);

		// Indicate that button has pressed down
		if(currentLaserState == 0){
			//Led on
			digitalWrite(ledStrip, LOW);
			//play laserSound IFF the lastButton state was off (avoids constant replaying sound and spam and doom)
			if (lastLaserState == 1) {
				//Play cheer sound once
				Mix_PlayChannel(-1, cheerSound, 0);
				//Turn LED On
				digitalWrite(ledStrip, LOW);

			}
			//printf("...LED on\n");

		}
		else{
			//Turn LED & Laser Off
			digitalWrite(ledStrip, HIGH);

			//printf("LED off...\n");
		}
		lastLaserState = currentLaserState;


	}


	// Clean up sound library stuff before end program
	Mix_FreeChunk(cheerSound);
	Mix_CloseAudio();
	SDL_Quit();

	//printf("Sound finished!\n");


	return 0;
}


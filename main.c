#include <wiringPi.h> //Wiring Pi
#include <stdio.h>
//#include <SDL2/SDL.h> //For Sound Library (The core of it)
//#include <SDL2/SDL_mixer.h> //For Sound Library (The actual sound stuff we need)
#include <signal.h>
#include "ws2811.h" //For the LED strip

#include <sys/time.h> //For Ultrasonic

//Special Defines for the LED Strip Stuff To Work
#define TARGET_FREQ WS2811_TARGET_FREQ //Signal Timing (how fast bits are sent)
#define GPIO_PIN 18 //Needs to be GPIO Pin 18 for this to work ig
#define DMA 10 //Channel timing idk what means
#define LED_COUNT 10 //LEDs on the strip, can be changed later

//Define the other pins we used in terms of wiring pi's language
#define Trig    2
#define Echo    3





float disMeasure(void)
{
	struct timeval tv1;
	struct timeval tv2;
	long time1, time2;
	float dis;

	digitalWrite(Trig, LOW);
	delayMicroseconds(2);

	digitalWrite(Trig, HIGH);
	delayMicroseconds(10);      
	digitalWrite(Trig, LOW);

	while(!(digitalRead(Echo) == 1));
	gettimeofday(&tv1, NULL);           

	while(!(digitalRead(Echo) == 0));
	gettimeofday(&tv2, NULL);           

	time1 = tv1.tv_sec * 1000000 + tv1.tv_usec;   
	time2  = tv2.tv_sec * 1000000 + tv2.tv_usec;

	dis = (float)(time2 - time1) / 1000000 * 34000 / 2;  //求出距离

	return dis;
}





// ULTRASONIC DEMO CODE
/*int main(void)
  {
  float dis;

  if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
  printf("setup wiringPi failed !");
  return 1;
  }

  ultraInit();

  while(1){
  dis = disMeasure();
  printf("%0.2f cm\n\n",dis);
  delay(300);
  }

  return 0;
  }*/

///END ULTRASOUND CODE


















volatile int keepRunning = 1;

void intHandler(int dummy) {
	keepRunning = 0;
}


//For ultrasonic ig
void ultraInit(void)
{
	pinMode(Echo, INPUT);
	pinMode(Trig, OUTPUT);
}


//This is basically the object we use to control the LED strip itself
ws2811_t ledstring = {
	.freq = TARGET_FREQ, 
	.dmanum = DMA,
	.channel[0] = {
		.gpionum = GPIO_PIN, //Which pin sends data
		.count = LED_COUNT, //How many LEDs
		.invert = 0,
		.brightness = 255, //Max Brightness
	},
};



int main(void){


	//Declare pins as input or output
	//	pinMode(laserSensor, INPUT);


	/*
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

*/


	signal(SIGINT, intHandler);
	// When initialize wiring failed, print message to screen
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}

	//Initialize LED strip
	//ws2811_init(&ledstring);

	//Initialize and If LED fails, print message to screen
	if (ws2811_init(&ledstring) != WS2811_SUCCESS) {
		printf("LED init failed\n");
		return 1;
	}


	float dis; //for ultrasound
	ultraInit(); //initialize ultrasound

	while(keepRunning/*1*/){


		//TODO: Replace currentLaserState with ultraSound

		dis = disMeasure();

		// Indicate that UltraSound is triggered
		if(dis < 200) { //TODO: random guess number here can be changed

			//Play cheer sound once
			//Mix_PlayChannel(-1, cheerSound, 0);
			//Loop over LED strip and turn them all on

			for (int i = 0; i < LED_COUNT; i++) {
				//LED Strip uses GRB Format Like A Weirdo (0x00GGRRBB)
				//Make a line of 3 colors
				if (i % 3 == 0) {
					ledstring.channel[0].leds[i] = 0x00FFC0CB; //Pink
				} else if ((i + 1) % 3 == 0) {
					ledstring.channel[0].leds[i] = 0x00C2B280; //Sandy Yellow
				} else {
					ledstring.channel[0].leds[i] = 0x0039FF14; //Neon Green

				}
			}
			ws2811_render(&ledstring); //Actually Renders the New Colors, Old Code Just Changed Data in the Object We made at the Start
			delay(100); //so lights turn on slowly


			//}
			//printf("...LED on\n");

	}
	else{
		//Turn LED & Laser Off
		for (int i = 0; i < LED_COUNT; i++) {
			ledstring.channel[0].leds[i] = 0x00000000; //Turn All LEDs off
		}
		ws2811_render(&ledstring); //Actually Renders the New Colors, Old Code Just Changed Data in the Object We made at the Start

		//printf("LED off...\n");
	}

	delay(10); //Add delay or else pi will just go waaaaay too fast, makes it better
}


// Clean up sound & Lights library stuff before end program
//Mix_FreeChunk(cheerSound); //Sound Clear
//Mix_CloseAudio(); //Sound Clear
//SDL_Quit(); //Sound Clear
ws2811_fini(&ledstring); //Lights Clear


//printf("Sound finished!\n");


return 0;
}


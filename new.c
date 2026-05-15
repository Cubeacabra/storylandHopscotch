#include <wiringPi.h> //Wiring Pi
#include <stdio.h>
//#include <signal.h>
#include <sys/time.h> //For Ultrasonic

//Define the other pins we used in terms of wiring pi's language
#define led1 1
#define led2 2
#define led3 3
#define Trig 4
#define Echo 5


//DEV NOTE: Compile With Below:
//gcc new.cc -lwiringPi


//For Ultrasound Calulations 
/*float disMeasure(void)
{
	printf("in dismeasure functionm\n");
	struct timeval tv1;
	struct timeval tv2;
	long time1, time2;
	float dis;



	digitalWrite(Trig, LOW);
	delayMicroseconds(2);

	digitalWrite(Trig, HIGH);
	delayMicroseconds(10);      
	digitalWrite(Trig, LOW);



	while(!(digitalRead(Echo)) == 1);
	gettimeofday(&tv1, NULL);           

	printf("this part success (PART 1)\n");


	while(!(digitalRead(Echo)) == 0);
	printf("this part success (PART 1.55555)\n");
	gettimeofday(&tv2, NULL);           



	printf("this part success (PART 2)\n");

	time1 = tv1.tv_sec * 1000000 + tv1.tv_usec;   
	time2  = tv2.tv_sec * 1000000 + tv2.tv_usec;

	dis = (float)(time2 - time1) / 1000000 * 34000 / 2;

	return dis;
}*/

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
	//printf("finished first loop\n");


    while(!(digitalRead(Echo) == 0));   
    gettimeofday(&tv2, NULL);           
	//printf("finished second loop\n");


    time1 = tv1.tv_sec * 1000000 + tv1.tv_usec;   
    time2  = tv2.tv_sec * 1000000 + tv2.tv_usec;

	//printf("Time1 is %ld \n", time1);
	//printf("Time2 is %ld \n", time2);
		
	//printf("Delta Time is %ld \n", (time2-time1));
	//printf("Delta Time converted is %ld \n", float(time2-time1)/1000000);
	//printf("Delta Time converted (2) is %ld \n", float(time2-time1)/1000000 * 34000 / 2);
	

    dis = (float)(time2 - time1) / 1000000 * 34000 / 2;  

    return dis;
}


//For ultrasonic initialization
void ultraInit(void)
{
	//printf("in ultraInt functionm");
	pinMode(Echo, INPUT);
	pinMode(Trig, OUTPUT);
}

/*
//So program can end
//volatile int keepRunning = 1;

void intHandler(int dummy) {
	printf("in intHandler functionm");
	keepRunning = 0;
}

*/

int main(void){

	//printf("First line runs\n");

	//signal(SIGINT, intHandler);

	// When initialize wiring failed, print message to screen
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed !");
		return 1;
	}

	//Set Pin Modes
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led3, OUTPUT);


	//Make sure they start off
	digitalWrite(led1,HIGH);
	digitalWrite(led2,HIGH);
	digitalWrite(led3,HIGH);

	//printf("pins set\n");

	float dis; //for ultrasound
	ultraInit(); //initialize ultrasound

	while(1){
	//printf("got in loop");
	
	
		//TODO: Replace currentLaserState with ultraSound

		dis = disMeasure();
		
	printf("this part success\n");
	
	//OLD BROKE GOT TO HERE
	
		printf("distance is %f cm\n", dis);	
		// Indicate that UltraSound is triggered
		if(dis < 60) { //TODO: random guess number here can be changed
			printf("distance branch triggered");
			for (int i = 0; i < 3; i++) {
				//LED Strip uses GRB Format Like A Weirdo (0x00GGRRBB)
				//Make a line of 3 colors
				if (i % 3 == 0) {
					digitalWrite(led1,LOW);
				} else if ((i + 1) % 3 == 0) {
					digitalWrite(led2,LOW);
				} else {
					digitalWrite(led3,LOW);
				}
			}

			delay(100); //so lights turn on slowly
			printf("...LED on\n");

	}
	else{
		//Turn LED & Laser Off
			printf("distance branch NOTTT triggered");
			for (int i = 0; i < 3; i++) {
				//LED Strip uses GRB Format Like A Weirdo (0x00GGRRBB)
				//Make a line of 3 colors
				if (i % 3 == 0) {
					digitalWrite(led1,LOW);
				} else if ((i + 1) % 3 == 0) {
					digitalWrite(led2,LOW);
				} else {
					digitalWrite(led3,LOW);
				}
			}
		printf("LED off...\n");
	}

	delay(10); //Add delay or else pi will just go waaaaay too fast, makes it better
}

return 0;
}


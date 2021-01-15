#include "address_map_arm.h"

volatile int *LEDptr = (int*)LED_BASE; 				//pointer to register connected to LEDs
volatile int *PBptr = (int*)KEY_BASE;				//pointer to register connected to pushbuttons
volatile int *SWptr = (int*)SW_BASE;				//pointer to register connected to switches
volatile int *HIGHHEXptr =(int*)HEX5_HEX4_BASE;		//pointer to hogher 7 segment displays
volatile int *LOWHEXptr = (int*)HEX3_HEX0_BASE;		//pointer to lower 7 segment displays
volatile int * AUDIOptr = (int *)AUDIO_BASE;		//pointer to audio input

int readSwitches();									//declaration of function to read current the switch value
void turnOn();										//declaration of function to turn on the system
void turnOff();										//declaration of function to turn off the system
void modeOne();										//declaration of function to execute mode one of the system
void modeTwo();										//declaration of function to execute mode two of the system
void modeThree();									//declaration of function to execute mode three of the system
void chooseDisplay();								//declaration of function to prompt the user to choose one mode 
													//in the event that more than one mode was requested


int main(void) {									//entry point of the program

	while(1){										//loop through program indefinitely

		volatile int modenum = readSwitches();		//initialize integer that holds desired mode number based on switches 
		
		switch(modenum){							//switch statement to organize modes  	
		
			case 1 :								//mode for only first switch on, representing a system ON state
				turnOn();							//call function to turn the system on
				break;
				
			case 3 :								//mode for first and second switches on, representing a system 
				modeOne();							//ON state, in mode 1, so call fuction to execute mode 1
				break;
				
			case 5 :								//mode for first and third switches on, representing a system
				modeTwo();							//ON state, in mode 2, so call fuction to execute mode 2
				break;
				
			case 9 :								//mode for first and fourth switches on, representing a system
				modeThree();						//ON state, in mode 3, so call fuction to execute mode 3
				break;
				
			case 7 :								//mode for first, second, and third switches on, representing
				chooseDisplay();					//an indecisive choice, so prompt to choose a proper input
				break;								//by calling choose function
				
			case 11 :								//mode for first, second, and fourth switches on, representing
				chooseDisplay();					//an indecisive choice, so prompt to choose a proper input
				break;								//by calling choose function
				
			case 13 :								//mode for first, third, and fourth switches on, representing
				chooseDisplay();					//an indecisive choice, so prompt to choose a proper input
				break;								//by calling choose function
			
			case 15 :								//mode for all four switches on, representing
				chooseDisplay();					//an indecisive choice, so prompt to choose a proper input		
				break;								//by calling choose function
												
			default	:								//mode for if the first switch is not on, representing a system OFF state
				turnOff();							//call function to turn the system on
				break;
		}
	}
}

int readSwitches(){
	return (*(SWptr))%16;							//return value from switches (mod 16 to 
}													//nullify most significant 6 switches have no effect)

void turnOn(){
	*(HIGHHEXptr) = 0x00000000;						//sets first 2 seven segments to nothing
	*(LOWHEXptr) = 0x00003F54;						//sets last 4 seven segments to 'On'
}

void turnOff(){
	*(HIGHHEXptr) = 0x00000000;						//sets first 2 seven segments to nothing
	*(LOWHEXptr) = 0x003F7171;						//sets last 4 seven segments to 'Off'
}

void modeOne(){
	*(HIGHHEXptr) = 0x0000006D;						//sets first 2 seven segments to 'SY'
	*(LOWHEXptr) = 0x6E6D0006;						//sets last 4 seven segments to 'S 1'
	
	*(LEDptr) = 0x00000000;							//Turn LEDs off to start
	
	while(1){										//continue until switches (aka mode) is not 1
		if(readSwitches() != 3){					//checks if switches have change 
			*(LEDptr) = 0x00000000;					//if we don't want to be in mode 1 anymore, turn LEDs off
			break;									//break out of loop
		}
		else if(*(AUDIOptr+2) > 500000000){			//if the sound reading is more than this value
			*(LEDptr) = 0xFFFFFFFF;					//turn on LEDs if threshold is reached
		}
		else{										//if threshold is not reached
			*(LEDptr) = 0x00000000;					//turn off the LEDS
		}
	}
}

void modeTwo(){
	*(HIGHHEXptr) = 0x0000006D;						//sets first 2 seven segments to 'SY'
	*(LOWHEXptr) = 0x6E6D005B;						//sets last 4 seven segments to 'S 2'
	
	int proceed = 1;								//variable containing logic to continue this mode or not
	int patternnum = 0;								//variable containing logic of which pattern to execute
	
	volatile int increasing1 = 1;					//variable containing logic of increasing or decreasing part of 
													//pattern in pattern 1 
	volatile int currpos1 = 0;						//variable containing logic of current LED wanted to manipulate 
													//in pattern 1
	volatile int currpos2 = 9;						//variable containing logic of another current LED wanted to 
													//manipulate in pattern 1
	volatile int brightLED1 = 1;					//variable containing logic value needed to set LED high/low

	volatile int LED2 = 0x55555555;					//value to set all LEDs high in mode 2
	
	volatile int LED3 = 0x00000000;					//value to set all LEDs low in mode 3
	volatile int increasing3 = 1;					//variable containing logic of increasing or decreasing part of 
													//pattern in pattern 3 
	volatile int currpos3 = 0;						//variable containing logic of current LED wanted to manipulate 
													//in pattern 3
	volatile int on3 = 1;							//variable containing logic of turning on or turning off part of 
													//pattern in pattern 3 

	while(proceed == 1){							//continue until switches (aka mode) is not 2
	increasing1 = 1;								//resetting variable at the beginning of each loop
	currpos1 = 0;									//resetting variable at the beginning of each loop
	currpos2 = 9;									//resetting variable at the beginning of each loop
	brightLED1 = 1;									//resetting variable at the beginning of each loop
	LED3 = 0x00000000;								//resetting variable at the beginning of each loop
	increasing3 = 1;								//resetting variable at the beginning of each loop
	currpos3 = 0;									//resetting variable at the beginning of each loop
	on3 = 1;										//resetting variable at the beginning of each loop
	
		switch(patternnum){							//switch statement to organize patterns  	

			case 0 :								//pattern 1
				while(1){							//continue until button is pressed to switch patterns
						
					*(LEDptr) = (brightLED1 << currpos1)|(brightLED1 << currpos2); //the functioning of this line is better
					//explained through an example. Assuming brightLED1 is a 1, currpos1 is a 2, and currpos2 is 7, 
					//brightLED1 << currpos1 is equivelant to 0000 0100, and brightLED1 << currpos2 is equivelant to 
					// 1000 0000, so by 'or-ing' these together we create 10000100, and send this to the LEDs, effectivley lighting 
					//the second and seventh LEDs

					if(readSwitches() != 5){		//continue until switches (aka mode) is not 2
						patternnum = 3;				//if we don't want to be in mode 1 anymore, prepare to go to default switch 
						break;
					}
					else if((*(PBptr) & 1) == 1){	//check if pushbutton has been pressed indicating a requested pattern change
						patternnum = (patternnum+1)%3; //update variable expressing desired pattern
						break;
					}				
					else{							//case to run pattern 1 as per normal
						if(increasing1){			//if in increasing portion of pattern
							if(currpos1 == 9){		//if we have reached to boundary of the LEDs
								increasing1 = 0;	//change to decreasing segement
							}
							else{					//if we have not reached the boundary of the LEDs
								currpos1++;			//increment position of one LED
								currpos2--;			//decrement position of other LED
							}
						}
						else{						//if in decreasing portion of pattern
							if(currpos1 == 0){		//if we have reached a boundary of the LEDs
								increasing1 = 1;	//change to inscreasing segement
							}
							else{					//if we have not reached the boundary of the LEDs
								currpos1--;			//decrement position of one LED
								currpos2++;			//increment position of other LED
							}
						}
						volatile int delay = 300000; //delay to simply make patterns visible
						while(delay){
							delay--;
						}
					}		
				}
				break;
				
			case 1 :								//pattern 2
				while(1){							//continue until button is pressed to switch patterns
						
					*(LEDptr) = LED2;				//begin by having all LEDs high
						
					if(readSwitches() != 5){		//continue until switches (aka mode) is not 2
						patternnum = 3;				//if we don't want to be in mode 1 anymore, prepare to go to default switch 
						break;
					}
					else if((*(PBptr) & 1) == 1){	//check if pushbutton has been pressed indicating a requested pattern change
						patternnum = (patternnum+1)%3; //update variable expressing desired pattern
						break;
					}
					else{							//case to run pattern 2 as per normal
						LED2 ^= 0xFFFFFFFF;			//toggle LEDs on/off
						volatile int delay = 500000; //delay simply to make the pattern visible
						while(delay){
							delay--;
						}
					}		
				}
				break;
			
			case 2 :								//pattern 3
				while(1){							//continue until button is pressed to switch patterns
						
					*(LEDptr) = LED3;				//begin by having all LEDs low
						
					if(readSwitches() != 5){		//continue until switches (aka mode) is not 2
						patternnum = 3;				//if we don't want to be in mode 1 anymore, prepare to go to default switch 
						break;
					}
					else if((*(PBptr) & 1) == 1){	//check if pushbutton has been pressed indicating a requested pattern change
						patternnum = (patternnum+1)%3; //update variable expressing desired pattern
						break;
					}
					else{							//case to run pattern 3 as per normal
						if(increasing3){			//if in increasing portion of pattern
							if(on3){				//if in turning on portion of the pattern		
								if(currpos3 == 9){	//if we have reached to boundary of the LEDs
									currpos3 = 0;	//reset current position
									on3 = 0;		//change to off segement
								}
								else{				//if we have not reached to boundary of the LEDs
									currpos3++;		//increment current LED position
									LED3 |= (1 << currpos3);//set this position high
								}
							}
							else{					//if in turning off portion of the pattern
								if(currpos3 == 9){	//if we have reached to boundary of the LEDs
									increasing3 = 0;//change to decreasing segement
									on3 = 1;		//change to on segement
								}
								else{				//if we have not reached to boundary of the LEDs
									currpos3++;		//increment current LED position
									LED3 &= ~(1 << currpos3);//clear this position to low
								}
							}
						}
						else{						//if in decreasing portion of pattern
							if(on3){				//if in turning on portion of the pattern
								if(currpos3 == 0){	//if we have reached to boundary of the LEDs
									currpos3 = 9;	//reset current position
									on3 = 0;		//change to off segement
								}
								else{				//if we have not reached to boundary of the LEDs
									currpos3--;		//decrement current LED position
									LED3 |= (1 << currpos3);//set this position to high
								}
							}
							else{					//if in turning off portion of the pattern
								if(currpos3 == 0){	//if we have reached to boundary of the LEDs
									increasing3 = 1;//change to increasing segement
									on3 = 1;		//change to on segement
								}
								else{				//if we have not reached to boundary of the LEDs
									currpos3--;		//decrement current LED position
									LED3 &= ~(1 << currpos3);//clear this position to low 
								}
							}
						}
						
						volatile int delay = 300000; //delay simply to make the pattern visible
						while(delay){
							delay--;
						}
					}		
				}
				break;
			
			default :								//case to exit mode 2 cleanly	
				*(LEDptr) = 0x00000000;				//set to LEDs to low
				proceed = 0;						//set while loop condition to it exits
				break;
		}
	}
}

void modeThree(){
	*(HIGHHEXptr) = 0x0000006D;						//sets first 2 seven segments to 'SY'
	*(LOWHEXptr) = 0x6E6D004F;						//sets last 4 seven segments to 'S 3'
	
	*(LEDptr) = 0xFFFFFFFF;							//start by setting LEDs all high
	while(1){										//continue until switches (aka mode) is not 3	
		if(readSwitches() != 9){					//if we don't want to be in mode 3 anymore 		
			*(LEDptr) = 0x00000000;					//clear LEDs
			break;
		}
		else if(*(AUDIOptr+2) > 500000000){			//case for normal mode 3 functioning, triggers when clap is registered
			*(LEDptr) ^= 0xFFFFFFFF;				//toggles LEDs on/off
			while(*(AUDIOptr+2) > 400000000){}		//waits for clap to finish
		}
	}
}

void chooseDisplay() {
	*(HIGHHEXptr) = 0x00003976;					//sets first 2 seven segments to 'Ch'
	*(LOWHEXptr) = 0x3F3F6D79;					//sets last 4 seven segments to 'oose'
}

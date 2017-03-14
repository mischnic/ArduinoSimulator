#include "main.h"

const int TABLE_OFFSET_Y = 7;
const int TABLE_OFFSET_X = 10;
const int TABLE_SPACING = 6;
const int CONTROLS_Y = 2;
int width = 0;
int height = 0;

extern int pinModes[NUM_DIGITAL_PINS];
extern int pinStates[NUM_DIGITAL_PINS];
extern char errors[NUM_DIGITAL_PINS][30];
extern float vcc;

extern int speedDivisor;

extern bool simulationRunning;
extern bool simulationPaused;
extern bool simulationWaiting;
extern ulong simulationStart;
extern ulong simulationPausedStart;

pthread_t simulationThread;

extern msg serialmsg[SERIALBUFFER_SIZE];


// 	
// 	           ,,                                ,,                  ,,
// 	 .M"""bgd  db                              `7MM           mm     db
// 	,MI    "Y                                    MM           MM
// 	`MMb.    `7MM  `7MMpMMMb.pMMMb.`7MM  `7MM    MM   ,6"Yb.mmMMmm `7MM  ,pW"Wq.`7MMpMMMb.
// 	  `YMMNq.  MM    MM    MM    MM  MM    MM    MM  8)   MM  MM     MM 6W'   `Wb MM    MM
// 	.     `MM  MM    MM    MM    MM  MM    MM    MM   ,pm9MM  MM     MM 8M     M8 MM    MM
// 	Mb     dM  MM    MM    MM    MM  MM    MM    MM  8M   MM  MM     MM YA.   ,A9 MM    MM
// 	P"Ybmmd" .JMML..JMML  JMML  JMML.`Mbod"YML..JMML.`Moo9^Yo.`Mbmo.JMML.`Ybmd9'.JMML  JMML.
// 	
// 	

void startSim(void){
	int err = pthread_create(&simulationThread, NULL, &run, NULL);
	if(err){
		mvprintw(20,0, "Couldn't create simulationThread: %d\n", err);
		return;
	}
	simulationPaused = false;
	simulationStart = realmillis();//millis();
	simulationPausedStart = 0;
	simulationRunning = true;
}

void stopSim(void){
	if(simulationRunning){
		simulationPaused = true;
		pthread_cancel(simulationThread);
		pthread_join (simulationThread, NULL);
		simulationRunning = false;
	}
	init();
}

void pauseSim(){
	simulationPaused = true;
	while(!simulationWaiting){
	}
	refresh();
	simulationPausedStart = realmillis();
}

void unpauseSim(){
	ulong pauseDuration = realmillis() - simulationPausedStart;
	simulationPaused = false;
	// for(int i = 0; i < SERIALBUFFER_SIZE; i++){
	//     serialmsg[i].time += pauseDuration;
	// }
	simulationStart += pauseDuration;
}

void toggleSimPause(){
	if(simulationPaused == false){
		pauseSim();
	} else{
		unpauseSim();
	}
}

void quit(void){
	stopSim();
	endwin();
}


// 	
// 	                               ,,
// 	`7MMF'   `7MF'               `7MM           mm
// 	  MM       M                   MM           MM
// 	  MM       M `7MMpdMAo.   ,M""bMM   ,6"Yb.mmMMmm .gP"Ya
// 	  MM       M   MM   `Wb ,AP    MM  8)   MM  MM  ,M'   Yb
// 	  MM       M   MM    M8 8MI    MM   ,pm9MM  MM  8M""""""
// 	  YM.     ,M   MM   ,AP `Mb    MM  8M   MM  MM  YM.    ,
// 	   `bmmmmd"'   MMbmmd'   `Wbmd"MML.`Moo9^Yo.`Mbmo`Mbmmd'
// 	               MM
// 	             .JMML.


void printDesc(bool init){
	static char descList[20][10];
	// memset(descList[i], 0, 10*sizeof(char));
	Data d;	
	if(init){
		memset(&d, 0, sizeof(Data));
		callGetState(&d, DATA_INFO | 0);
		vcc = d.value / 1000.0f;

		for(int i = 0; i < 20; i++){
			memset(&d, 0, sizeof(Data));
			callGetState(&d, DATA_PIN | i);
			strcpy(descList[i], d.desc);
		}
	}

	attrset(A_DIM);
	for(int i = 0; i < 20; i++){
		if(strlen(descList[i])==0){
			continue;
		}
		int y = TABLE_OFFSET_Y + 1;
		int x = TABLE_OFFSET_X + i * TABLE_SPACING;

		if( i>0 && strlen(descList[i-1]) > (TABLE_SPACING-1) ){
			y += 1;
			// color_set(COLOR_BLUE,0);
			color_set(COLOR_MAGENTA,0);
		} else{
			color_set(COLOR_MAGENTA,0);
		}
		mvprintw(y, x, descList[i]);
	}

	// memset(&d, 0, sizeof(Data));
	// callGetState(&d, 0);
	// int max = d.value + 1;

	for(int i = 1; i < 20; i++){
		memset(&d, 0, sizeof(Data));
		callGetState(&d, DATA_INFO | i);
		if(strcmp(d.desc, "MOTOR") == 0){
			renderMotor(10,30, d.value);
		} else if(strcmp(d.desc, "!!END") == 0){
			break;
		} else {
			color_set(COLOR_WHITE, 0);
			mvaddstr(10+4+2+(i-2), 30, "                     ");
			if(d.desc[0] == '?' && d.desc[1] == 'B'){
				char buffer[16];
				strncpy(buffer, d.desc+2, strlen(d.desc)+2+1);
				mvprintw(10+4+2+(i-2), 30, "%s: %s", buffer, d.value ? "true" : "false");
			} else{
				mvprintw(10+4+2+(i-2), 30, "%s: %i", d.desc, d.value);
			}
		}
	}


	attrset(0);
	color_set(COLOR_WHITE,0);

}


void renderMotor(int startY, int startX, int val){
	for(int y = 0; y < 5; y++){
		move(startY + y, 0); clrtoeol();
	}
	attrset(0);
	int color = COLOR_WHITE;
	int dir = val >> 4 & 0b00000011;
	int state = val & 0b00000011;
	if(state == 0){
		color = COLOR_WHITE;
	} else if(state == 1){
		color = COLOR_RED;
	} else if(state == 2){
		color = COLOR_GREEN;
	}
	color_set(color,0);
	if(dir == 1){
		mvaddstr(startY+0, startX, "   -----  ");
		mvaddstr(startY+1, startX, " /-"); color_set(COLOR_MAGENTA,0); addstr("/--"); color_set(color,0); addstr("---\\");
		mvaddstr(startY+2, startX, "|-"); color_set(COLOR_MAGENTA,0); addstr("|"); color_set(color,0); addstr("--M----|");
		mvaddstr(startY+3, startX, " \\-"); color_set(COLOR_MAGENTA,0); addstr("\\>"); color_set(color,0); addstr("----/");
		mvaddstr(startY+4, startX, "   -----");
	} else if(dir == 0) {
		mvaddstr(startY+0, startX, "   -----  ");
		mvaddstr(startY+1, startX, " /-------\\");
		mvaddstr(startY+2, startX, "|----M----|");
		mvaddstr(startY+3, startX, " \\-------/");
		mvaddstr(startY+4, startX, "   -----");
	} else if(dir == 2){
		mvaddstr(startY+0, startX, "   -----  ");
		mvaddstr(startY+1, startX, " /---"); color_set(COLOR_MAGENTA,0); addstr("--\\"); color_set(color,0); addstr("-\\");
		mvaddstr(startY+2, startX, "|----M--"); color_set(COLOR_MAGENTA,0); addstr("|"); color_set(color,0); addstr("-|");
		mvaddstr(startY+3, startX, " \\----"); color_set(COLOR_MAGENTA,0); addstr("</"); color_set(color,0); addstr("-/");
		mvaddstr(startY+4, startX, "   -----");
	}
}




void update()
{
	int size = NUM_DIGITAL_PINS;
	color_set(COLOR_WHITE,0);
	
	for(int y = -2; y <= 2; y++){
		move(TABLE_OFFSET_Y + y, 0);
		clrtoeol();
	}
	move(CONTROLS_Y, 0);
	clrtoeol();
	move(CONTROLS_Y+1, 0);
	clrtoeol();
	
	standout();
	if(simulationRunning){
		color_set(COLOR_RED,0);
	}
	else {
		color_set(COLOR_GREEN,0);
	}
	
	mvprintw(CONTROLS_Y, 2, (simulationRunning ? "Stop  Sim" : "Start Sim"));
	standend();
	if(simulationRunning){
		if(simulationPaused){
			printw("  Running for %.2f s", ((simulationPausedStart-simulationStart)/1000.0)*
											( (speedDivisor < 0) ? (-speedDivisor) : (1.0f/speedDivisor) )
							);
		} else{
			printw("  Running for %.2f s", ((realmillis()-simulationStart)/1000.0)*
											( (speedDivisor < 0) ? (-speedDivisor) : (1.0f/speedDivisor) )
							);
		}
		if(speedDivisor < 0){
			printw("; Speed=%ix", abs(speedDivisor));
		} else{
			printw("; Speed=1/%ix", speedDivisor);
		}
	} else{
		if(speedDivisor < 0){
			printw("  Speed= %2.ix  ", abs(speedDivisor));
		} else{
			printw("  Speed=1/%2.ix ", speedDivisor);
		}
		standout();
		printw("+");
		standend();
		printw("/");
		standout();
		printw("-");
		standend();
	}
	standout();
	mvprintw(CONTROLS_Y + 1, 2, (simulationRunning ? ( simulationPaused ? "Resume" : "Pause") : "Start Paused"));
	standend();
	
	color_set(COLOR_WHITE,0);

	for(int i = 0; i < size; i++){
		mvprintw(TABLE_OFFSET_Y - 2,  TABLE_OFFSET_X + i * TABLE_SPACING, "%i", i);
	}

	//pinMode
	mvprintw(TABLE_OFFSET_Y -1, TABLE_OFFSET_X - 8, "MODE");
	for(int i = 0; i < size; i++){
		bool error = strlen(errors[i])>0;
		int style = A_BOLD | A_BLINK;
		
		if(error){
			attrset(style);
		} else{
			standend();
		}
		if(pinModes[i] == -1){
			color_set(COLOR_WHITE,0);
			mvaddstr(TABLE_OFFSET_Y - 1, TABLE_OFFSET_X + i * TABLE_SPACING, "UND");
		} else if(pinModes[i] == OUTPUT){
			color_set(COLOR_GREEN,0);
			mvaddstr(TABLE_OFFSET_Y - 1, TABLE_OFFSET_X + i * TABLE_SPACING, "OUT");
		} else if(pinModes[i] == INPUT){
			color_set(COLOR_RED,0);
			mvaddstr(TABLE_OFFSET_Y - 1, TABLE_OFFSET_X + i * TABLE_SPACING, "IN");
		} else if(pinModes[i] == ANALOG){
			color_set(COLOR_YELLOW,0);
			mvaddstr(TABLE_OFFSET_Y - 1, TABLE_OFFSET_X + i * TABLE_SPACING, "ANL");
		} else if(pinModes[i] == SERIAL){
			// attrset(A_ITALIC);
			color_set(COLOR_YELLOW, 0);
			mvaddstr(TABLE_OFFSET_Y - 1, TABLE_OFFSET_X + i * TABLE_SPACING, "SER");
		}
	}

	//pinState
	color_set(COLOR_WHITE,0);
	mvprintw(TABLE_OFFSET_Y, TABLE_OFFSET_X - 8, "STATE");
	standout();
	for(int i = 0; i < size; i++){
		
		color_set(COLOR_YELLOW,0);
		
		if(pinModes[i] == ANALOG){
			standout();
			color_set(COLOR_YELLOW,0);
			mvprintw(TABLE_OFFSET_Y , TABLE_OFFSET_X + i * TABLE_SPACING, "%.2fV", pinStates[i]*(vcc/1024));
//            mvprintw(TABLE_OFFSET_Y , TABLE_OFFSET_X + i * TABLE_SPACING, "%4i", pinStates[i]);
		} else{
			if(pinModes[i] == -1 || pinModes[i] == SERIAL) standend();
			else standout();

			if(pinStates[i] == HIGH){
				color_set(COLOR_RED,0);
				mvaddstr(TABLE_OFFSET_Y , TABLE_OFFSET_X + i * TABLE_SPACING, "HIGH");
			} else{
				color_set(COLOR_GREEN,0);
				mvaddstr(TABLE_OFFSET_Y , TABLE_OFFSET_X + i * TABLE_SPACING, "LOW");
			}
		}
	}
	standend();
	color_set(COLOR_WHITE,0);

	mvprintw(TABLE_OFFSET_Y +1, TABLE_OFFSET_X - 8, "DESC");
	printDesc(false);

	int x = width/3;
	for(int i = 1; i < SERIALBUFFER_SIZE; i++){
		msg m = serialmsg[i];
		int y = TABLE_OFFSET_Y + 2 + (SERIALBUFFER_SIZE-i);
		move(y, x); clrtoeol();
		if(strlen(m.payload) != 0){
			if(m.payload[0] == '-' && m.payload[1] == '-' && m.payload[2] == '-'){
				if(m.payload[3] == '-'){
					if(m.payload[4] == '-'){
						color_set(COLOR_RED,0);
					} else{
						color_set(COLOR_MAGENTA,0);
					}
				}
				else{
					color_set(COLOR_GREEN,0);
				}
			}
			
			//mvprintw(y, x, "%5.ims: %s", (m.time == 0) ? 0 : m.time - simulationStart, m.payload);
			mvprintw(y, x, "%3.3fs: %s", ((double)m.time) / 1000.0 , m.payload);
			color_set(COLOR_WHITE,0);
		}
	}

	int num = 0;
	for(int i = 0; i < NUM_DIGITAL_PINS; i++){
		if(strlen(errors[i]) != 0){
			mvprintw(TABLE_OFFSET_Y + 4 + num++, 1, "%i, %s", i, (char*) errors[i]);
		}
	}


	refresh();
}

// 	
// 	                           ,,
// 	`7MMM.     ,MMF'           db
// 	  MMMb    dPMM
// 	  M YM   ,M MM   ,6"Yb.  `7MM  `7MMpMMMb.
// 	  M  Mb  M' MM  8)   MM    MM    MM    MM
// 	  M  YM.P'  MM   ,pm9MM    MM    MM    MM
// 	  M  `YM'   MM  8M   MM    MM    MM    MM
// 	.JML. `'  .JMML.`Moo9^Yo..JMML..JMML  JMML.
// 	
// 	

int main(void)
{    
	initscr();
	atexit(quit);
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	mousemask(BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED, NULL);
	getmaxyx(stdscr, height, width);
	start_color();
	init_pair(COLOR_WHITE,  COLOR_WHITE,   COLOR_BLACK);
	init_pair(COLOR_RED,    COLOR_RED,     COLOR_BLACK);
	init_pair(COLOR_GREEN,  COLOR_GREEN,   COLOR_BLACK);
	init_pair(COLOR_YELLOW, COLOR_YELLOW,  COLOR_BLACK);
	init_pair(COLOR_BLUE,   COLOR_BLUE,    COLOR_BLACK);
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
	if(width < (TABLE_OFFSET_X + TABLE_SPACING * NUM_DIGITAL_PINS)){
		color_set(COLOR_RED,0);
		mvprintw(0,0, "SCREEN TOO NARROW!");
		refresh();
		color_set(COLOR_WHITE,0);
	}

	init();

	printDesc(true);

	update();
	color_set(COLOR_WHITE,0);

	MEVENT mev;
	
	static struct timespec tim;
	tim.tv_sec  = 0;
	tim.tv_nsec = 3 * 1000000L;
	
	ulong lastUpdate = 0;

	while(1){
		nanosleep(&tim, NULL);
		if(kbhit()){
			int ch = getch();
			if(ch == KEY_MOUSE){ 
				if(getmouse(&mev) == OK){
					int mx = mev.x;
					int my = mev.y;
					
					if(my == TABLE_OFFSET_Y){
						int i = (mx - TABLE_OFFSET_X)/TABLE_SPACING;
						if(pinModes[i] == ANALOG){
							bool paused = false;
							if(simulationRunning == true &&simulationPaused == false){
								pauseSim();
								paused = true;
							}

							echo();


							mvaddstr(TABLE_OFFSET_Y, TABLE_OFFSET_X + i * TABLE_SPACING, "     ");
							char buffer[6] = "";
							mvgetnstr(TABLE_OFFSET_Y, TABLE_OFFSET_X + i * TABLE_SPACING, buffer, 5);
							char lastChar = buffer[strlen(buffer)-1];
							long value = 0;
							if(lastChar == 'V' || lastChar == 'v'){
								char substr[5] = "";
								strncpy(substr, buffer, strlen(buffer)-1);
								float f = strtof(substr, NULL);
								value = f * (1023.0f/vcc);
							} else{
								for(int i = 0; i < strlen(buffer); i++){
									if(buffer[i] == '.'){
										float f = strtof(buffer, NULL);
										value = f * 1023.0f;
										break;
									} else if(i==strlen(buffer)-1){
										value = strtol(buffer, NULL, 10);
									}
								}
							}
							
							if(value < 1024){
								pinStates[i] = (int)value;
							}


							noecho();

							if(paused){
								unpauseSim();
							}
						} else if(pinModes[i] != -1 && pinModes[i] != SERIAL){
							pinStates[i] = !pinStates[i];
						}
					} else if(my == CONTROLS_Y){
						if(mx >= 2 && mx <= 10){
							if(!simulationRunning) {
								startSim();
							} else{
								stopSim();
							}
						} else if(mx >= 25 && mx <= 27){
							if(mev.bstate & BUTTON1_DOUBLE_CLICKED){
								if(mx == 25){
									if(speedDivisor == 2){
										speedDivisor = -1;
									} else if(speedDivisor > 0){
										speedDivisor /=2;
									} else{
										speedDivisor *=2;
									}
								} else if(mx == 27){
									if(speedDivisor == -1){
										speedDivisor = 2;
									} else if(speedDivisor > 0){
										speedDivisor *=2;
									} else{
										speedDivisor /=2;
									}
								}
							}else{
								if(mx == 25){
									if(speedDivisor == 2){
										speedDivisor = -1;
									}else{
										speedDivisor--;
									}
								} else if(mx == 27){
									if(speedDivisor == -1){
										speedDivisor = 2;
									}else{ // 1/2x
										speedDivisor++;
									}
								}
							}
						}
					} else if(my == CONTROLS_Y + 1){
						if(mx >= 2 && my <= 7){
							if(simulationRunning){
								toggleSimPause();
							} else {
								startSim();
								pauseSim();
							}
						}
					}
				}
			}else if(ch == 'w'){
				if(simulationRunning) {
					stopSim();
				} else{
					startSim();
				}
			} else if(ch == 'd'){
				if(!simulationRunning) {
					startSim();
					pauseSim();
				}
			} else if(ch == 'e'){
				if(simulationRunning) {
					toggleSimPause();
				}
			} else if(ch == 'q'){
				quit();
				exit(0);
			}
		}

		if((realmillis() - lastUpdate) > 20){
			lastUpdate = realmillis();
			update();
		}
		
	}

	return 0;
}
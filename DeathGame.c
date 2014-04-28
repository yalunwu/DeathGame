#include "applications/application.h"
#include "protocol/protocol.h"
#include "drivers/light.h"
#include "drivers/sensors.h"
#include "applications/game.h"
#include "drivers/timer.h"

static void init(void);
static void update(void);
static void packet_listener(frame *frm);
application DeathGame = {init, update, packet_listener};

#define SPAWN_INTERVAL 1000
#define SPAWN_P_THRESHOLD 0x2F
#define SEND_INTERVAL 500


static uint8_t dummy;
static timer spawnTimer;
static timer sendTimer;
static timer initTimer;
static bool isCorner = false;
static bool hasColor = false;
static uint8_t recievedRed = 0;
static uint8_t recievedGreen = 0;
static uint8_t recievedBlue = 0;
static uint8_t timerCounter = 0;

#define InitPacket 		0
#define SetBase			1
#define UpdateColor		2
#define Alive			3
#define requestTurnOff	4
#define requestTurnOn	5
typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t type;
}packet_format;


static void init(void) {
	game_init(sizeof(struct packet_format));
	set_all_leds(LIGHT_OFF);
	timer_reset(&spawnTimer);
	timer_reset(&sendTimer);
	timer_reset(&initTimer);
}

static void update(void) {
	//each tile should check if it is pressed on or not.....
	//if the tile is corner, color should never turn off and should tick off
	//if the tile has a color it should turn off and send a message to master tile 
	if(initialized == true){
		if (is_master())
		{
			//chose where to send the tile here and keep track where to send it. 
		}
		else if (isCorner == true)
		{
			// do spawn timer here
		}else{
			if (fsr_update() && hasColor == true )
			{
				set_all_leds(0,0,0);
			}else{
				set_all_leds(recievedRed,recievedGreen,recievedBlue);
			}
		}
	}else{
		// do detection for topology here
	}
}

static void packet_listener(frame *frm) {
	if(is_source) return;
	struct packet_format *data;
	data = (struct packet_format *) frm->info;
	if( data->destination_address == ){//need my address here
		if (data->type == )
		{
			/* code */
		}
	}


}

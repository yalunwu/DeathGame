#include "applications/application.h"
#include "protocol/protocol.h"
#include "drivers/light.h"
#include "drivers/sensors.h"
#include "applications/game.h"
#include "drivers/timer.h"
#include "protocol/topology.h"

static void init(void);
static void update(void);
static void packet_listener(frame *frm);
application DeathGame = {init, update, packet_listener};

#define SPAWN_INTERVAL 1000
#define SPAWN_P_THRESHOLD 0x2F
#define SEND_INTERVAL 500

static uint8_t packetData=999;

static uint8_t dummy;
static timer spawnTimer;
static timer sendTimer;
static timer initTimer;
static bool isCorner 	= false;
static bool hasColor 	= false;
static bool enabled 	= true;
static uint8_t recievedRed = 0;
static uint8_t recievedGreen = 0;
static uint8_t recievedBlue = 0;
static uint8_t timerCounter = 0;
static uint8_t numberOfTiles = 0;
static uint16_t packetSource = 0;
#define InitPacket 		0
#define SetBase			1
#define Alive			3
#define requestTurnOff	4
#define requestTurnOn	5
#define requestSpawn	6
struct packet_format {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t type;
}pac = {.r = 0, .g = 0, .b = 0, .type = 0};


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
	//code assumers masterpiece is at a corner 
	if(initialized && enabled){
		if (is_master())
		{
			if (packetData == requestSpawn){
				//find a none used address

				//send packet to turn a chip on
				
				//update table  of tile

			}else if(packetData ==requestTurnOff){
				//update table of tiles 
				//packetSource shows where the packet is from 
			}
			if (timer_compare(&sendTimer, 3500)
			{
				timer_reset(&sendTimer);
				set_one_led(0,8,0,timerCounter);
				timerCounter = timerCounter + 1;
			}
			if(timerCounter == 8){
				timerCounter = 0;
				set_all_leds(0,0,0);

			}

		}
		else if (isCorner )
		{
			if (timer_compare(&sendTimer, 3500)
			{
				
				timer_reset(&sendTimer);
				set_one_led(0,8,0,timerCounter);
				timerCounter = timerCounter + 1;
			}
			if(timerCounter == 8){
				timerCounter = 0;
				set_all_leds(0,0,0);
				//send a packet to master saying time to spawn timer
				pac.type = requestSpawn;
				send_command(my_master_addr(),&pac);
			}

		}else{// if i am not a corner piece or the master tile

			if (fsr_update() && hasColor  )
			{
				set_all_leds(0,0,0);
				hasColor = false;
				pac.type = requestTurnOff;
				send_command(my_master_addr(),&pac);

			}
			if (packetData == requestTurnOn)
			{
				set_all_leds(recievedRed,recievedGreen,recievedBlue);
				hasColor = true;
			}
		}

	}else{
		// do detection for topology here
		if(is_master()&&timer_compare(&initTimer, 3500)){
				timer_reset(&spawnTimer);
				timer_reset(&sendTimer);
				timer_reset(&initTimer);
				initialized = true;
				//lets find the other corner and the whole list of tiles
				numberOfTiles = TOP_calc_size_of_platform();
				if (numberOfTiles<9)
				{
					enabled = false; 
				}else{
					// do detection of the board here
				}
		}

	}
	//what happens if there is not enough tiles
	if (enabled == false)
	{
		set_all_leds(8,0,0);
		pac.type 	= requestTurnOn;
		pac.r 		= 8;
		pac.g 		= 0;
		pac.b 		= 0;
		send_command_to_all(&pac);
	}
	//need to flush data to remove problems
	packetData = 999;
}

static void packet_listener(frame *frm) {
	if(is_source) return;
	struct packet_format *data;
	data = (struct packet_format *) frm->info;
	if( frm->destination_address == my_address){//need my address here
		packetData 		= 	data->type;
		recievedBlue 	=	data->b; 
		recievedGreen 	=	data->g;
		recievedRed 	=	data->r;
		packetSource 	= 	frm->source_address;
	}


}

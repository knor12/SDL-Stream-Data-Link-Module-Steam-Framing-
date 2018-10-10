#include "SDL.h"

/*
* Author Nkessa
* Do whatever yu want with this code, on your own risk :)
* This SDL module stands for Steam Data Link, the purpose of the module is to provide a way of packaging a steam on data into 
* packets to facilitate the sending of packets on a physical medium that only supports a serial data transmission, such RS232 
* blue-tooth etc.
* 
* Please note this code in re-entrance. feel free to make it re-entrant by encapsulating variables in a structure.
* 
* Rules for packets:
* 1)Every packet starts with STO byte.
* 2)Every packet ends with EOT
* 3)If a byte in the data being sent happen to equal either STO,EOT or ESC it is preceded by ESC (stuffing) 
* 4)Data integrity check such as CRC and packet length is not implemented in this version.
* 
 */

void SDL_init(){
	/*initialization code goes here*/

}


SDL_Return_t SDL_PackageFrame(const uint8_t * data,const uint8_t data_length ,uint8_t * frame,  uint8_t * frame_length ){

	const uint8_t * ptrData = data;
	uint8_t * ptrFrame = frame;	

	/*insert SOT at the beginning of the frame*/
	* ptrFrame = SOT;
	ptrFrame++;

	while(ptrData < data + data_length){
		/*TODO check if the frame buffer is not too small*/

		/*check if we have a bit pattern that requires an escape sequence*/
		if ((*ptrData)==SOT ||(*ptrData)==EOT ||(*ptrData)==ESC){
			* ptrFrame = ESC;
			ptrFrame++;	
		}
		* ptrFrame = * ptrData;
		ptrFrame++;
		ptrData++;
	}

	/*insert EOT at the end of the frame*/
	* ptrFrame = EOT;
	ptrFrame++;

	/*calculate the frame size*/
	*frame_length = ptrFrame -frame ;

	return SDL_OK;
}

typedef enum {
	SDL_Unpackage_State_Init =0,
	SDL_Unpackage_State_Data,
	SDL_Unpackage_State_Escape,
	SDL_Unpackage_State_Done,
	SDL_Unpackage_State_Error

}SDL_Unpackage_State;


SDL_Return_t SDL_UnpackageFrame(uint8_t * data,  uint8_t * data_length ,const uint8_t * frame,const uint8_t frame_length ){
	/*
	This function implements the state machine below.

	                      +-----------+
                          |   init    |
                          +---+-------+
                              |
                              |
                              |   SOT
                              |
                              |
                      +-------v-------+
       ESC            |     DATA      |
     +----------------+               +-------------------------+
     |                +^---+-------+--+                         |  EOT
     |                 |   ^       |                            |
     |                 |   |       |                            |
     |                 |   |       |                       +----v-------+
     |                 |   +-------+                       |            |
     |                 |    ACCEPT DATA                    |   DONE     |
     |                 |                                   |            |
+----v----------+      |                                   +------------+
|               +------+
|     ESCAPE    | ACCEPT DATA
+---------------+



	 */

	uint8_t * ptrData = data;
	const uint8_t * ptrFrame = frame;	
	uint16_t lenFrame =frame_length;
	SDL_Unpackage_State SDL_Unpackage_State_Current = SDL_Unpackage_State_Init ;

	/*The frame needs to start with SOT*/
	if ((* ptrFrame )!=SOT){
		return SDL_ERROR;
	}else{
		printf("processing SOT \n");
		ptrFrame++;
		SDL_Unpackage_State_Current=SDL_Unpackage_State_Data;
	}

	while((ptrFrame <= lenFrame+frame)&& (SDL_Unpackage_State_Current !=SDL_Unpackage_State_Done ) && (SDL_Unpackage_State_Current !=SDL_Unpackage_State_Error )){

		switch (SDL_Unpackage_State_Current){

		case SDL_Unpackage_State_Data:
			/*check if we have an escape character*/
			if ((* ptrFrame )==ESC){
				ptrFrame++;
				SDL_Unpackage_State_Current=SDL_Unpackage_State_Escape;

				/*check if we reached the end of the packet*/	
			}else if ((* ptrFrame )==EOT){
				//printf("processing EOT \n");
				/*set the state machine to done*/
				SDL_Unpackage_State_Current =SDL_Unpackage_State_Done;

				/*calculate how much data was extracted from the frame*/
				* data_length = ptrData - data;

				/*this is just data, process it*/	
			}else {
				/*check if the data buffer for overflow*/
				/*TODO*/ 
				//printf("processing %x  \n" , * ptrFrame);
				* ptrData =* ptrFrame ;
				ptrData++;
				ptrFrame++;

			}

			continue;

		case SDL_Unpackage_State_Escape:
			//printf("ESC processing %x  \n" , * ptrFrame);
			* ptrData =* ptrFrame ;
			ptrData++;
			ptrFrame++;
			SDL_Unpackage_State_Current=SDL_Unpackage_State_Data;

			continue ;
		case 	SDL_Unpackage_State_Done:
			break;
		case    SDL_Unpackage_State_Error:
			break ;
		default: 
			SDL_Unpackage_State_Current =SDL_Unpackage_State_Error ; 
			break ;
		}
	}
	if (SDL_Unpackage_State_Current ==SDL_Unpackage_State_Done){
		return SDL_OK;
	}else {
		return SDL_ERROR;
	}	

}
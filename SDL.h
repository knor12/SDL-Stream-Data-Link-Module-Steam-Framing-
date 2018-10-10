#include "SDL_cfg.h"


typedef enum {
	SDL_OK =0,
	SDL_ERROR =1,
	SDL_PACKET_WRONG_START,

} SDL_Return_t;

void SDL_init();


/*the frame buffer needs to be at least 2*data_length+2 to guarantee no buffer overflow */
SDL_Return_t SDL_PackageFrame(const uint8_t * data,const uint8_t data_length ,uint8_t * frame,  uint8_t * frame_length );


/*the data buffer needs to be at least as big as frame_length-2 to avoid buffer overflow*/
SDL_Return_t SDL_UnpackageFrame(uint8_t * data,  uint8_t * data_length ,const uint8_t * frame,const uint8_t frame_length );
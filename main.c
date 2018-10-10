#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "SDL.h"


static void printArray(uint8_t  * buff , uint16_t buff_len ){

	uint8_t * b = buff ;

	while(b < buff+buff_len){
		if (*b==SOT){
			printf("-SOT" );

		}else if (*b==EOT){
			printf("-EOT");

		}else if (*b==ESC){
			printf("-ESC");

		}else{
			printf("-0x%X" ,  * b );
		}
		b++;
	}

	printf("\n");

}


int main (int argc , char * argv[]){

	unsigned char     test_data[][10]={  
			{0x10 , 0x11 , 0x20, SOT , 0x30,0x41},
			{0x10 , 0x11 , 0x20, ESC , 0x30,0x41},
			{0x10 , 0x11 , 0x20, EOT , 0x30,0x41},
			{SOT , 0x11 , 0x20, 0x00 , 0x30,0x41},
			{ESC , 0x11 , 0x20, 0x00 , 0x30,0x41},
			{EOT , 0x11 , 0x20, 0x00 , 0x30,0x41},
			{0x10 , 0x11 , 0x20, 0x00 , 0x30,SOT},
			{0x10 , 0x11 , 0x20, 0x00 , 0x30,ESC},
			{0x10 , 0x11 , 0x20, 0x00 , 0x30,EOT},
			{0x10 , 0x11 , 0x20, 0x00 , ESC,EOT},
			{0x10 , 0x11 , 0x20, ESC , ESC,EOT}

	};

	uint8_t data_out[10];
	uint8_t frame[10*2+2];
	uint8_t frame_length=0;
	uint8_t data_length=0;	
	SDL_Return_t ret ;

	uint8_t i =0;

	for (i=0;i<11;i++){
		printf("\n\n");
		/*print data before processing*/
		printf("Data to be sent \n");
		printArray(&test_data[i][0] , 6);

		/*pack the frame*/	
		ret =SDL_PackageFrame(&test_data[i][0],6 ,&frame[0],&frame_length );

		/*print frame */
		printf("frame packed error code %d \n" , ret);
		printArray(&frame[0] , frame_length);

		/*unpack the frame*/
		ret = SDL_UnpackageFrame(&data_out[0],  &data_length ,&frame[0],frame_length );

		/*print data extracted */
		printf("data unpacked  error code %d \n" , ret);	
		printArray(&data_out[0] , data_length);
	}



	return 0;
}
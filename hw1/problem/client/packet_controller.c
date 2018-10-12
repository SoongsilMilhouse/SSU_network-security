
#include "packet_controller.h"
#include "packet.h"
#include "codec.h"
#include <string.h>

int Packet_Handler(unsigned char *src, unsigned char **dst, int msgType, int *dst_len)
{
	LOGIN_ACK *loginAck;
	IMG_ACK *imgAck;
	unsigned char *imgBuf;
	FILE *fp;
	int rv;

	switch(msgType){
	case MT_LOGIN_ACK:
		printf("\nClient received LOGIN_ACK packet.\n");
		decode_LoginAck(src, &loginAck);
		printf("\nLogin Result Msg: %s\n", loginAck->res_msg);
		free(loginAck);
		rv = 0;
		break;
	case MT_IMG_ACK:
		printf("\nClient received IMG_ACK packet.\n");
		decode_ImgAck(src, &imgAck);
		printf("Img Result Msg: %s\n", imgAck->res_msg);
		free(imgAck);
		rv = 0;

	default:
		rv = -1;
		break;
	}

	return rv;
}

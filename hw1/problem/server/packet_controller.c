
#include "packet_controller.h"
#include "packet.h"
#include "codec.h"
#include <string.h>

static const char *ID = "csec";
static const char *PWD = "1q2w3e4r";

int Packet_Handler(unsigned char *src, unsigned char **dst, int msgType, size_t* dst_len)
{
	LOGIN_REQ *loginReq;
	LOGIN_ACK *loginAck;
	IMG_SEND *imgSend;
	IMG_ACK *imgAck;

	char *id ,*passwd;
	int rv;
	FILE *fp;

	int imgLength;
	unsigned char *img;

	switch(msgType){
	case MT_LOGIN_REQ:
		printf("\nLogin Request received\n");
		decode_LoginReq(src, &loginReq); 	//Decoding Message
		id = loginReq->id;
		passwd = loginReq->passwd;
		//Checking ID/PWD and Generating Packet Message
		if(strcmp(id, ID) != 0 || strcmp(passwd, PWD) != 0)
		{
			char *res_msg = "Login Fail!";
			loginAck = (LOGIN_ACK *)calloc(1, sizeof(LOGIN_ACK));
			loginAck->result = R_FAIL;
			strncpy(loginAck->res_msg, res_msg, strlen(res_msg));
		}
		else
		{
			char *res_msg = "Login Success!";
			loginAck = (LOGIN_ACK *)calloc(1, sizeof(LOGIN_ACK));
			loginAck->result = R_SUCCESS;
			strncpy(loginAck->res_msg, res_msg, strlen(res_msg));
		}
		//Encoding Packet
		*dst_len = encode_packet(MT_LOGIN_ACK, (void *)loginAck, dst);
		printf("Sending... Login Ack Message to client\n");
		free(loginReq); free(loginAck);
		rv = 0;
		break;
	case MT_IMG_SEND:
		printf("\nImg Send recevied\n");
		decode_ImgSend(src, &imgSend);	//Decoding Image

		fp = fopen("image.jpg", "wb");
		//fp = open("image.jpg", O_RDWR|O_CREAT|O_TRUNC);
		fwrite(imgSend->img, sizeof(unsigned char), imgSend->imgLength, fp);
		//write(fp, img, imgLen);
		fclose(fp);

		char *res_msg = "IMG SEND Success!";
		imgAck = (IMG_ACK*)calloc(1, sizeof(IMG_ACK));
		imgAck->imgResult = R_SUCCESS;
		strncpy(imgAck->res_msg, res_msg, strlen(res_msg));

		//Encoding Packet
		*dst_len = encode_packet(MT_IMG_ACK, (void *)imgAck, dst);
		printf("Sending... Image Ack Message to client\n");
		free(imgSend); free(imgAck);
		rv = 0;
		break;
	default:
		rv = -1;
		break;
	}

	return rv;
}


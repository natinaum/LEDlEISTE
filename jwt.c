#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"deps/hmac-sha256/hmac-sha256.h"
#include"deps/sha256/sha256.h"
#define HMAC_SHA256_DIGEST_SIZE 32
//EXEMPLARISCHER AUFBAU
char header[40]={0};
char payload[70]={0};
char unsigniert[200]={0};
char withSecret[300]={0};
char final[500]={0};
char String[45]={0};
char secret[]="test";

char * table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=";
char * output;
const char * base64_encode(char* input,unsigned length){
output=malloc(800);
int j=0;
int padding=strlen(input)%3;
if(padding==2){padding=1;}
else if(padding==1){padding=2;}
int i;
if(length==0) length=strlen(input);
for(i=0;i<length-3;i=i+3){
	//if(strlen(input)<=3){
	//	break;
	//}
	output[j]=table[(input[i] & 0b11111100)>>2];
	j++;
	output[j]=table[((input[i] & 0b00000011)<<4)|((input[i+1]&0b11110000)>>4)];
	j++;
	output[j]=table[((input[i+1]&0b00001111)<<2)|((input[i+2]&0b11000000)>>6)];
	j++;
	output[j]=table[input[i+2]&0b00111111];
	j++;
}
switch(padding){
	case 1:
		output[j]=table[(input[i] & 0b11111100)>>2];
		j++;
		output[j]=table[((input[i] & 0b00000011)<<4)|((input[i+1]&0b11110000)>>4)];
		j++;
		output[j]=table[((input[i+1]&0b00001111)<<2)];

		//output[j]='%';
		//j++;
		//output[j]='3';
		//j++;
		//output[j]='D';
		break;
	
	case 2:
		output[j]=table[(input[i] & 0b11111100)>>2];
		j++;
		output[j]=table[((input[i] & 0b00000011)<<4)];

		//output[j]='%';
		//j++;
		//output[j]='3';
		//j++;
		//output[j]='D';
		//j++;

		//output[j]='%';
		//j++;
		//output[j]='3';
		//j++;
		//output[j]='D';

		break;
	case 0:
		output[j]=table[(input[i] & 0b11111100)>>2];
		j++;
		output[j]=table[((input[i] & 0b00000011)<<4)|((input[i+1]&0b11110000)>>4)];
		j++;
		output[j]=table[((input[i+1]&0b00001111)<<2)|((input[i+2]&0b11000000)>>6)];
		j++;
		output[j]=table[input[i+2]&0b00111111];
		break;
}
j++;
output[j]=0;
return output;
}


const char* generateJWT(char String[]){
sprintf(header,  "{\"alg\": \"HS256\", \"typ\": \"JWT\"}");
sprintf(payload, "{\"SRGB-HEX-STRING\": \"%s\"}",String);
sprintf(unsigniert, "%s.%s", base64_encode(header,0), base64_encode(payload,0));
printf("Shit i work with: %s\n", String);
sprintf(withSecret, "%s%s", unsigniert,secret) ;

printf("unsigniert: %s\n",unsigniert);
printf("secret: %s\n",secret);

unsigned char hash[HMAC_SHA256_DIGEST_SIZE];
hmac_sha256(hash,unsigniert,strlen(unsigniert), secret,strlen(secret));

sprintf(final, "%s.%s", unsigniert, base64_encode(hash,HMAC_SHA256_DIGEST_SIZE));
final[strlen(final)]=0;
return final;
}

//int main(int nargs, char ** cargs){
//printf("%s\n",generateJWT(cargs[1]));
//}

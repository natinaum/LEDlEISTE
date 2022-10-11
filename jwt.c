//EXEMPLARISCHER AUFBAU
char header[40]={0};
char payload[70]={0};
char unsigniert[200]={0};
char withSecret[300]={0};
char final[500]={0};
char String[45]={0};
char secret[]="das Geheimnis muss geheim sein";
#include <openssl/sha.h>

char[] hash(char input){

}

char * table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
char * output;
char * base64_encode(char* input){
	int j=0;
	int padding=strlen(input)%3;
	if(padding==2){padding=1;}
	else if(padding==1){padding=2;}
	int i;
	for(i=0;i<strlen(input)-3;i=i+3){
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
			j++;
			output[j]='=';
			break;
		
		case 2:
			output[j]=table[(input[i] & 0b11111100)>>2];
			j++;
			output[j]=table[((input[i] & 0b00000011)<<4)];
			j++;
			output[j]='=';
			j++;
			output[j]='=';
			break;
		case 0:
			output[j]=table[(input[i] & 0b11111100)>>2];
			j++;
			output[j]=table[((input[i] & 0b00000011)<<4)|((input[i+1]&0b11110000)>>4)];
			j++;
			output[j]=table[((input[i+1]&0b00001111)<<2)|((input[i+2]&0b11000000)>>6)];
			j++;
			output[j]=table[input[i+2]&0b00111111];
			printf("\n");
			break;
	}
	j++;
	output[j]=0;
	return output;
}

	
char[] generateJWT(char[] String){
	sprintf(header,  "pr{\"alg\": \"HS512\", \"typ\": \"JWT\"}");
	sprintf(payload, "{\"SRGB-HEX-STRING\": %s}",String);
	sprintf(unsigniert, "%s.%s", base64_encode(header), base64_encode(payload));

	sprintf(withSecret, "%s%s", unsigniert,secret) ;

	char hash[SHA512_DIGEST_LENGTH];
	SHA512(unsigniert, sizeof(unsigniert) - 1, hash);

	sprintf(final, "%s.%s", unsigniert, base64_encode(hash));
	return final;
}

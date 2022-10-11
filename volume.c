#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SECTIONS 7
#define PORT 4210
#define SA struct sockaddr


#include <unistd.h>
#include <fcntl.h>

#define VOLUME_BOUND 140
#ifdef OSSCONTROL
#define MIXER_DEV "/dev/dsp"

#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <stdio.h>
#else
#include <alsa/asoundlib.h>
#endif
static int MAX = SECTIONS*6;
 
struct sockaddr_in servaddr;



/*
  Drawbacks. Sets volume on both channels but gets volume on one. Can be easily adapted.
 */
//int audio_volume(long* outvol)
int audio_volume()
{
    long vol = -1;
    long* outvol= &vol;
    int ret = 0;
    int fd, devs;

    snd_mixer_t* handle;
    snd_mixer_elem_t* elem;
    snd_mixer_selem_id_t* sid;

    static const char* mix_name = "Master";
    static const char* card = "default";
    static int mix_index = 0;

    long pmin, pmax;
    long get_vol, set_vol;
    float f_multi;

    snd_mixer_selem_id_alloca(&sid);

    //sets simple-mixer index and name
    snd_mixer_selem_id_set_index(sid, mix_index);
    snd_mixer_selem_id_set_name(sid, mix_name);

        if ((snd_mixer_open(&handle, 0)) < 0)
        return -1;
    if ((snd_mixer_attach(handle, card)) < 0) {
        snd_mixer_close(handle);
        return -2;
    }
    if ((snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
        snd_mixer_close(handle);
        return -3;
    }
    ret = snd_mixer_load(handle);
    if (ret < 0) {
        snd_mixer_close(handle);
        return -4;
    }
    elem = snd_mixer_find_selem(handle, sid);
    if (!elem) {
        snd_mixer_close(handle);
        return -5;
    }

    long minv, maxv;

    snd_mixer_selem_get_playback_volume_range (elem, &minv, &maxv);
    fprintf(stderr, "Volume range <%i,%i>\n", minv, maxv);


        if(snd_mixer_selem_get_playback_volume(elem, 0, outvol) < 0) {
            snd_mixer_close(handle);
            return -6;
        }

        fprintf(stderr, "Get volume %i with status %i\n", *outvol, ret);
        /* make the value bound to 100 */
        *outvol -= minv;
        maxv -= minv;
        minv = 0;
        *outvol = 100 * (*outvol) / maxv; // make the value bound from 0 to 100
        snd_mixer_close(handle);
    return *outvol;
    }

int main(void)
{

	int sockfd, connfd, len;
	// socket create and verification
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
	    printf("socket creation failed...\n");
	    exit(0);
	}
	else
	    printf("Socket successfully created..\n");
	//bzero(&servaddr, sizeof(servaddr));
	// assign IP, PORT
	printf("Server Adresse festlegen\n");
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr("192.168.2.11");


    long vol=audio_volume()*7/100;
    printf("Master volume is %li\n", vol);
     printf("Stripe nullen\n");
     char zahl[]="000000000000000000000000000000000000000000";
     sendto(sockfd, zahl, (strlen(zahl)+1), 0,
 (struct sockaddr *)&servaddr, sizeof(servaddr)) ;
     printf("Stripe genullt... theoretisch\n");
     _Bool count[]={0,0,0,0,0,0,0};
     int tmp;
     char zahlen[][6]={"000000","ffffff","ff0000","00ff00","0000ff"};
     int h=1;
     while(1){
    	long vol=audio_volume()*7/100;
	for(int i = 0; i<7;i++){
		if(i<=vol){
			count[i]=1;
		}else{
			count[i]=0;
		}
	}
        for(int k=0;k<42;k++){
                  zahl[k]=zahlen[(count[k/6]+h)%5][k%6];
        }
        sendto(sockfd, zahl, (strlen(zahl)+1), 0,
        (struct sockaddr *)&servaddr, sizeof(servaddr)) ;
        printf("Schicke: %s\n", zahl);
        sleep(1);
     }


}

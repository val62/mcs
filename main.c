/* 
 * File:   main.c
 * Author: valentin
 *
 * Created on 19 novembre 2012, 08:24
 */
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

static char *device = "default"; /* playback device */
snd_output_t *output = NULL;
unsigned char buffer[16 * 1024]; /* some random data */

/*
int main(void) {
    int err;
    unsigned int i;
    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    for (i = 0; i < sizeof (buffer); i++)
        buffer[i] = random() & 0xff;
    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = snd_pcm_set_params(handle,
            SND_PCM_FORMAT_U8,
            SND_PCM_ACCESS_RW_INTERLEAVED,
            1,
            48000,
            1,
            500000)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < 16; i++) {
        frames = snd_pcm_writei(handle, buffer, sizeof (buffer));
        if (frames < 0)
            frames = snd_pcm_recover(handle, frames, 0);
        if (frames < 0) {
            printf("snd_pcm_writei failed: %s\n", snd_strerror(err));
            break;
        }
        if (frames > 0 && frames < (long) sizeof (buffer))
            printf("Short write (expected %li, wrote %li)\n", (long) sizeof (buffer), frames);
    }
    snd_pcm_close(handle);
    return 0;
}

 */
//Retourn le socket
int init_socket(...);
//
int init_sound(snd_pcm_t *handle_read,snd_pcm_t *handle_write);


int main(void) {
    int err;
    unsigned int i;
    snd_pcm_t *handle_read;
    snd_pcm_t *handle_write;
    snd_pcm_sframes_t frames;

    //Config recorder
    if ((err = snd_pcm_open(&handle_read, device, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        printf("Capture open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = snd_pcm_set_params(handle_read,
            SND_PCM_FORMAT_U8,
            SND_PCM_ACCESS_RW_INTERLEAVED,
            1,
            48000,
            1,
            500000)) < 0) { /* 0.5sec */
        printf("Capture open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    //Config playback
    if ((err = snd_pcm_open(&handle_write, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = snd_pcm_set_params(handle_write,
            SND_PCM_FORMAT_U8,
            SND_PCM_ACCESS_RW_INTERLEAVED,
            1,
            48000,
            1,
            500000)) < 0) { /* 0.5sec */
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    //Loop
    while (1) {
        //Read
        frames = snd_pcm_readi(handle_read, buffer, sizeof (buffer));
        if (frames < 0)
            frames = snd_pcm_recover(handle_read, frames, 0);
        if (frames < 0) {
            printf("snd_pcm_writei failed: %s\n", snd_strerror(err));
            break;
        }
        if (frames > 0 && frames < (long) sizeof (buffer))
            printf("Short write (expected %li, wrote %li)\n", (long) sizeof (buffer), frames);
        
        //Write
        frames = snd_pcm_writei(handle_write, buffer, sizeof (buffer));
        if (frames < 0)
            frames = snd_pcm_recover(handle_write, frames, 0);
        if (frames < 0) {
            printf("snd_pcm_writei failed: %s\n", snd_strerror(err));
            break;
        }
        if (frames > 0 && frames < (long) sizeof (buffer))
            printf("Short write (expected %li, wrote %li)\n", (long) sizeof (buffer), frames);
    }
    snd_pcm_close(handle_read);
    snd_pcm_close(handle_write);
    return 0;
}



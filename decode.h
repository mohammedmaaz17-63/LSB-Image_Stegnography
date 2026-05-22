#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "common.h"
#include "types.h"

typedef struct _DecodeInfo
{
    /* SORCE IMAGE INFO */
    FILE *fptr_src_image;
    char *src_fname;

    /* DATA FILE */
    FILE *fptr_secret;
    char *secret_fname;

    int extn_size;
    int secret_data_size;
} DecodeInfo;

// Decoding
Status do_decoding(DecodeInfo *decInfo);

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status decode_magic_string(FILE *fptr_src);

char decode_byte_to_lsb(unsigned char *buff);

Status decode_extn_size(DecodeInfo *decInfo);

uint decode_size_to_lsb(unsigned char *buff);

Status decode_extn(DecodeInfo *decInfo);

Status decode_secret_data_size(DecodeInfo *decInfo);

Status decode_secret_data(DecodeInfo *decInfo);

#endif
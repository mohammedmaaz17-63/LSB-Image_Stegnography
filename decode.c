#include "decode.h"

/* =======================================================================
 *  read_and_validate_decode_args
 *  Validates input arguments and prepares output filename.
 * =======================================================================*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /* Validate input BMP */
    if (!strstr(argv[2], ".bmp"))
    {
        printf("ERROR: Source image must be a .bmp file\n");
        return e_failure;
    }

    int len = strlen(argv[2]);
    if (len < 4 || strcmp(argv[2] + len - 4, ".bmp") != 0)
    {
        printf("ERROR: Invalid output \".bmp\" file\n");
        printf("%s\n", argv[2]);
        return e_failure;
    }
    decInfo->src_fname = argv[2];//for success

    /* Check output filename argument */
    if (argv[3] != NULL)
    {
        /* Allocate memory for output filename */
        decInfo->secret_fname = malloc(20);

        if (!decInfo->secret_fname)
            return e_failure;

        /* Copy user-provided name */
        strcpy(decInfo->secret_fname, argv[3]);

        /* Remove extension, if present */
        char *dot = strchr(decInfo->secret_fname, '.');
        if (dot)
            *dot = '\0';

        return e_success;
    }

    /* Default filename if not provided */
    decInfo->secret_fname = malloc(20);
    if (!decInfo->secret_fname)
        return e_failure;
    strcpy(decInfo->secret_fname, "dec_data");

    return e_success;
}

/*1. =======================================================================
 *  file_open
 *  Opens source BMP for reading.
 * =======================================================================*/
Status file_open(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image = fopen(decInfo->src_fname, "rb");

    if (decInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_fname);
        return e_failure;
    }

    return e_success;
}

/* =======================================================================
 *  decode_byte_to_lsb
 *  Extracts 1 byte from 8 LSBs of image bytes.
 * =======================================================================*/
char decode_byte_to_lsb(unsigned char *buff)
{
    char ch = 0;

    for (int i = 0; i < 8; i++)
        ch = (buff[i] & 1) | (ch << 1);

    return ch;
}

/* 2.=======================================================================
 *  decode_magic_string
 *  Reads and verifies MAGIC_STRING from BMP.
 * =======================================================================*/
Status decode_magic_string(FILE *fptr_src)
{
    fseek(fptr_src, 54, SEEK_SET);//skip the header

    int len = strlen(MAGIC_STRING);
    char str[len + 1];
    char buff[8];

    for (int i = 0; i < len; i++)
    {
        fread(buff, sizeof(buff), 1, fptr_src);
        str[i] = decode_byte_to_lsb(buff);
    }

    str[len] = '\0';
    printf("Decoded Magic String = %s\n", str);

    return (strcmp(str, MAGIC_STRING) == 0) ? e_success : e_failure;
}

/* =======================================================================
 *  decode_size_to_lsb
 *  Extracts 32-bit integer from 32 LSBs of image bytes.
 * =======================================================================*/
uint decode_size_to_lsb(unsigned char *buff)
{
    uint size = 0;

    for (int i = 0; i < 32; i++)
        size = (buff[i] & 1) | (size << 1);

    return size;
}

/* =======================================================================
 *  decode_extn_size
 *  Reads file extension size.
 * =======================================================================*/
Status decode_extn_size(DecodeInfo *decInfo)
{
    unsigned char buff[32];

    fread(buff, 1, 32, decInfo->fptr_src_image);
    decInfo->extn_size = decode_size_to_lsb(buff);

    printf("Extension Size = %d bytes\n", decInfo->extn_size);

    return e_success;
}

/* =======================================================================
 *  decode_extn
 *  Reads and reconstructs extension (like ".txt").
 * =======================================================================*/
Status decode_extn(DecodeInfo *decInfo)
{
    char str[10];
    int i = 0;
    for (i = 0; i < decInfo->extn_size; i++)
    {
        unsigned char buff[8];
        fread(buff, 1, 8, decInfo->fptr_src_image);
        str[i] = decode_byte_to_lsb(buff);
    }

    str[decInfo->extn_size] = '\0';

    printf("Decoded Extension : %s\n", str);

    strcat(decInfo->secret_fname, str);

    printf("Final Output Filename : %s\n", decInfo->secret_fname);

    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");

    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        printf("ERROR: Unable to create file: %s\n", decInfo->secret_fname);
        return e_failure;
    }

    return e_success;
}

/* =======================================================================
 *  decode_secret_data_size
 *  Reads size of hidden data.
 * =======================================================================*/
Status decode_secret_data_size(DecodeInfo *decInfo)
{
    unsigned char buff[32];

    fread(buff, 1, 32, decInfo->fptr_src_image);
    decInfo->secret_data_size = decode_size_to_lsb(buff);

    printf("Secret Data Size = %d bytes\n", decInfo->secret_data_size);

    return e_success;
}

/* =======================================================================
 *  decode_secret_data
 *  Extracts actual hidden data.
 * =======================================================================*/
Status decode_secret_data(DecodeInfo *decInfo)
{
    char str[decInfo->secret_data_size];
    char buff[8];

    for (int i = 0; i < decInfo->secret_data_size; i++)
    {
        fread(buff, 8, 1, decInfo->fptr_src_image);
        str[i] = decode_byte_to_lsb(buff);
    }

    str[decInfo->secret_data_size] = '\0';

    fwrite(str, decInfo->secret_data_size, 1, decInfo->fptr_secret);

    return e_success;
}

/* =======================================================================
 *  do_decoding
 *  Master function – performs entire decoding pipeline.
 * =======================================================================*/
Status do_decoding(DecodeInfo *decInfo)
{
    printf("\n===============  DECODING STARTED  ================ \n");

    if (file_open(decInfo) == e_success)
    {
        printf("File opened successfully.\n");

        if (decode_magic_string(decInfo->fptr_src_image) == e_success)
        {
            printf("Magic string verified.\n");

            if (decode_extn_size(decInfo) == e_success)
            {
                printf("Extension size decoded.\n");

                if (decode_extn(decInfo) == e_success)
                {
                    printf("Extension extracted.\n");

                    if (decode_secret_data_size(decInfo) == e_success)
                    {
                        printf("Secret data size decoded.\n");

                        if (decode_secret_data(decInfo) == e_success)
                        {
                            printf("\n SECRET DATA DECODED SUCCESSFULLY!\n\n");
                            free(decInfo->secret_fname);
                            return e_success;
                        }
                    }
                }
            }
        }
    }

    printf("Decoding Failed.\n");
    return e_failure;
}
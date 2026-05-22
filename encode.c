#include "encode.h"



/* ---------------------------------------------------------------------
 * check_operation_type
 * Identifies whether user wants encode or decode.
 * -------------------------------------------------------------------*/
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if (strcmp(argv[1], "-d") == 0)
        return e_decode;

    printf("Usage:\n");
    printf("Encoding: ./a.out -e <image.bmp> <secret.txt> <output.bmp>\n");
    printf("Decoding: ./a.out -d <image.bmp>\n");
    return e_unsupported;
}

/* 1---------------------------------------------------------------------
 * read_and_validate_encode_args
 * Validates source BMP filename, secret filename, and output BMP
 * Extracts secret file extension.
 * -------------------------------------------------------------------*/
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /* Validate BMP input file */
    if (strstr(argv[2], ".bmp")==NULL)
    {
        printf("ERROR: Source image must be with \".bmp\" extension\n");
        return e_failure;
    }

    /* Validate secret file extension */
    if (!strchr(argv[3], '.'))
    {
        printf("ERROR: Secret file must have extension (e.g., file.txt)\n");
        return e_failure;
    }

    int len = strlen(argv[2]);
    if (len < 4 || strcmp(argv[2] + len - 4, ".bmp") != 0)
    {
        printf("ERROR: Invalid output \".bmp\" file\n");
        printf("%s\n", argv[2]);
        return e_failure;
    }

    /* Extract extension */
    char *ptr = strrchr(argv[3], '.');
    strcpy(encInfo->extn_secret_file, ptr);

    encInfo->secret_fname = argv[3];

    /* Validate output stego BMP */
    if (argv[4] != NULL)
    {
        if (!strstr(argv[4], ".bmp"))
        {
            printf("ERROR: Output file must be \".bmp\"\n");
            return e_failure;
        }

        int len = strlen(argv[4]);
        if (len < 4 || strcmp(argv[4] + len - 4, ".bmp") != 0)
        {
            printf("ERROR: Invalid output \".bmp\" file\n");
            printf("%s\n", argv[4]);
            return e_failure;
        }

        encInfo->stego_image_fname = argv[4];
        printf("Output Stego Image     : %s\n", argv[4]);
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
        printf("Output file not given — using default: stego.bmp\n");
    }

    /* Store source filename */
    encInfo->src_image_fname = argv[2];

    printf("File Extension Detected: %s\n", encInfo->extn_secret_file);
    printf("Extension Length        : %ld\n", strlen(encInfo->extn_secret_file));

    return e_success;
}


/*2. ---------------------------------------------------------------------
 * open_files
 * Opens: source BMP, secret file, and output stego file.
 * Returns success or failure if file open fails.
 * -------------------------------------------------------------------*/
Status open_files(EncodeInfo *encInfo)
{
    /* Open source image */
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    if (encInfo->fptr_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open %s\n", encInfo->src_image_fname);
        return e_failure;
    }

    /* Open secret file */
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    if (encInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open %s\n", encInfo->secret_fname);
        return e_failure;
    }

    /* Open output stego image */
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open %s\n", encInfo->stego_image_fname);
        return e_failure;
    }

    printf("All files opened successfully!\n");
    return e_success;
}

/*3.1 ---------------------------------------------------------------------
 * get_image_size_for_bmp
 * Reads width and height from the BMP header (offset 18).
 * Returns total image capacity in bytes (width * height * 3).
 * -------------------------------------------------------------------*/
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;

    fseek(fptr_image, 18, SEEK_SET); // Move to width position
    fread(&width, sizeof(int), 1, fptr_image);
    printf("Image Width  : %u\n", width);

    fread(&height, sizeof(int), 1, fptr_image);
    printf("Image Height : %u\n", height);

    return width * height * 3; // BMP uses 3 bytes per pixel
}

/*3.2 ---------------------------------------------------------------------
 * get_file_size
 * Returns size of secret file in bytes.
 * -------------------------------------------------------------------*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

/* 3.---------------------------------------------------------------------
 * check_capacity
 * Ensures BMP has enough room to hide all data.
 * -------------------------------------------------------------------*/
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

    long required_capacity =
        (strlen(MAGIC_STRING) + strlen(encInfo->extn_secret_file) + sizeof(int)                     + sizeof(encInfo->size_secret_file) + encInfo->size_secret_file) * 8;
//                                to store the extension             to store the size of extension     to store the size of secret file    to store the secret data
    if (encInfo->image_capacity > required_capacity)
    {
        printf("Image has enough capacity to hide data.\n");
        return e_success;
    }

    printf("ERROR: Image does NOT have enough capacity!\n");
    return e_failure;
}


/* 4.---------------------------------------------------------------------
 * copy_bmp_header
 * Copies the first 54 bytes (header) unchanged.
 * -------------------------------------------------------------------*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buff[54];
    fseek(fptr_src_image, 0, SEEK_SET);
    fread(buff, 1, 54, fptr_src_image);
    fwrite(buff, 1, 54, fptr_dest_image);

    printf("BMP Header copied successfully.\n");
    return e_success;
}

/* ---------------------------------------------------------------------
 * encode_byte_to_lsb
 * Encodes 1 byte into 8 bytes of image buffer.
 * -------------------------------------------------------------------*/
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
        image_buffer[i] = (image_buffer[i] & ~(1)) | ((data >> (7 - i)) & 1);//here first we are clearing the lsb of source image byte and then we are setting the stego image byte's lsb to the corresponding bit of the data byte (secret data, magic string, file extension, or file size) that we want to hide.

    return e_success;
}

/* ---------------------------------------------------------------------
 * encode_data_to_image
 * Encodes byte array (string) into the BMP image pixel data.
 * -------------------------------------------------------------------*/
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buff[8];

    for (int i = 0; i < size; i++)
    {
        fread(buff, 8, 1, fptr_src_image);//Read 8 bytes from source image
        encode_byte_to_lsb(data[i], buff);
        fwrite(buff, 8, 1, fptr_stego_image);//Write 8 bytes to the destination image(Here, we are writing the magic string, file extension, file size, and secret data)
    }

    return e_success;
}

/* ---------------------------------------------------------------------
 * encode_magic_string
 * Stores MAGIC_STRING inside the BMP.
 * -------------------------------------------------------------------*/
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image((char *)magic_string,
                         strlen(magic_string),
                         encInfo->fptr_src_image,
                         encInfo->fptr_stego_image);

    printf("Magic string encoded successfully.\n");
    return e_success;
}

/* ---------------------------------------------------------------------
 * encode_size_to_lsb
 * Converts 32-bit integer into 32 bytes (1 bit each).
 * -------------------------------------------------------------------*/
Status encode_size_to_lsb(long data, char *buff)
{
    for (int i = 0; i < 32; i++)
        buff[i] = (buff[i] & ~(1)) | ((data >> (31 - i)) & 1);

    return e_success;
}

/* ---------------------------------------------------------------------
 * encode_secret_file_extn_size
 * Encodes size of file extension (e.g., 4 for ".txt"), Because during decoding we must know how many bytes to read for the file extension.
 * -------------------------------------------------------------------*/
Status encode_secret_file_extn_size(long file_size, EncodeInfo *encInfo)
{
    char buff[32];
    fread(buff, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, buff);
    fwrite(buff, 32, 1, encInfo->fptr_stego_image);

    printf("File extension size encoded.\n");
    return e_success;
}

/* ---------------------------------------------------------------------
 * encode_secret_file_extn
 * Stores file extension (.txt, .c, .jpg)
 * -------------------------------------------------------------------*/
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buff[8];

    for (int i = 0; i < strlen(file_extn); i++)
    {
        fread(buff, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i], buff);
        fwrite(buff, 8, 1, encInfo->fptr_stego_image);
    }

    printf("File extension encoded.\n");
    return e_success;
}

/* ---------------------------------------------------------------------
 * encode_secret_file_size(it is the file containing the secret data)
 * Encodes size of the actual secret file. This size is required during decoding to know How many bytes to extract?
 * -------------------------------------------------------------------*/
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char buff[32];
    fread(buff, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, buff);
    fwrite(buff, 32, 1, encInfo->fptr_stego_image);

    printf("Secret file size encoded.\n");
    return e_success;
}


/* ---------------------------------------------------------------------
 * encode_secret_file_data
 * Stores all secret file bytes into the image.
 * -------------------------------------------------------------------*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    long size = encInfo->size_secret_file;
    char buff[size];

    rewind(encInfo->fptr_secret);
    fread(buff, size, 1, encInfo->fptr_secret);

    char data[8];

    for (int i = 0; i < size; i++)
    {
        fread(data, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(buff[i], data);
        fwrite(data, 8, 1, encInfo->fptr_stego_image);
    }

    printf("Secret data encoded.\n");
    return e_success;
}

/* ---------------------------------------------------------------------
 * copy_remaining_img_data
 * -------------------------------------------------------------------*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;

    printf("Copying remaining image data...\n");

    while (fread(&ch, 1, 1, fptr_src) > 0)
        fwrite(&ch, 1, 1, fptr_dest);

    return e_success;
}

/* ---------------------------------------------------------------------
 * do_encoding
 * Master function that performs all encoding steps in order.
 * -------------------------------------------------------------------*/
Status do_encoding(EncodeInfo *encInfo)
{
    printf("\n============ ENCODING PROCESS STARTED ============\n");

    if (open_files(encInfo) == e_success)
    {
        if (check_capacity(encInfo) == e_success)
        {
            if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
            {
                if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
                    {
                        if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                        {
                            if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                            {
                                if (encode_secret_file_data(encInfo) == e_success)
                                {
                                    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf("\n Encoding Completed Successfully!\n");

                                        fclose(encInfo->fptr_src_image);
                                        fclose(encInfo->fptr_stego_image);
                                        fclose(encInfo->fptr_secret);

                                        return e_success;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    printf("Encoding failed.\n");
    return e_failure;
}

//After doing the encoding successfully, we have stored the secret data, magic string, file extension, and file size into the destination image (stego image) by modifying the least significant bits of the pixel data of the source image.
//we are only modifiying the lsb of the pixel data of the source image to hide the secret data, magic string, file extension, and file size. The rest of the bits of the pixel data remain unchanged, so the visual quality of the image is not significantly affected. The same text can be decoded using encoded image and can be stored in separate decodetext.txt file.-> this will happen in decode.c
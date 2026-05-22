/*
 Name : Mohammed Maaz
 Roll No : 25031A_114
 Start Date : 02/02/2026
 End Date : 09/02/2026

 Description :
 Implementation of the art and science of hiding information using LSB mechanism in C programming.
 I have used the image(be.bmp) to hide secret message(sec.txt) by creating a duplicate(encoded) image of original image.
 The same text can be decoded using encoded image and can be stored in separate decodetext.txt file.
 */

#include "encode.h"

int main(int argc, char *argv[])
{
    /* ---------------------------------------------------------
     * 3. Validate number of arguments for Encodeing
     * ---------------------------------------------------------*/
    if (argc < 3 || argc > 5)
    {
        printf("\nERROR: Not enough arguments!\n");
        printf("\nUsage :\n");
        printf("   -------------------------------------------------------\n");
        printf("    Encoding:\n");
        printf("       ./a.out -e <input.bmp> <secret.txt> <output.bmp>\n");
        printf("\n    Decoding:\n");
        printf("       ./a.out -d <stego.bmp> <decoded_output_file(optional)>\n");
        printf("   -------------------------------------------------------\n\n");
        return 0;
    }
    /* ---------------------------------------------------------
     * 1. Check operation type
     * ---------------------------------------------------------*/
    OperationType op = check_operation_type(argv);

    /* ---------------------------------------------------------
     * 2. Encoding Operation
     * ---------------------------------------------------------*/
    if (op == e_encode)
    {
        EncodeInfo encInfo;

        /* ---------------------------------------------------------
         * 3. Validate number of arguments for Encodeing
         * ---------------------------------------------------------*/
        if (argc < 4 || argc > 5)
        {
            printf("\nERROR: Not enough arguments!\n");
            printf("\nUsage :\n");
            printf("   -------------------------------------------------------\n");
            printf("    Encoding:\n");
            printf("       ./a.out -e <input.bmp> <secret.txt> <output.bmp>\n");
            printf("   -------------------------------------------------------\n\n");
            return 0;
        }

        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {

            printf("\n MODE : Encoding Selected\n");
            printf(" Validation Successful. Starting Encoding...\n\n");

            do_encoding(&encInfo);
            return 0;
        }
        else
        {
            printf("ERROR: Encoding validation failed!\n");
            return 0;
        }
    }

    /* ---------------------------------------------------------
     * 4. Decoding Operation
     * ---------------------------------------------------------*/
    else if (op == e_decode)
    {
        DecodeInfo decInfo;

        /* ---------------------------------------------------------
         * 5. Validate number of arguments for Encodeing
         * ---------------------------------------------------------*/
        if (argc < 3 || argc > 4)
        {
            printf("\nERROR: Not enough arguments!\n");
            printf("\nUsage :\n");
            printf("   -------------------------------------------------------\n");
            printf("\n    Decoding:\n");
            printf("       ./a.out -d <stego.bmp> <decoded_output_file(optional)>\n");
            printf("   -------------------------------------------------------\n\n");
            return 0;
        }
        //decoded_output_file-> this is the file where we will store the decoded secret data after decoding the stego image.
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {

            printf("\nMODE : Decoding Selected\n");
            printf("Validation Successful. Starting Decoding...\n\n");

            do_decoding(&decInfo);
            return 0;
        }
        else
        {
            printf("ERROR: Decoding validation failed!\n");
            return 0;
        }
    }

    /* ---------------------------------------------------------
     * 6. Unsupported Operation
     * ---------------------------------------------------------*/
    else
    {
        printf("\nERROR: Unsupported Operation!\n");

        printf("\nUsage :\n");
        printf("   -------------------------------------------------------\n");
        printf("    Encoding:\n");
        printf("       ./a.out -e <input.bmp> <secret.txt> <output.bmp>\n");
        printf("\n    Decoding:\n");
        printf("       ./a.out -d <stego.bmp> <decoded_output_file(optional)>\n");
        printf("   -------------------------------------------------------\n\n");

        return 0;
    }

    return 0;
}
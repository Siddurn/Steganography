#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"

Status open_dfiles(DecodeInfo *decInfo)
{
    // ./a.out -d stego.bmp output.txt
    // Stego Image file
    decInfo->fptr_stego = fopen(decInfo->stego_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_fname);
        return d_failure;
    }

    // Output file
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    // Do Error handling
    if (decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);

        return d_failure;
    }

    // No failure return e_success
    return d_success;
}

Status read_and_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // ./a.out -d stego.bmp output.txt
    if (argv[2] != NULL && strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        decInfo->stego_fname = argv[2];
    }
    else
    {
        return d_failure;
    }
    // validate .txt
    if (argv[3] != NULL)
    {
        decInfo->output_fname = argv[3];
    }
    else
    {
        decInfo->output_fname = "output.txt";
    }
    return d_success;
}
void read_bits(FILE *fptr, char *d_byte, int size)
{
    char data;
    char mask = 0x01;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            fread(&data, sizeof(char), 1, fptr);
            d_byte[i] |= ((data & mask) << (7 - j));
        }
    }
}

Status decode_magic_string(const char *magic_str, DecodeInfo *decInfo)
{
    int size = strlen(magic_str);
    printf("size = %d\n", size);
    fseek(decInfo->fptr_stego, 54, SEEK_SET);
    char data;
    char d_byte[size + 1];
    for (int i = 0; i < size + 1; i++)
    {
        d_byte[i] = 0;
    }

    read_bits(decInfo->fptr_stego, d_byte, size);
    printf("Decoded magic string: %s\n", d_byte);

    if (strcmp(d_byte, magic_str) == 0)
    {

        return d_success;
    }
    else
    {
        printf("Magic string not matched\n");
        return d_failure;
    }
}
Status decode_secret_file_extension_size(DecodeInfo *decInfo)
{
    char d_byte[4];
    for (int i = 0; i < 4; i++)
        d_byte[i] = 0;
    read_bits(decInfo->fptr_stego, d_byte, 4);
    int sec_fsize = 0;
    for (int i = 0; i < 4; i++)
    {
        sec_fsize |= d_byte[i];
        if (i == 3)
            break;
        sec_fsize <<= 8;
    }
    printf("The size of secret file extension is %d\n", sec_fsize);
    return d_success;
}
Status decode_secret_file_extension(DecodeInfo *decInfo)
{
    char d_byte[5];
    for (int i = 0; i < 4; i++)
        d_byte[i] = 0;
    d_byte[4] = '\0';
    read_bits(decInfo->fptr_stego, d_byte, 4);
    printf("Secret file extension is: %s\n", d_byte);
    return d_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char d_byte[4];
    for (int i = 0; i < 4; i++)
        d_byte[i] = 0;
    read_bits(decInfo->fptr_stego, d_byte, 4);
    decInfo->file_size = 0;
    for (int i = 0; i < 4; i++)
    {
        decInfo->file_size |= d_byte[i];
        if (i == 3)
            break;
        decInfo->file_size <<= 8;
    }
    printf("Secret file size = %ld\n", decInfo->file_size);
    return d_success;
}
Status decode_secret_data(DecodeInfo *decInfo)
{
    char d_byte[decInfo->file_size];
    for (int i = 0; i < decInfo->file_size; i++)
    {
        d_byte[i] = 0;
    }
    read_bits(decInfo->fptr_stego, d_byte, decInfo->file_size);
    fwrite(d_byte, sizeof(char), decInfo->file_size, decInfo->fptr_output);
    return d_success;
}

/*****************************************************************/
// Function to do decoding
Status do_decoding(DecodeInfo *decInfo)
{
    // Call rest of the encoding functions
    // open required file
    if (open_dfiles(decInfo) == d_success)
    {
        printf("All files opened successfully\n");
        if (decode_magic_string(MAGIC_STRING, decInfo) == d_success)
        {
            printf("Magic string decoded successfully\n");
            if (decode_secret_file_extension_size(decInfo) == d_success)
            {
                printf("Decode secret file exension size is successfull\n");
                if (decode_secret_file_extension(decInfo) == d_success)
                {
                    printf("Secret file extension decodded successfully\n");
                    if (decode_secret_file_size(decInfo) == d_success)
                    {
                        printf("Secret file size is decoded successfully\n");
                        if (decode_secret_data(decInfo) == d_success)
                        {
                            printf("Secret data is successfully Decoded\n");
                        }
                        else
                        {
                            printf("Failed to decode secret data\n");
                        }
                    }
                    else
                    {
                        printf("Failed to decode secret file size\n");
                    }
                }
                else
                {
                    printf("Failed to decode secret file extension\n");
                }
            }
            else
            {
                printf("Decode secret file exension failed\n");
            }
        }
        else
        {
            printf("Failed to decode Magic string\n");
            return d_failure;
        }
    }
    else
    {
        printf("Failed to open files\n");
        return d_failure;
    }

    return d_success;
}
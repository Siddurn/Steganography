#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"

int main(int argc, char *argv[])
{
    // check operation type
    if (check_operation_type(argv) == e_encode)
    {
        EncodeInfo encInfo;
        printf("Selected Encoding\n");
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            printf("Read and validate encode arguement is successful\n");
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Completed encoding\n");
            }
            else
            {
                printf("Failed to do the encoding\n");
            }
        }
        else
        {
            printf("Failed: Read and validate encode argument\n");
        }
    }
    else if (check_operation_type(argv) == e_decode)
    {
        printf("Selected Decoding\n");
        DecodeInfo decInfo;
        if (read_and_decode_args(argv, &decInfo) == d_success)
        {
            printf("Read and validated argv successfully\n");
            if (do_decoding(&decInfo) == d_success)
            {
                printf("Completed decoding\n");
            }
            else
            {
                printf("Failed to do the decoding\n");
            }
        }
        else
        {
            printf("Failed to read and validate decode arg\n");
        }
    }
    else
    {
        printf("Invalid Option\n*****Usage*****\n");
        printf("Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
        printf("Decoding: ./a.out -d stego.bmp output.txt\n");
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if (strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

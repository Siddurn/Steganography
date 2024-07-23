#ifndef DECODE_H
#define DECODE_H
#include "types.h"
// ./a.out -d stego.bmp output.txt
typedef struct _DecodeInfo
{
    //stego file name
    char *stego_fname;
     FILE *fptr_stego;
     char decode_magic[2];


     //Output file
     char *output_fname;
     FILE *fptr_output;
     long int file_size;
}DecodeInfo;

//Functions declarations

Status read_and_decode_args(char *argv[],DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string(const char *magic_str,DecodeInfo *decInfo);

Status decode_secret_file_extension_size(DecodeInfo *decInfo);

Status decode_secret_file_extension(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_data(DecodeInfo *decInfo);

void read_bits(FILE *fptr,char *d_byte,int size);

#endif
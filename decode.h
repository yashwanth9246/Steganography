#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct DecodeInfo
{
    char *stego_image_fname;
    FILE *fptr_stego_image;
    int magic_string_size;
    char *output_fname;
    FILE *fptr_output_file;
    char extn[5];
    int extn_size;
    int data_size;
}DecodeInfo;
/* To read and validate the arguments provided by the user*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* To open the validated files */
Status open_files_decode(DecodeInfo *decInfo);

/* To decode the stored Magic string size */
Status decode_magic_string_size(DecodeInfo *decInfo);

/* To decode size from LSB's */
int decode_lsb_to_size(DecodeInfo *decInfo);

/* To decode the stored Magic string */
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo);

/* To decode the data from image */
Status decode_data_to_image(int size,char *string,FILE *fptr);

/* To convert Byte from LSB's */
char decode_byte_to_lsb(char *buffer);

/*To decode the stored extension size*/
Status decode_extension_size(DecodeInfo *decInfo);

/*To decode the stored extension*/
Status decode_extension(DecodeInfo *decInfo);

/*To decode the stored secret data size */
Status decode_secret_data_size(DecodeInfo *decInfo);

/*To decode the stored secret data and save it to new file */
Status decode_secret_data(DecodeInfo *decInfo);

/*To start the decoding*/
Status do_decoding(DecodeInfo *decInfo);
#endif

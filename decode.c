#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
	decInfo ->stego_image_fname=argv[2];
    }
    if(argv[3]!=NULL)
    {
	decInfo->output_fname=strtok(argv[3],".");
    }
    else
    {
	printf("INFO: Output File name Not Mentioned. Creating Output.txt as default\n");
	decInfo->output_fname=malloc(20);
	strcpy(decInfo->output_fname,"Output");
    }
return e_success;

}


Status open_files_decode(DecodeInfo *decInfo){
    printf("INFO: Opening Required Files\n");
    decInfo->fptr_stego_image=fopen(decInfo->stego_image_fname,"r");
    if(ferror(decInfo->fptr_stego_image)){
	printf("INFO: %s is not available",decInfo->stego_image_fname);
	return e_failure;
    }
    fseek(decInfo->fptr_stego_image,54,SEEK_SET);
    return e_success;
}

Status decode_magic_string_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Magic String Size\n");
    decInfo->magic_string_size=decode_lsb_to_size(decInfo);
    return e_success;
}

int decode_lsb_to_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int size=0;
    fread(buffer,32,1,decInfo->fptr_stego_image);
    for(int i=0;i<32;i++)
    {
	size= (size | ((buffer[i] & 1)<<i));
    }
    return size;
}

Status decode_magic_string(char *magic_string,DecodeInfo *decInfo)
{
    printf("INFO: Decoding magic string\n");
    char *local_magic=malloc((decInfo->magic_string_size)+1);
    if((decode_data_to_image(decInfo->magic_string_size,local_magic,decInfo->fptr_stego_image))==e_success){
    if(strcmp(local_magic,magic_string)==0)
    {
	return e_success;
    }
    else
	return e_failure;
    }
}

Status decode_data_to_image(int size,char *string,FILE *fptr)
{
int i;
char buffer[8];
    for(int i=0;i<size;i++)
    {
	fread(buffer,8,1,fptr);
	string[i]=decode_byte_to_lsb(buffer);
    }
    string[size]='\0';
    return e_success;
}

char decode_byte_to_lsb(char *buffer)
{
    char data=0;
    for(int i=0;i<8;i++)
    {
	data= data|(buffer[i] & 1)<<i;
    }
    return data;
}

Status decode_extension_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding extension size\n");
    decInfo->extn_size=decode_lsb_to_size(decInfo);
    return e_success;
}

Status decode_extension(DecodeInfo *decInfo)
{
    printf("INFO: Decoding Extension\n");
    char buffer[5];
    decode_data_to_image(decInfo->extn_size,buffer,decInfo->fptr_stego_image);
    strcpy(decInfo->extn,buffer);
    strcat(decInfo->output_fname,buffer);
    decInfo->fptr_output_file=fopen(decInfo->output_fname,"w");
    printf("INFO: Opened Output File");
    return e_success;
}

Status decode_secret_data_size(DecodeInfo *decInfo)
{
    printf("INFO: Decoding secret data size\n");
    decInfo->data_size=decode_lsb_to_size(decInfo);
    return e_success;
}


Status decode_secret_data(DecodeInfo *decInfo)
{
    printf("INFO: Decoding secret data to file\n");
    decInfo->data_size=decInfo->data_size;
    char buffer[decInfo->data_size];
   decode_data_to_image(decInfo->data_size,buffer,decInfo->fptr_stego_image);
fputs(buffer,decInfo->fptr_output_file);
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    printf("## Decoding Procedure Started ##\n");
    if(decode_magic_string_size(decInfo)==e_success)
    {
	printf("INFO: Done\n");
	if(decode_magic_string(MAGIC_STRING,decInfo)==e_success)
	{
	    printf("INFO: Done\n");
	    if(decode_extension_size(decInfo)==e_success)
	    {
		printf("INFO: Done\n");
		if(decode_extension(decInfo)==e_success)
		{
		    printf("INFO: Done\n");
		    if(decode_secret_data_size(decInfo)==e_success)
		    {
			printf("INFO: Done\n");
			if(decode_secret_data(decInfo)==e_success){
			printf("INFO: Done\n");
			return e_success;
			}
			else{
			    printf("INFO: Not able to decode the secret data\n");
			    return e_failure;
			}
		    }
		    else{
			printf("INFO: Not able to decode the secret data size\n");
			return e_failure;
		    }
		}
		else{
		    printf("INFO: Not able to decode the Extension\n");
		    return e_failure;
		}
	    }
	    else{
		printf("INFO: Not able to decode the Extension size\n");
		return e_failure;
	    }
	}
	else{
	    printf("INFO: Not able to decode the magic string\n");
	    return e_failure;
	}
    }
    else{
	printf("INFO: Not able to decode the magic string size\n");
	return e_failure;
    }
}





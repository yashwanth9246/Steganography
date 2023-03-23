#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0){
	return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0){
	return e_decode;
    }
    else
	return e_unsupported;
}




Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
	encInfo->src_image_fname=argv[2];
    }
    if(((strstr(argv[3],".txt")!=NULL) || strstr(argv[3],".c")!=NULL))
    {
    encInfo->secret_fname=argv[3];
    }
    if(argv[4]!=NULL){
	if(strstr(argv[4],".bmp")!=NULL)
	{
	    encInfo->stego_image_fname=argv[4];
	}
    }
    else{
	printf("INFO: Output File not mentioned. Creating steged_img.bmp as default\n");
	encInfo->stego_image_fname="steged_img.bmp";
    }
    return e_success;
}





Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);
    printf("INFO: Copying Image Header\n");
    char buffer[54];
    fread(buffer,54,1,fptr_src_image);
    fwrite(buffer,54,1,fptr_dest_image);
    return e_success;
}





Status check_capacity(EncodeInfo *encInfo)
{
printf("INFO: Checking for %s capacity to handle %s\n",encInfo->src_image_fname,encInfo->secret_fname);
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    if((encInfo->image_capacity)>(54+(sizeof(char)*strlen(MAGIC_STRING)+strlen(MAGIC_STRING) +4+sizeof(char)*strlen(strstr(encInfo->secret_fname,"."))+4+encInfo->size_secret_file)*8))
    {
	return e_success;
    }
}




unsigned int get_file_size(FILE *fptr){
    fseek(fptr,1,SEEK_END);
    uint k=ftell(fptr);
    rewind(fptr);
    return k-1;
}




Status open_files(EncodeInfo *encInfo){
    printf("INFO: Opening Required Files\n");
    encInfo->fptr_src_image=fopen(encInfo->src_image_fname,"r");

	printf("INFO: Opened %s \n", encInfo->src_image_fname);
    encInfo->fptr_secret=fopen(encInfo->secret_fname,"r");

	printf("INFO: Opened %s \n", encInfo->secret_fname);
    if(strcmp(encInfo->stego_image_fname,"\0")){
	encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"w");
    }
    else
	encInfo->fptr_stego_image=fopen("steged_img.bmp","w");

	printf("INFO: Opened %s \n", encInfo->stego_image_fname);
	printf("INFO: Done\n");
	return e_success;
    }





Status encode_magic_string_size(EncodeInfo *encInfo,int Size){
    printf("INFO: Encoding magic string size\n");
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(buffer,Size);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}






Status encode_size_to_lsb(char *buffer,int size){
    for(int i=0;i<32;i++){
	buffer[i]=(buffer[i] & 0xFE)|(size>>i) & 1;
    }
    return e_success;
}









Status encode_magic_string( char *magic_string, EncodeInfo *encInfo){
    printf("INFO: Encoding magic string\n");
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);
return e_success;
}





Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image){
    char buffer[8];
    for(int i=0;i<size;i++){
	fread(buffer,8,1,fptr_src_image);
	if(encode_byte_to_lsb(data[i],buffer)==e_success){
	    fwrite(buffer,8,1,fptr_stego_image);
	}

    }
    return e_success;
}








Status encode_byte_to_lsb(char data, char *image_buffer){
    for(int i=0;i<8;i++){
	image_buffer[i]=(image_buffer[i] & 0xFE)|((data>>i) & 0x01);
    }
    return e_success;
}







Status encode_secret_file_extn_size(EncodeInfo *encInfo)
{
    printf("INFO: Encoding Secret File Extension Size\n");
    char buffer[32];
    int size=strlen(strstr(encInfo->secret_fname,"."));
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(buffer,size);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}





Status encode_secret_file_extn( char *file_extn, EncodeInfo *encInfo)
{
    printf("INFO: Encoding Secret File Extension\n");
    encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}






Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    printf("INFO: Encoding Secret file size\n");
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(buffer,file_size);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    return e_success;
}






Status encode_secret_file_data(EncodeInfo *encInfo)
{
    printf("INFO: Encoding Secret File data\n");
    char buffer[encInfo->size_secret_file];
    fread(buffer,(encInfo->size_secret_file),1,encInfo->fptr_secret);
    encode_data_to_image(buffer,strlen(buffer),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}




Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    printf("INFO: Copying left over data\n");
    while(1)
    {
	char buffer=fgetc(fptr_src);
	if(feof(fptr_src))
	    break;
	fputc(buffer,fptr_dest);
	
    }
    return e_success;
}
	
    


Status do_encoding(EncodeInfo *encInfo){
    printf("INFO: ## Encoding Procedure Started ##\n");
    printf("INFO: Checking for %s size\n",encInfo->secret_fname);
    if(get_file_size(encInfo->fptr_secret)>1)
    {
	printf("INFO: Done. Not Empty\n");
    if(check_capacity(encInfo)==e_success)
    {
	printf("INFO: Done. Found OK\n");
	if((copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image))==e_success)
	{
	    printf("INFO: Done\n");
	    if(encode_magic_string_size(encInfo,strlen(MAGIC_STRING))==e_success)
	    {
		printf("INFO: Done\n");
		if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
		{
		    printf("INFO: Done\n");
		    if(encode_secret_file_extn_size(encInfo)==e_success)
		    {
			printf("INFO: Done\n");
			if(encode_secret_file_extn((strstr(encInfo->secret_fname,".")),encInfo)==e_success)
			{
			    printf("INFO: Done\n");
			    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
			    {
				printf("INFO: Done\n");
				if(encode_secret_file_data(encInfo)==e_success)
				{
				    printf("INFO: Done\n");
				    if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
				    {
					printf("INFO: Done\n");
					return e_success;
				    }
				    else{
					printf("INFO: Not able to copy the remaining data\n");
					return e_failure;
				    }
				}
				else{
				    printf("INFO: Not able to encode secret file data\n");
				    return e_failure;
				}
			    }
			    else{
				printf("INFO: Not able to encode secret file size\n");
				return e_failure;
			    }
			}
			else{
			    printf("INFO: Not able to encode secret file extension\n");
			    return e_failure;
			}
		    }
		    else{
			printf("INFO: Not able to encode secret file extension size\n");
			return e_failure;
		    }
		}
		else{
		    printf("INFO: Not able to encode Magic String\n");
		    return e_failure;
		}
	    }
	    else{
		printf("INFO: Not able to encode magic string size\n");
		return e_failure;
	    }
	}
	else{
	    printf("INFO: Not able to copy the header\n");
	    return e_failure;
	}
    }
    else{
	printf("INFO: Secret file Empty\n");
	return e_failure;
    }
}
}



			





/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    //fseek(fptr_image,1,SEEK_SET);
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
/*
   Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}*/

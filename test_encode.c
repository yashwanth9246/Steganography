#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"


int main(int argc, char *argv[])
{
    if(argc<=5)
    {
   	if(argc >=3)
	{ 
    if(check_operation_type(argv)==e_encode)
    {
    EncodeInfo encInfo;
    uint img_size;
    if(read_and_validate_encode_args(argv,&encInfo)==e_success)
    {
    if (open_files(&encInfo) == e_failure)
    {
    	printf("ERROR: %s function failed\n", "open_files" );
    	return 1;
    }
    else
    {
    	printf("SUCCESS: %s function completed\n", "open_files" );
	if(do_encoding(&encInfo)==e_success){
	    printf("INFO: Encoding Done Successfully\n");
	}
    }
    }
    }
    else if(check_operation_type(argv)==e_decode)
    {
	DecodeInfo decInfo;
	if(read_and_validate_decode_args(argv,&decInfo)==e_success)
	{
	    if(open_files_decode(&decInfo)==e_success){
		if(do_decoding(&decInfo)==e_success){
		    printf("INFO: Decoding Done Successfully\n");
		}
	    }
	}
    }
    
    else
    {
	printf("INFO: Enter the valid usage\nUsage -\n");
	printf("<executable file name> -e <Original Picture file name> <File name to be encoded> <Output file name(Optional)>\n");
	printf("<executable file name> -d <Stegenographed file name> <Output file name(Optional)>\n");
    }
	}
    }
    return 0;
}

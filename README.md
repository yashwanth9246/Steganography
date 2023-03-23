# Steganography
Steganography  is the process of hiding information in an image and create a .bmp file without altering the image. And also can decode the image using the same program.
1. Files in the directory are-
   * test_encode.c - This is the file having main function.
   * encode.h - This is the file containing all the function declarations for encoding.
   * decode.h - This is the file containing all the function declarations for decoding.
   * encode.c - This is the file containing all the function definitions for encoding.
   * decode.c - This is the file containing all the function definitions for decoding.
   * types.h  - This is the file containing some datatype declarations.
   * common.h - This is the file containing details about magic string.
2. Open the terminal and execute all the files inorder to generate an executable file.
3. Run the executable file with the arguments as shown
   To encode-
      <executable file name> -e <Original Picture file name> <File name to be encoded> <Output file name(Optional)>
   To decode-
      executable file name> -d <Stegenographed file name> <Output file name(Optional)>

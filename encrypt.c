#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

static unsigned long sdbm(unsigned char *str) 
{ 
    unsigned long hash = 0; 
    int c; 
    while (c = *str++)
    {
        hash = c + (hash << 6) + (hash << 16) - hash; 
    }
    return hash; 
}

unsigned long generate(unsigned long seed)
{
    return (1103515245*seed+12345)%256;
}

int main(int argc, char **argv)
{
    //handle input
    if(argc!=4)
    {
        printf("Error: program command line must be as shown below\n./encrypt (password) (plaintext) (ciphertext)\nNo parameter is optional\n");
        exit(1);
    }
    unsigned long seed = sdbm((unsigned char*)argv[1]);
    FILE* input;
    FILE* output;
    struct stat info;
    if(stat(argv[2], &info)!=0)
    {
        printf("Error: input file does not exist");
        exit(1);
    }
    input = fopen(argv[2], "r");
    output = fopen(argv[3], "w");

    

    char ciphertextBlock[16];
    //this intializes ciphertextBlock as the IV
    for(int i=0; i<16; i++)
    {
        seed = generate(seed);
        ciphertextBlock[i] = (char)seed;
    }
    char tempBlock[16];
    char byteBlock[16];
    char keystreamBlock[16];
    int bytesRead;
    do
    {
        bytesRead = fread(byteBlock, 1, 16, input);

        //step 1: add padding
        if(bytesRead<16)
        {
            for(int i=bytesRead; i<16; i++)
            {
                byteBlock[i] = 16-bytesRead;
            }
        }

        //step 2: apply CBC
        for(int i=0; i<16; i++)
        {
            tempBlock[i] = byteBlock[i]^ciphertextBlock[i];
        }

        //step 3: 16 bytes from keystream
        for(int i=0; i<16; i++)
        {
            seed = generate(seed);
            keystreamBlock[i] = (char)seed;
        }

        //step 4: shuffle bytes
        for (int i=0; i<16; i++)
        { 
            int first = keystreamBlock[i] & 0xf; //lower 4 bits of the keystream 
            int second = (keystreamBlock[i] >> 4) & 0xf; //top 4 bits of the keystream
            //exchange the bytes
            char temp = tempBlock[first]; 
            tempBlock[first] = tempBlock[second];
            tempBlock[second] = temp;
        }

        //step 5: create ciphertextBlock
        for (int i=0; i<16; i++)
        {
            ciphertextBlock[i] = tempBlock[i]^keystreamBlock[i];
        }

        //step 6: write cipher
        fwrite(ciphertextBlock, 1, 16, output);
    } while(bytesRead==16);
}
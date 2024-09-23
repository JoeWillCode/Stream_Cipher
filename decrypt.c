//most of this code was copied from sbencrypt.
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
        printf("Error: program command line must be as shown below\n./decrypt (password) (ciphertext) (plaintext)\nNo parameter is optional\n");
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

    

    //this intializes prevBlock as the IV
    char prevBlock[16];
    for(int i=0; i<16; i++)
    {
        seed = generate(seed);
        prevBlock[i] = (char)seed;
    }
    char tempBlock[16];
    char byteBlock[16];
    char keystreamBlock[16];
    char plaintextBlock[16];
    int bytesRead = fread(byteBlock, 1, 16, input);
    if(bytesRead==0)
    {
        exit(0);
    }
    do
    {   
        //step 1: 16 bytes from keystream
        for(int i=0; i<16; i++)
        {
            seed = generate(seed);
            keystreamBlock[i] = (char)seed;       
        }

        //step 2: key stream XOR
        for(int i=0; i<16; i++)
        {
            tempBlock[i] = byteBlock[i]^keystreamBlock[i];
        }

        //step 3: UNshuffle bytes
        for (int i=15; i>=0; i--)
        { 
            int first = keystreamBlock[i] & 0xf; //lower 4 bits of the keystream 
            int second = (keystreamBlock[i] >> 4) & 0xf; //top 4 bits of the keystream
            //exchange the bytes
            char temp = tempBlock[first]; 
            tempBlock[first] = tempBlock[second];
            tempBlock[second] = temp;
        }

        //step 4: CBC
        for(int i=0; i<16; i++)
        {
            plaintextBlock[i] = tempBlock[i]^prevBlock[i];
        }

        //step 5: copy ciphertextBlock to prevBlock
        for(int i=0; i<16; i++)
        {
            prevBlock[i] = byteBlock[i];
        }

        //step 6: get next input and check for padding before printing
        bytesRead = fread(byteBlock, 1, 16, input);
        if(bytesRead==16)
        {
            fwrite(plaintextBlock, 1, 16, output);
        }
        else
        {
            int padCount = plaintextBlock[15];
            fwrite(plaintextBlock, 1, 16-padCount, output);
        }
    } while(bytesRead==16);
}
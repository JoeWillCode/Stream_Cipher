This stream cipher uses block encryption to encrypt and decrypt any file type using a user's password. It starts by taking the user's alphanumeric password and running it through a simple sbdm hash. Then, the generated hash is used as a seed for a linear congruential generator; this keystream will encrypt our blocks. The encryption begins with the plaintext read in blocks of 16 bytes, adding padding as necessary. It takes this block of plaintext and XORs it with the previous ciphertext block; if it is the first block, it instead uses the first 16 bytes of the keystream. This block is the temp block. Then, the program reads the next 16 bytes of the keystream and swaps the first and last 4 bits of said block. It finally takes the temp block and XORs it with the shuffled keystream block, outputting the final ciphertext block. The decryption program works precisely as the encryption program does. However, it reverses the processes to create the plaintext so long as the password provided is the same as that used in the encryption.

This program was designed for Linux, and a makefile is provided for easy compiling. The syntax is as follows

./encrypt (password) (plaintext) (ciphertext)

./decrypt (password) (ciphertext) (plaintext)

I have included a testing materials folder alongside the program files; it contains two text files from Project Gutenberg—the Odyssey for the average reader and Winnie the Pooh for those who prefer the finer texts. I have also provided an image of myself to prove that even in this fleshy body I too can be securely encrypted. Again, this program can be used for all files, so feel free to try it on your own if you like.

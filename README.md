# gpgme-example-code
**GnuPG Made Easy (GPGME)** is a library designed to make access to GnuPG easier for applications. It doesn't have the best documentation. It is very difficult to find examples with sample code, or they are old. Therefore, I decided to make my code public that can help you in learning GPGME. My code are based on the 2.2.27 version.

## How compile and run code?
Run command:
```
$ gpgme-config --cflags --libs
$ gcc -Wall [output by gpgme-config] -o encrypt encrypt.c
$ ./encrypt
```

## Link to GPGME
**https://gnupg.org/software/gpgme/**
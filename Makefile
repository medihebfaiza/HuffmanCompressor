all : compresser decompresser

coder : compresser.c	
	gcc -Wall -g compresser.c -o compresser
decoder : decompresser.c
	gcc -Wall -g decompresser.c -o decompresser

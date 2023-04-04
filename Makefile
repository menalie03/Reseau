all: main

main: main.o decode_trame.o decode_paquet.o decode_segment.o decode_message.o complement_decodage.o affichage_filtre.o
	gcc -Wall -o main main.o decode_trame.o decode_paquet.o decode_segment.o decode_message.o complement_decodage.o affichage_filtre.o

main.o: main.c decode_segment.o decode_paquet.o decode_trame.o decode_message.o complement_decodage.o affichage_filtre.o
	gcc -Wall  -c main.c

complement_decodage.o: complement_decodage.c complement_decodage.h
	gcc -Wall -c complement_decodage.c

decode_message.o: decode_message.c decode_message.h complement_decodage.o
	gcc -Wall -c decode_message.c

decode_segment.o: decode_segment.c decode_segment.h decode_message.o complement_decodage.o
	gcc -Wall -c decode_segment.c

decode_paquet.o: decode_paquet.c decode_paquet.h decode_segment.o decode_message.o complement_decodage.o
	gcc -Wall -c decode_paquet.c

decode_trame.o: decode_trame.c decode_trame.h decode_paquet.o decode_segment.o decode_message.o complement_decodage.o
	gcc -Wall -c decode_trame.c

affichage_filtre.o: affichage_filtre.c affichage_filtre.h decode_trame.o decode_paquet.o decode_segment.o decode_message.o complement_decodage.o
	gcc -Wall -c affichage_filtre.c	

clean:
	rm -f  main  *.o


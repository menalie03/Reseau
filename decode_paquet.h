#ifndef DECODE_PAQUET_H
#define DECODE_PAQUET_H
#include "complement_decodage.h"
#include "decode_message.h"


// structure paquet IPv4
typedef struct _IPv4 { 
  int version;       // version=4
  int Header_length; // en decimal (entre 5-15)
  char ToS[2];
  long Total_length; 
  char id[5];
  char flag_FO[5]; 
  int TTL;        
  int protocole[2];
  char header_checksum[5];
  char *IP_source;
  char *IP_dst;
  char *option;
  TCP *data;
  char *code_suite;
} IPv4;

IPv4 *create_paquetIP(char* paquet);
void delete_IPv4(IPv4 *paquet);
int est_IPv4(int *tab);

#endif
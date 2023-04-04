#ifndef DECODE_SEGMENT_H
#define DECODE_SEGMENT_H
#include "complement_decodage.h"
#include "decode_message.h"

//les drapeaux de TCP initialisables en bit
typedef struct _flag {
  int URG;
  int ACK;
  int PSH;
  int RST;
  int SYN;
  int FIN;
} Flag;

// structure de TCP
typedef struct _TCP {
  long Port_source;
  long Port_dest;
  long Sequence_nb;
  long Acknowledgmt_nb;
  int Data_Offset; // IHL TCP
  // champs Reserved pas pris en compte comme il est toujours à 0
  Flag *flags;
  long Window;
  char checksum[5]; 
  char urgent_pointer[5];
  char *option; 
  HTTP *data;
  char *code_message;
} TCP;



Flag* traduit_flag(char* chaine);
TCP *create_TCP(char *segment);
void delete_TCP(TCP *msg);
int est_TCP(int *tab);
int contient_HTTP(int p1, int p2);
#endif
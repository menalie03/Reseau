#include "decode_segment.h"
#include "complement_decodage.h"
#include "decode_message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Flag* traduit_flag(char* chaine){
  Flag* flag= (Flag*) malloc (sizeof(Flag));
  if (!flag) {
    printf("Erreur d'allocation\n");
    return NULL;
  }
  int hexa=char_to_int(chaine);
  int* bit=dec_to_bit(hexa);
  flag->URG=bit[6];
  flag->ACK=bit[7];
  flag->PSH=bit[8];
  flag->RST=bit[9];
  flag->SYN=bit[10];
  flag->FIN=bit[11];
  free(bit);
  return flag;  
}

TCP *create_TCP(char *segment) {
  TCP *tcp = (TCP *)malloc(sizeof(TCP));
  if (!tcp) {
    printf("Erreur d'allocation\n");
    return NULL;
  }

  char *port_s = malloc(sizeof(char) * 6);
  char *port_d = malloc(sizeof(char) * 6);
  strncpy(port_s, segment, 5);
  strncpy(port_d, segment + 6, 5);
  port_s[5]='\0';
  port_d[5]='\0';
  tcp->Port_source = char_to_int(port_s);
  tcp->Port_dest = char_to_int(port_d);
  
  if (!contient_HTTP(tcp->Port_source,tcp->Port_dest)){
    printf("Une des trame ne contient pas de message HTTP --- Trame non prise en compte\n");

    // return NULL;
  }

  char *seq = malloc(sizeof(char) * 12);
  strncpy(seq, segment + 12, 11);
  seq[11]='\0';
  tcp->Sequence_nb = char_to_int(seq);

  char *ack = malloc(sizeof(char) * 12);
  ack[11]='\0';
  strncpy(ack, segment + 24, 11);  
  tcp->Acknowledgmt_nb= char_to_int(ack);
  
  tcp->Data_Offset = one_char_to_int(segment[36]);

  char res_flag[4]={segment[37],segment[39],segment[40]};
  tcp->flags = traduit_flag(res_flag);

  char *win = malloc(sizeof(char) * 6);
  strncpy(win, segment + 42, 5);
  tcp->Window = char_to_int(win);
  

  char ch[5]= {segment[48],segment[49], segment[51], segment[52]};
  strcpy(tcp->checksum,ch);
  
  char up[5]= {segment[54],segment[55], segment[57], segment[58]};
  strcpy(tcp->urgent_pointer,up);

  tcp->data = NULL;

  int longueur_option;
  if (tcp->Data_Offset == 5) {
    printf("Il n'y pas d'option de TCP\n");
    tcp->option = NULL;
    if(segment[60]==' ' && segment[61]==' '){
      tcp->code_message=NULL;
    }
    else{
      tcp->code_message=strdup(segment+60);
    }
    return tcp;
  } else {
    longueur_option = tcp->Data_Offset*4 - 20; // octet d'option
    tcp->option = strndup(60 + segment, longueur_option *3); // la taile de l'option inclue sa longueur en octet *2 pour les caratères et donc *3 pour les espaces et le caractère de fin de chaine
  }
  
  int fin=60 + longueur_option * 3;
  
  if (segment[fin]==' ' && segment[fin+1]==' ' ){
    tcp->code_message = NULL;
    
  }else{
    tcp->code_message = strdup(segment + fin);
  }

  free(port_s);
  free(port_d);
  free(win);
  free(seq);
  free(ack);
  return tcp;
}

void delete_TCP(TCP *msg){
  free(msg->flags);
  free(msg->option); 
  delete_HTTP(msg->data);
  free(msg->code_message);
  free(msg);
};

int est_TCP(int *tab) { return (tab[0] == 0 && tab[1] == 6); }

int contient_HTTP(int p1, int p2){
  return ((p1==80 && p2!=80)||(p1!=80 && p2==80)) ;
}

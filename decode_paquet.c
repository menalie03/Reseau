#include "decode_message.h"
#include "decode_segment.h"
#include "decode_paquet.h"
#include "complement_decodage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// création paquet IPv4
IPv4 *create_paquetIP(char* paquet){
  IPv4 *ip = (IPv4 *)malloc(sizeof(IPv4));
  if (!ip) {
    printf("Erreur d'allocation\n");
    return NULL;
  }

  ip->version = one_char_to_int(paquet[0]);
  ip->Header_length = one_char_to_int(paquet[1]);

  ip->data= NULL;

  char tos[2];
  tos[0]=paquet[3];
  tos[1]=paquet[4];
  strcpy(ip->ToS,tos);

  char tl[5]={paquet[6], paquet[7], paquet[9], paquet[10]};
  ip->Total_length = char_to_int(tl);

  char id[5]= {paquet[12], paquet[13], paquet[15], paquet[16]};
  strcpy(ip->id,id);

  char fl[5]= {paquet[18], paquet[19], paquet[21], paquet[22]};
  strcpy(ip->flag_FO,fl);

  char ttl[3];
  strncpy(ttl, paquet+24,2);
  ip->TTL = char_to_int(ttl);
  int p1=one_char_to_int(paquet[27]);
  int p2=one_char_to_int(paquet[28]);
  ip->protocole[0]=p1;
  ip->protocole[1]=p2;
  
  if(! est_TCP(ip->protocole)){
    printf("Une des trames ne contient pas de segment TCP --- Trame non prise en compte\n");
  }

  char ch[5]= {paquet[30], paquet[31], paquet[33], paquet[34]};
  strcpy(ip->header_checksum,ch);

  char* ipsrc= malloc(sizeof(char)*12);
  char* ipdst= malloc(sizeof(char)*12);
  strncpy(ipsrc, paquet+36,11);
  ipsrc[11]='\0';
  strncpy(ipdst, paquet+48,11);
  ipdst[11]='\0';

  ip->IP_source = strdup(adresse_ip(ipsrc));
  ip->IP_dst = strdup(adresse_ip(ipdst));

  int longueur_option;
  if(ip->Header_length==5){
    ip->option =NULL;
    if (paquet[60]==' ' && paquet[61]==' ' ){ip->code_suite=NULL;}
    else{ip->code_suite=strdup(paquet+60);}
    return ip;
  }else{  
    longueur_option=(ip->Header_length*4)-20; //octets d'option   
    ip->option=strndup(60+paquet,longueur_option*3); //la taile de l'option inclue sa longueur en octet *2 pour les caratères et donc *3 pour les espaces et le caractère de fin de chaine 
  }
  int fin=60+(longueur_option*3) ;
  
  if (paquet[fin]==' ' && paquet[fin+1]== ' '){
    ip->code_suite=NULL;
    }else{
        ip->code_suite=strdup(paquet+fin);
  }
  
  free(ipsrc);
  free(ipdst);
  return ip;
}

//fonction de suppression du paquet IPv4
void delete_IPv4(IPv4 *paquet) {
  if(paquet!=NULL){
    if (paquet->code_suite!=NULL){
      free(paquet->code_suite);}
    free(paquet->IP_source);
    free(paquet->IP_source);
    if(paquet->option != NULL) { 
      free(paquet->option); }
    if(paquet->data!=NULL){
      delete_TCP(paquet->data);}
  free(paquet);
  }
}

// verifie qu'il s'agit bien du protocole IPv4
int est_IPv4(int *tab) {
  return (tab[0] == 0 && tab[1] == 8 && tab[2] == 0 && tab[3] == 0);
}



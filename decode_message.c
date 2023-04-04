#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decode_message.h"
#include "complement_decodage.h"

// création du message HTTP
HTTP* create_http(char* msg){
  HTTP* http=(HTTP*) malloc(sizeof(HTTP));
  if(!http){
    printf("Erreur d'allocation");
    return NULL;
  }
  if(msg==NULL){
    return NULL;
  }

  int i=0;
  char* ligne_re=(char*) malloc(sizeof(char)*256);

  while(msg[i]!='0' || msg[i+1]!='d' || msg[i+3]!='0' || msg[i+4]!= 'a'){
    ligne_re[i]=msg[i];  
    i++;
  }
 
  http->req_ou_rep=strdup(hexa_to_char(ligne_re));
  http->entete_corp=strdup(msg+i); 
  free(ligne_re);
  return http;
}

//fonction de suppression du message http
void delete_HTTP(HTTP* http){
  if(http!=NULL){
    free(http->req_ou_rep);
    free(http->entete_corp); 
    free(http);
  }
}
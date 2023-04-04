#ifndef DECODE_MESSAGE_H
#define DECODE_MESSAGE_H
#include "complement_decodage.h"

// structure de HTTP
typedef struct _HTTP {
                      
  char* req_ou_rep;// requete: methode et reponse: HTTP
  char* entete_corp;

} HTTP;

HTTP* create_http(char* msg);
void delete_HTTP(HTTP* http);
#endif
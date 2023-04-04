#include "decode_trame.h"
#include "complement_decodage.h"
#include "decode_message.h"
#include "decode_paquet.h"
#include "decode_segment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// création Ethernet
Ethernet *create_ethernet(char *trame) { 
  Ethernet *eth = (Ethernet *)malloc(sizeof(Ethernet));
  if (!eth) {
    return NULL;
  }
  char *Msrc = malloc(sizeof(char) * 18);
  char *Mdst = malloc(sizeof(char) * 18);
  if (!Mdst || !Msrc) {
    return NULL;
  }
  int type[4];
  int i = 0, dest_ip_read = 0, src_ip_read = 0, type_read = 0, i_bis = 0;
  while (!dest_ip_read || !src_ip_read || !type_read) {
    if (i < 18) { // lecture du mac du destinataire trame[0-17]
      if (i == 17) {
        Mdst[i_bis] = '\0';
        dest_ip_read = 1;
      } 
      else if (trame[0] == ' ') {
        Mdst[i_bis] = ':';
      }else{
        Mdst[i_bis] = trame[0];
      }
      i_bis++;
    }
    if (i >= 18 && i < 36) { // lecture du mac de la source trame[18-35]
      if(i==18){
        i_bis=0;
      }
      if (i == 35) {
        Msrc[i_bis] = '\0';
        src_ip_read = 1;
      } else if (trame[0] == ' ') {
        Msrc[i_bis] = ':';
      }else{
        Msrc[i_bis] = trame[0];
      }
      i_bis++;
    }
    if (i < 41 && i >= 36) { // lecture du type du paquet trame[36-41]
      if (i==36){ 
        i_bis=0;
      }
      if (i == 40) {
        type_read = 1;
      }
      if (trame[0] != ' ') {
        type[i_bis] = one_char_to_int(trame[0]); // conversion entier en char en entier en int
        i_bis++;
      }
    }
    i++;
    trame++;
  }

  eth->mac_src = strdup(Msrc);
  eth->mac_dst = strdup(Mdst);
  eth->type[0] = type[0];
  eth->type[1] = type[1];
  eth->type[2] = type[2];
  eth->type[3] = type[3];
  eth->paquet_not_decoded = trame +1;
  eth->protocole =NULL;
  free(Msrc);
  free(Mdst);
  return eth;
}

// regarde son offset
int is_new_trame(char *offset) {
  if (!offset) {
    return 0;
  }
  for (int i = 0; i < 4; i++) {
    if (offset[i] != '0') {
      return 0; // pas nouvel trame
    }
  }
  return 1;
}

// indique le nombre de lignes d'un fichier trame.txt
int nb_lignes_file_trames(char *nomFichier) {
  FILE *f = fopen(nomFichier, "r");
  if (f == NULL) {
    fprintf(stderr,"erreur d'ouverture du fichier texte contenant les trames %s\n",nomFichier);
    return 0;
  }
  int lgr=0;
  char buffer[256];
  while (fgets(buffer, 256, f) != 0){lgr++;}
  fclose(f);
  return lgr;
}

// prise des trames ethernet dans le format requis (IPV4 TCP HTTP)
ETH_CELL *read_file_trames(char *nomFichier) {
  FILE *f = fopen(nomFichier, "r");
  if (f == NULL) {
    fprintf(stderr,"erreur d'ouverture du fichier texte contenant les trames %s\n",nomFichier);
    return NULL;
  }
  ETH_CELL *leth = NULL;
  Ethernet *eth;
  IPv4 *Paquet;
  TCP *Segment;
  HTTP *http=NULL;
  char buffer[256];
  char *trame = malloc(sizeof(char));
  trame[0] = '\0';
  char *s_trame = malloc(sizeof(char) * 256);
  char *offset = malloc(sizeof(char) * 4);
  int tour = 0, i = 1, last=0;

  int lgr=nb_lignes_file_trames(nomFichier);
  int t=0;

  while (fgets(buffer, 256, f) != 0 || last) {
    if (t==(lgr-1)){
      last=1;
      trame = (char *)realloc(trame, sizeof(char) * (256 * i));
      strncpy(s_trame, buffer + 7, 48); // pour ne pas prendre offsets et commentaires dans le fichier textes de trames
      trame = strcat(trame, s_trame);
    }
    if (!last){
      for (int i = 0; buffer[i] != ' '; i++) {
        offset[i] = buffer[i];
      }
    }    
    if ((is_new_trame(offset) && tour == 1)|| last) { // une nouvelle trame, on rentre ici quand on aura enregistré toute la première trame
      eth = create_ethernet(trame);
      if (!est_IPv4(eth->type)) { printf("Une des trames ne contient pas de paquet IPv4 --- Trame non prise en compte\n");}
      else{//si la trame est de type 0800
        Paquet = create_paquetIP(eth->paquet_not_decoded);
        eth->protocole = Paquet;
        if (est_TCP(Paquet->protocole)) {
          Segment = create_TCP(Paquet->code_suite);
          eth->protocole->data = Segment;
          if (eth->protocole->data!=NULL){
            if (contient_HTTP(eth->protocole->data->Port_source, eth->protocole->data->Port_dest)) {
              if (eth->protocole->data->code_message) { // si il y a des données
                http = create_http(Segment->code_message);
                eth->protocole->data->data = http;
              }
              leth = insert_Eth_in_Cell(eth, leth);
              if (last==1){break;}
            }
          }
        }
      }
      
      i=1;
      free(trame);
      trame=malloc(sizeof(char));
      trame[0] = '\0';
      i++;
    }
    trame = (char *)realloc(trame, sizeof(char) * (256 * i));
    strncpy(s_trame, buffer + 7, 48); // pour ne pas prendre offsets et commentaire dans le fichier textes de trames
    trame = strcat(trame, s_trame);
    tour = 1;
    i++;
    t++;
  }
  free(s_trame);
  free(trame);
  free(offset);
  fclose(f);
  return leth;
}

// création d'une case/cellule d'une liste de trames Ethernet
ETH_CELL *create_cell_eth(Ethernet *eth) {
  ETH_CELL *leth = malloc(sizeof(ETH_CELL));
  if (leth == NULL) {
    printf("Erreur d'allocation\n");
    return NULL;
  }
  if (eth == NULL) {
    printf("il n' y a pas de Ethernet\n");
    return NULL;
  }
  leth->trame = eth;
  leth->suivant = NULL;
  return leth;
}

//création et insertion d'une trame à la fin d'une liste de trames
ETH_CELL *insert_Eth_in_Cell(Ethernet *E, ETH_CELL *leth) {
  ETH_CELL *tmpleth;
  tmpleth = leth;
  if (leth == NULL) {
    return create_cell_eth(E);
  }
  while (tmpleth->suivant) {
    tmpleth = tmpleth->suivant;
  }
  tmpleth->suivant = create_cell_eth(E);
  return leth;
}

//suppression de la trame Ethernet
void delete_eth(Ethernet *eth) {
  if (eth) {
    free(eth->mac_src);
    free(eth->mac_dst);
    delete_IPv4(eth->protocole);
    free(eth);
  }
}

// suppression de la liste Ethernet
void delete_list_eth(ETH_CELL *leth) {
  ETH_CELL *tmp, *tmpp;
  tmp = leth->suivant;
  tmpp = NULL;
  if (leth == NULL) {
    return;
  }
  while (tmp) {
    delete_eth(leth->trame);
    // free(leth);
    tmpp = leth;
    leth = tmp;
    tmpp->suivant = NULL;
    free(tmpp);
    tmp = tmp->suivant;
  }
  if (leth && !tmp) {
    delete_eth(leth->trame);
    free(leth);
  }
}

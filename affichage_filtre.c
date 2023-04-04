#include "affichage_filtre.h"
#include "complement_decodage.h"
#include "decode_message.h"
#include "decode_paquet.h"
#include "decode_segment.h"
#include "decode_trame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//creation d'un tableau de liste ethernet
Eth_cell_tab* create_ceth_tab(int size){
  Eth_cell_tab* mat=(Eth_cell_tab *)malloc(sizeof(Eth_cell_tab));
  mat->size=size;
  mat->tab=(ETH_CELL**)malloc(size*sizeof(ETH_CELL*));
  for(int i=0; i<size; i++){
    mat->tab[i]=NULL;
  }
  return mat;
}

// la position+1 de la liste de connexions du couple (ipA, ipB) dans le tableau de liste
// Si la fonction renvoie 0, c'est que le couple n'est pas dans la liste
int ip_among_connexions(Eth_cell_tab* mat, char *ipA, char *ipB) {
  int pos = 0;
  if(mat->tab[0]==NULL){
    return 0;
  }
  while (pos<mat->size && mat->tab[pos]!=NULL){
    IPv4* ip=(mat->tab[pos])->trame->protocole;
    if(((strcmp(ip->IP_source, ipA))==0 && (strcmp(ip->IP_dst,ipB))==0) || ((strcmp(ip->IP_source, ipB))==0 && (strcmp(ip->IP_dst, ipA)==0))){
      return pos;
    }else{pos++;}
  }
  return pos;
  
}

// repartition dans un tableau de connexions TCP selon les variation des couples IP qui s'échangent les données
Eth_cell_tab* connexions_table(Eth_cell_tab* tab_leth, ETH_CELL *leth) {

  if (!(tab_leth)) {
    return NULL;
  }
  int i=0;
  int pos_insertion;
  ETH_CELL* tmp=leth;
  while(tmp!=NULL && i<tab_leth->size){
    pos_insertion=ip_among_connexions(tab_leth,tmp->trame->protocole->IP_source, tmp->trame->protocole->IP_dst);
    tab_leth->tab[pos_insertion]=insert_Eth_in_Cell(tmp->trame,tab_leth->tab[pos_insertion]);
    tmp=tmp->suivant;
    i++;
  }  
  return tab_leth;
}

//lit l'ensemble des flags à 1
char *write_flags(Flag *F) {
  char *res = malloc(sizeof(char)*40);
  res[0]='\0';
  if (!res) {
    return NULL;
  }
  char name[5];
  if (F->URG) {
    strcpy(name,",URG");
    res = strcat(res, name);
  }
  if (F->ACK) {
    strcpy(name,",ACK");
    res = strcat(res, name);
  }
  if (F->PSH) {
    strcpy(name,",PSH");
    res = strcat(res, name);
  }
  if (F->RST) {
    strcpy(name,",RST");
    res = strcat(res, name);
  } 
  if (F->SYN) {
    strcpy(name,",SYN");
    res = strcat(res, name);
  }
  if (F->FIN) {
    strcpy(name,",FIN");
    res = strcat(res, name);
  }
  res = strcpy(res, res + 1);
  return res;
}

//retourne 1 s'il si la trame est envoyé du client au serveur
int client_to_serveur(Ethernet* eth){
  return (eth->protocole->data->Port_dest == 80) && (eth->protocole->data->Port_source != 80);
}

//retourne 0 s'il si la trame est envoyé du client au serveur
int serveur_to_client(Ethernet* eth){
  return (eth->protocole->data->Port_source == 80) && (eth->protocole->data->Port_dest != 80);
}
  
// affiche le diagramme des flux d'une liste d'échanges TCP - HTTP
void affichage_flot(ETH_CELL *leth) {
  FILE *f = fopen("diagramme.txt", "a");
  if (!f) {
    printf("Erreur ouverture fichier\n");
    return;
  }
  if (!leth){return;}
  ETH_CELL *tmp = leth;
  fprintf(f,"%s              %s\n", leth->trame->protocole->IP_source, leth->trame->protocole->IP_dst);
  while (tmp) {
    Ethernet *eth = tmp->trame;
    IPv4 *ip = eth->protocole;
    TCP *tcp = ip->data;
    HTTP *http = tcp->data;

    if (client_to_serveur(eth)) {
      if (http == NULL) {
        int len = ip->Total_length - ip->Header_length*4 - tcp->Data_Offset*4;
        fprintf(f,"%ld|------------------------>|%ld     TCP: %ld -> %ld [%s] Seq=%ld Ack=%ld Win=%ld Len=%d\n", tcp->Port_source, tcp->Port_dest, tcp->Port_source, tcp->Port_dest, write_flags(tcp->flags),  tcp->Sequence_nb, tcp->Acknowledgmt_nb, tcp->Window, len); 
      } else {
        fprintf(f, "%ld|------------------------>|%ld     HTTP: %s\n", tcp->Port_source, tcp->Port_dest, http->req_ou_rep);
      }
    }
    if (serveur_to_client(eth)) {
      if (http == NULL) {
        int len = ip->Total_length - ip->Header_length*4 - tcp->Data_Offset*4;
        fprintf(f,"%ld|<------------------------|%ld     TCP: %ld-> %ld [%s] Seq=%ld Ack=%ld Win=%ld Len=%d\n", tcp->Port_dest, tcp->Port_source, tcp->Port_source, tcp->Port_dest, write_flags(tcp->flags), tcp->Sequence_nb, tcp->Acknowledgmt_nb, tcp->Window, len); 
      } else {
        fprintf(f,"%ld|<------------------------|%ld     HTTP: %s\n", tcp->Port_dest, tcp->Port_source, http->req_ou_rep);
      }
    }
    tmp = tmp->suivant;
  }

  fprintf(f, "\n\n");
  fclose(f);
}

//affichage du diagramme des flux pour un tableau de listes, soit pour un ensemble de connexions (ipA, ipB)
void diagramme(Eth_cell_tab *tab_leth) { 
  ETH_CELL **res =tab_leth->tab;
  int i=0;
  while(res[i]!=NULL && i<tab_leth->size){
    affichage_flot(res[i]);
    i++;
  }
  
}

// affichage du diagramme des flux avec un filtre ip donné en paramètre
void diagramme_filtre_selon_ip(char *ip, Eth_cell_tab* tab_leth) {
  if (tab_leth) { 
    ETH_CELL **tmp= tab_leth->tab;
    int i=0, verifie=0;
    while (tmp[i] && i<tab_leth->size) {
      if (!strcmp(tmp[i]->trame->protocole->IP_source, ip) || !strcmp(tmp[i]->trame->protocole->IP_dst, ip)) { //ipsrc==ip || ipdst==ip
        affichage_flot(tmp[i]);
        verifie=1;
      }
      i++;
    }
    if (!verifie){printf("Désolé, l'IP saisie ne correspond à aucune adresse IP du diagramme.\n");}
    return;
  }
}

// affichage du diagramme des flux avec un filtre de protocole. Dans notre cas, ayant seulement  protocoles affichés, si la valeur prot ==1, il s'agit d'un TCP et si prot == 2, c'est un HTTP
void diagramme_filtre_selon_prot(int prot, Eth_cell_tab *tab_leth) {
  if (tab_leth) { 
    FILE *f = fopen("diagramme.txt", "w");
    ETH_CELL **tmp = tab_leth->tab;
    ETH_CELL *ttmp;
    int i = 0, verifie = 0;

    while (tmp[i] && i < tab_leth->size) {
      ttmp = tmp[i];
      IPv4 *ip = ttmp->trame->protocole;
      TCP *tcp = ip->data;
      HTTP *http = tcp->data;
      fprintf(f,"%s              %s\n", ttmp->trame->protocole->IP_source, ttmp->trame->protocole->IP_dst);
      while (ttmp) {
        ip=ttmp->trame->protocole;
        tcp=ip->data;
        http=tcp->data;
        if (prot == 2) {
          if (client_to_serveur(ttmp->trame) && http) { 
            fprintf(f, "%ld|------------------------>|%ld     HTTP: %s\n",tcp->Port_source, tcp->Port_dest, http->req_ou_rep);
            verifie = 1;
          }

          if (serveur_to_client(ttmp->trame) && http) { 
            fprintf(f, "%ld|<------------------------|%ld     HTTP: %s\n",tcp->Port_dest, tcp->Port_source, http->req_ou_rep);
            verifie = 1;
          }
        }else if (prot == 1) {
          if (client_to_serveur(ttmp->trame) && tcp) { 
            int len = ip->Total_length - ip->Header_length * 4 - tcp->Data_Offset * 4;
            fprintf(f,"%ld|------------------------>|%ld     TCP: %ld -> %ld [%s] Seq=%ld Ack=%ld Win=%ld Len=%d\n",tcp->Port_source, tcp->Port_dest, tcp->Port_source, tcp->Port_dest, write_flags(tcp->flags), tcp->Sequence_nb, tcp->Acknowledgmt_nb, tcp->Window, len);
            verifie = 1;
          }

          if (serveur_to_client(ttmp->trame) && tcp) { 
            int len = ip->Total_length - ip->Header_length * 4 - tcp->Data_Offset * 4;
            fprintf(f,"%ld|<------------------------|%ld     TCP: %ld -> %ld [%s] Seq=%ld Ack=%ld Win=%ld Len=%d\n",tcp->Port_dest, tcp->Port_source, tcp->Port_source, tcp->Port_dest, write_flags(tcp->flags), tcp->Sequence_nb, tcp->Acknowledgmt_nb, tcp->Window, len);
            verifie = 1;
          }
        }
        ttmp = ttmp->suivant;
      }
      fprintf(f, "\n");
      i++;
    }
    
    // cas où aucune trame n'a été affiché
    if (!verifie) {
      printf("Désolé, aucune trame correspond au protocole saisie\n");
    }

    fclose(f);
    return;
  }
}
#ifndef AFFICHAGE_FILTRE_H
#define AFFICHAGE_FILTRE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "decode_trame.h"
#include "decode_message.h"
#include "decode_segment.h"
#include "decode_paquet.h"
#include "complement_decodage.h"

typedef struct _Eth_cell_tab{
  ETH_CELL **tab;
  int size;
}Eth_cell_tab;



Eth_cell_tab* create_ceth_tab(int size);
int ip_among_connexions(Eth_cell_tab* mat, char *ipA, char *ipB);
Eth_cell_tab* connexions_table(Eth_cell_tab* tab_leth, ETH_CELL *leth);
int client_to_serveur(Ethernet* eth);
int serveur_to_client(Ethernet* eth);
char *write_flags(Flag *F);
void affichage_flot(ETH_CELL *leth);
void diagramme(Eth_cell_tab *mat);
void diagramme_filtre_selon_ip(char *ip, Eth_cell_tab* tab_leth);
void diagramme_filtre_selon_prot(int prot, Eth_cell_tab* tab_leth);

#endif

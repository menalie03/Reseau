#ifndef DECODE_TRAME_H
#define DECODE_TRAME_H
#include "decode_message.h"
#include "decode_segment.h"
#include "decode_paquet.h"
#include "complement_decodage.h"

// structure de l'Ethernet
typedef struct ethernet {
  char *mac_src;
  char *mac_dst;
  int type[4];
  IPv4 *protocole;
  char *paquet_not_decoded; 
} Ethernet;

typedef struct Eth_cell {
  Ethernet *trame;
  struct Eth_cell *suivant;
} ETH_CELL;


Ethernet *create_ethernet(char *trame);
int is_new_trame (char* offset);
int nb_lignes_file_trames(char *nomFichier);
ETH_CELL* read_file_trames(char* nomFichier);
ETH_CELL* create_cell_eth (Ethernet *eth);
ETH_CELL* insert_Eth_in_Cell(Ethernet* E, ETH_CELL *leth);
void delete_eth(Ethernet* eth);
void delete_list_eth(ETH_CELL* leth);
#endif
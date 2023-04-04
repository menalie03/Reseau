#include "affichage_filtre.h"
#include "complement_decodage.h"
#include "decode_message.h"
#include "decode_paquet.h"
#include "decode_segment.h"
#include "decode_trame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  for (int i=0; i<100000000; i++){}
  printf("Bonjour !\n\n");
  for (int i=0; i<100000000; i++){}
  printf("Nous trions d'abord les trames de manière à garder uniquement celles respectant le format Ethernet-IPv4-TCP-HTTP...\n\n");
  ETH_CELL* leth;
  leth=read_file_trames("trame.txt");
  Eth_cell_tab* mat=create_ceth_tab(1001);
  mat=connexions_table(mat, leth);
  for (int i=0; i<100000000; i++){}  
  printf("\n\n\n\nPour visualiser le diagramme type 'Flow Graph' chez Wireshark, choisissez une option dans le menu:\n\n\n");
  for (int i=0; i<100000000; i++){}
  printf("    01 - Afficher l'ensemble des trames Ethernet format ip-tcp-http\n    02 - Filtrer les trames Ethernet selon une adresse IP\n    03 - Filtrer les trames Ethernet selon un protocole\n\nNote:Le visualisateur sera affiché dans le fichier 'diagramme.txt'\n\n");
  
  int saisie;
  scanf("%d",&saisie);

  if ((saisie<1 || saisie>3)){
    printf("\n\nMerci de resaisir le numéro de l'option correspondante.\n");
    scanf("%d",&saisie);
  }
  if (saisie==1){
    diagramme(mat);
  }

  else if (saisie==2){
    printf("\n\nVeuillez saisir une adresse IP.\n");
    char s[256];
    scanf("%s",s);
    diagramme_filtre_selon_ip(s,mat);
  }

   else if (saisie==3){
    printf("\n\nVeuillez choisir parmis les protocoles de filtrage proposés.\n    01 - TCP\n    02 - HTTP\n\n");
    int prot;
    scanf("%d",&prot);
    if ((prot!=1) && (prot!=2)){
      printf("\n\nMerci de resaisir un numéro parmis les valeurs proposées.\n");
      scanf("%d",&prot);
    }
    diagramme_filtre_selon_prot(prot,mat);
  }


  printf("\n\nSouhaitez-vous refaire un test?\n     01 - Oui\n    02 - Non, je souhaite quitter\n");
  scanf("%d",&saisie);
  if ((saisie!=1) && (saisie!=2)){
    printf("\n\nMerci de resaisir le numéro de l'option correspondante.\n");
    scanf("%d",&saisie);
  }
  if (saisie==2){
      printf("\nMerci d'avoir tester le visualisateur.\nSi vous souhaitez refaire d'autres tests, merci de bien vouloir supprimer le fichier 'diagramme.txt' manuellement s'il existe.\nSi vous souhaitez visualiser vos propres trames, merci de bien vouloir les mettre dans le fichier 'trame.txt' en respectant le format sans saut de ligne.\nMerci et à bientot !\n");
      return 1;
  }

  printf("\nPour visualiser le diagramme type 'Flow Graph' chez Wireshark, choisissez une option dans le menu:\n\n\n");
  printf("    01 - Afficher l'ensemble des trames Ethernet format ip-tcp-http\n    02 - Filtrer les trames Ethernet selon une adresse IP\n    03 - Filtrer les trames Ethernet selon un protocole\n\nNote:Le visualisateur sera affiché dans le fichier 'diagramme.txt'\n\n");
  remove("diagramme.txt");
  scanf("%d",&saisie);
  if ((saisie!=3) && (saisie!=1) && (saisie!=2)){
    printf("\n\nMerci de resaisir le numéro de l'option correspondante.\n");
    scanf("%d",&saisie);
  }

  if (saisie==1){
    diagramme(mat);
  }

  else if (saisie==2){
    printf("\n\nVeuillez saisir une adresse IP.\n");
    char s[256];
    scanf("%s",s);
    diagramme_filtre_selon_ip(s,mat);
  }

  else if (saisie==3){
    printf("\n\nVeuillez choisir parmis les protocoles de filtrage proposés.\n    01 - TCP\n    02 - HTTP\n\n");
    int prot;
    scanf("%d",&prot);
    if ((prot!=1) && (prot!=2)){
      printf("\n\nMerci de resaisir un numéro parmis les valeurs proposées.\n");
      scanf("%d",&prot);
    }
    diagramme_filtre_selon_prot(prot,mat);
  }


  printf("\n\nSouhaitez-vous refaire un dernier test?\n     01 - Oui\n     02 - Non, je souhaite quitter\n");
  scanf("%d",&saisie);
  if ((saisie!=1) && (saisie!=2)){
    printf("\n\nMerci de resaisir le numéro de l'option correspondante.\n");
    scanf("%d",&saisie);
  }
  if (saisie==2){
      printf("\nMerci d'avoir tester le visualisateur.\nSi vous souhaitez refaire d'autres tests, merci de bien vouloir supprimer le fichier 'diagramme.txt' manuellement s'il existe.\nSi vous souhaitez visualiser vos propres trames, merci de bien vouloir les mettre dans le fichier 'trame.txt' en respectant le format sans saut de ligne.\nMerci et à bientot !\n");
      return 1;
  }

  printf("\nPour visualiser le diagramme type 'Flow Graph' chez Wireshark, choisissez une option dans le menu:\n\n\n");
  printf("    01 - Afficher l'ensemble des trames Ethernet format ip-tcp-http\n    02 - Filtrer les trames Ethernet selon une adresse IP\n    03 - Filtrer les trames Ethernet selon un protocole\n\nNotes:Le visualisateur sera affiché dans le fichier 'diagramme.txt'\n\n");
  remove("diagramme.txt");
  scanf("%d",&saisie);
  if ((saisie!=3) && (saisie!=1) && (saisie!=2)){
    printf("\n\nMerci de resaisir le numéro de l'option correspondante.\n");
    scanf("%d",&saisie);
  }

  if (saisie==1){
    diagramme(mat);
  }

  else if (saisie==2){
    printf("\n\nVeuillez saisir une adresse IP.\n");
    char s[256];
    scanf("%s",s);
    diagramme_filtre_selon_ip(s,mat);
  }

  else if (saisie==3){
    printf("\n\nVeuillez choisir parmis les protocoles de filtrage proposés.\n      01 - TCP\n      02 - HTTP\n\n");
    int prot;
    scanf("%d",&prot);
    if ((prot!=1) && (prot!=2)){
      printf("\n\nMerci de resaisir un numéro parmis les valeurs proposées.\n");
      scanf("%d",&prot);
    }
    diagramme_filtre_selon_prot(prot,mat);
  }


  for (int i=0; i<10000; i++){}
  printf("\nMerci d'avoir tester le visualisateur.\nSi vous souhaitez refaire d'autres tests, merci de bien vouloir supprimer le fichier 'diagramme.txt' manuellement s'il existe.\nSi vous souhaitez visualiser vos propres trames, merci de bien vouloir les mettre dans le fichier 'trame.txt' en respectant le format sans saut de ligne.\nMerci et à bientot !\n");
  for (int i=0; i<10000; i++){}

  return 0;
}
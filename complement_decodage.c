#include "complement_decodage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char mmaj(char c){
  char m;
  if (c <= 'z' && c>= 'a'){
    m=c- ('a' - 'A');}
  else{
    m=c;
  }
  return m;
}

char* min_a_maj(char* ch){  
  int i=0;
  char* maj=malloc(sizeof(char)*(strlen(ch)+1));
  while(ch[i]!='\0'){
    maj[i] = mmaj(ch[i]);
    i++;
  }
  maj[i]='\0';
  return maj;
}

int one_char_to_int(char c) {
  int sum = 0;
    char m=mmaj(c);
    if (m <= 'Z' && m >= 'A') {
      sum = m - 'A' + 10;
    } else {
      sum = m - '0';
    }
  return sum;
}

long char_to_int(char *hexa) {
  int t = 0;
  long sum = 0;
  char* maj=min_a_maj(hexa);
  for (int i = 0; i < strlen(hexa); i++) {
    if (maj[i] !=' '){
      if (maj[i] <= 'Z' && maj[i] >= 'A') {
        t = maj[i] - 'A' + 10;
      } else {
        t = maj[i] - '0';
      }
      sum = sum * 16 + t;  
    }
  }
  return sum;
}


char* int_to_char(long deci){
  char *s=malloc(sizeof(char)*256);
    if (!s){
    printf("erreur allocation\n");
    return NULL;
  }
  sprintf(s,"%ld",deci);
  s[strlen(s)]='\0';
  return s;
}

char *adresse_ip(char* adresse){
  int i=0;
  int j=0;
  char* hexa=malloc(sizeof(char)*4);
  long cf;
  char *deci=malloc(sizeof(char)*4);
  char* ss=malloc(sizeof(char)*256); 
  while(adresse[i]!='\0'){
    if(adresse[i]==' '){
      hexa[j]='\0';
      cf=char_to_int(hexa);
      deci=int_to_char(cf);
      strcat(ss,deci);
      strcat(ss,".");
      j=0;
    }else{
      hexa[j]=adresse[i];
      j++;
    } 
    i++;
  }
  cf=char_to_int(hexa);
  deci=int_to_char(cf);
  strcat(ss,deci);
  ss[strlen(ss)]='\0';
  free(hexa);
  free(deci);
  return ss;
}

int* dec_to_bit(long d){
  int *bit=(int*) malloc(sizeof(int)*12);
  
  // intialisation
  for(int i=0; i<12; i++){ 
    bit[i]=0;
  }

  for(int i=0; d>0; i++){
    bit[i]=d%2;
    d=d/2;
  }

  // inversion
  int i=0;
  int j=11;
  while(i<6 && j>=6){
    int tmp=bit[i];
    int tmmp=bit[j];
    bit[j]=tmp;
    bit[i]=tmmp;
    i++;
    j--;
  }
  return bit;
}

char one_hexa_to_char(char* hexa){
  long decimal=char_to_int(hexa);
  return (char) decimal;
}

char *hexa_to_char(char *lhexa) { // ex: "47 45 54 20 " ->"GET"
  int i = 0;//position de lhexa
  int j = 0;//position de tmp
  int m = 0;// position de ch
  char one_char;
  char *tmp = malloc(sizeof(char) * 4);
  char *ch = (char *)malloc(sizeof(char) * 256);
  
  while (lhexa[i]!='\0') {
    if (lhexa[i] == ' ') { 
      if(strcmp(tmp,"20")){
        ch[m]=' ';
      }
      tmp[j]='\0';
      one_char = one_hexa_to_char(tmp);
      ch[m]=one_char;
      j=0;
      m++;
    } else {
      tmp[j] = lhexa[i];
      j++;
    }
    i++;
  }
  free(tmp);
  return ch;
}



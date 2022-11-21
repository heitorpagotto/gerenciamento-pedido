#include <stdio.h>

typedef struct
{
  int id;
  char name[50];
  float price;
  char desc[150];
} Product;

typedef struct
{
  int id_h_product;
  int id;
  int id_product;
  int amount;
  float total_price;
} RequestItem;

typedef struct
{
  int id;
  int total_price;
  char client_name[50];
} RquestHeader;

void mainMenuRender();

int main(void)
{
  mainMenuRender();
  return 0;
}

void mainMenuRender()
{
  printf("Selecione o que gostaria de gerenciar:\n\n");
  printf("1.Pedidos\n");
  printf("2.Produtos\n");
  printf("3.Sair");

  getchar();
}
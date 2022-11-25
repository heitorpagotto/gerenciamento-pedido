#include <stdio.h>

// product_data.dat
typedef struct
{
  int id;
  char name[50];
  float price;
  char desc[150];
} Product;

//requests_item_data.dat
typedef struct
{
  int id_h_product;
  int id;
  int id_product;
  int amount;
  float total_price;
} RequestItem;

// requests_header_data.dat
typedef struct
{
  int id;
  int total_price;
  char client_name[50];
} RequestHeader;

void mainMenuRender();
bool insertProduct();
void readProducts();
Product getProductById(int id, bool shouldPrint);
int returnLastId(char *fileName, int fileType);

int main(void)
{
  readProducts();
  printf("\n\n");
  if (insertProduct() == true) {
  	printf("tes");
  	printf("\n\n\n");
  	readProducts();
  }
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

// TODO: Resolver problema de scanf pulando depois de inserir dados com espaços
// TODO: Testar GetById
// TODO: Implementar métodos de insert/get/getById para RequestItem e RequestHeader
bool insertProduct() {
	FILE *productFile;
	
	Product newProduct;
	
	productFile = fopen("product_data.bin", "a+");
	
	if (productFile == NULL){
		printf("erro");
		return false;
	}
	
	newProduct.id = returnLastId("product_data.bin", 1) + 1;
	
	printf("Insira o nome do Produto: ");
	scanf("%*s", newProduct.name);
	printf("\n\nInsira a descricao do Produto: ");
	scanf(" %*s", newProduct.desc);
	printf("\n\nInsira o preco do Produto: ");
	scanf("%*f", &newProduct.price);
	
	int writeAction = fwrite(&newProduct, sizeof(Product), 1, productFile);
	
	if (writeAction == 0) {
		printf("erro");
		return false;
	}
	
	fclose(productFile);
	
	return true;
}

/**
* Função que printa as informações de todos os produtos dentro do arquivo
*/
void readProducts() {
	FILE *productFile;
	Product product;
	
	productFile = fopen("product_data.bin", "r");
	
    if (productFile == NULL)
    {
        fprintf(stderr, "\nErro ao abrir o arquivo.\n");
        return;
    }
     
    while(fread(&product, sizeof(Product), 1, productFile))
        printf ("%d %s %s %.2f\n", 
				product.id,
        		product.name, 
				product.desc,
				product.price);
 
    fclose(productFile);
}

/**
* Função que retorna o produto baseado no Id
* @param int id = Id do produto
* @returns Product
*/
Product getProductById(int id, bool shouldPrint) {
	FILE *productFile;
	Product product;
	
	productFile = fopen("product_data.bin", "r");
	
    if (productFile == NULL)
    {
        fprintf(stderr, "\nErro ao abrir o arquivo.\n");
        return product;
    }
     
    while(fread(&product, sizeof(Product), 1, productFile)) {
    	if (id == product.id && shouldPrint == true) {
    		printf ("%d %s %s %.2f\n", 
				product.id,
        		product.name, 
				product.desc,
				product.price);	
		}
	}
 
    fclose(productFile);
    
    return product;
}

/**
* Função que retorna o id do último item cadastrado no arquivo
* @param char *fileName = Nome do arquivo com pontuação
* @param int fileType = Struct do arquivo para pesquisar sendo 1- Product, 2- Request Item e 3 - Request Header
* @returns int
*/
int returnLastId(char *fileName, int fileType) {
	FILE *fileToRead;
	int idToReturn = 0;
	
	fileToRead = fopen(fileName, "r");
	
	if (fileToRead == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        return 0;
    }
	
	switch (fileType) {
		case 1:
			Product product;
			fseek(fileToRead, sizeof(Product) * -1, SEEK_END);
			fread(&product, sizeof(Product), 1, fileToRead);
			idToReturn = product.id;
			break;
		case 2:
			break;
		case 3:
			break;
	}
	
	fclose(fileToRead);
	
	return idToReturn;
}



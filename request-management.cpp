#include <stdio.h>
#include <windows.h>
#include <conio.h>

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
RequestItem setRequestItemToObject();
void saveRequestItems();
bool insertRequestItem(RequestItem newReqItem);

int main(void)
{
  saveRequestItems();
  
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

// TODO: Implementar métodos de insert/get/getById para RequestItem e RequestHeader

/*
* Função que salva informações da struct Product para o arquivo bin
* @returns bool
*/
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
	scanf("%s", newProduct.name);
	printf("\n\nInsira a descricao do Produto: ");
	scanf(" %s", newProduct.desc);
	printf("\n\nInsira o preco do Produto: ");
	scanf("%f", &newProduct.price);
	
	int writeAction = fwrite(&newProduct, sizeof(Product), 1, productFile);
	
	if (writeAction == 0) {
		printf("erro");
		return false;
	}
	
	fclose(productFile);
	
	return true;
}

/*
* Função que salva informações da em uma instancia da struct RequestItem
* @returns RequestItem
*/
RequestItem setRequestItemToObject() {
	RequestItem newReqItem;

	newReqItem.id = returnLastId("requests_item_data.bin", 2) + 1;
	newReqItem.id_h_product = returnLastId("requests_header_data.bin", 2) + 1;

	printf("Insira o Id do Produto: ");
	scanf("%d", &newReqItem.id_product);
	printf("\n\nInsira a quantidade do Produto: ");
	scanf("%d", &newReqItem.amount);

	Product product = getProductById(newReqItem.id_product, false);
	
	newReqItem.total_price = product.price * newReqItem.amount;
	

	return newReqItem;
}

/*
* Função que faz um loop para adição de diversos itens de um pedido
* @returns void
*/
void saveRequestItems() {
	system("cls");
	int currentItem = 0;
	int loopRunning = 1;
	RequestItem* requestItensToAdd;

	requestItensToAdd = (RequestItem*)malloc(sizeof(RequestItem));

	while (loopRunning > 0) {
		RequestItem reqItem = setRequestItemToObject();

		requestItensToAdd[currentItem].id = reqItem.id;
		requestItensToAdd[currentItem].id_h_product = reqItem.id_h_product;
		requestItensToAdd[currentItem].id_product = reqItem.id_product;
		requestItensToAdd[currentItem].total_price = reqItem.total_price;
		requestItensToAdd[currentItem].amount = reqItem.amount;

		printf("Deseja inserir mais um item?\n\n");
		printf("1. Sim\n");
		printf("2. Nao");

		int pressedKey = getch();

		if (pressedKey == 49) {
			system("cls");
			currentItem++;
			requestItensToAdd = (RequestItem*)realloc(requestItensToAdd, sizeof(RequestItem) * currentItem + 1);
			continue;
		}
		else 
		{
			for (int i = 0; i <= currentItem;i++) {
				insertRequestItem(requestItensToAdd[i]);
			}
			//TODO: INSERT REQUEST HEADER
			loopRunning = 0;
		}
	}
}

/*
* Função que insere no arquivo um item de pedido
* @param RequestItem newReqItem = Instancia da struct para inserir no arquivo
* @returns bool
*/
bool insertRequestItem(RequestItem newReqItem) {
	FILE* requestItemFile;

	requestItemFile = fopen("requests_item_data.bin", "a+");

	if (requestItemFile == NULL) {
		return false;
	}

	int writeAction = fwrite(&newReqItem, sizeof(RequestItem), 1, requestItemFile);

	if (writeAction == 0) {
		return false;
	}

	fclose(requestItemFile);

	return true;
}

/*
* Função que printa as informações de todos os produtos dentro do arquivo
* @returns void
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

/*
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

/*
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
        //printf("\nErro ao abrir o arquivo.\n");
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
			RequestItem requestItem;
			fseek(fileToRead, sizeof(RequestItem) * -1, SEEK_END);
			fread(&requestItem, sizeof(RequestItem), 1, fileToRead);
			idToReturn = requestItem.id;
			break;
		case 3:
			break;
	}
	
	fclose(fileToRead);
	
	return idToReturn;
}



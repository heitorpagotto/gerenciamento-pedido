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
  float total_price;
  char client_name[50];
} RequestHeader;

void mainMenuRender();
bool insertProduct();
void readProducts();
Product getProductById(int id, bool shouldPrint);
int returnLastId(char *fileName, int fileType);
RequestItem setRequestItemToObject(int lastItemId);
void saveRequestItems();
bool insertRequestItem(RequestItem newReqItem);
bool insertRequestHeader(RequestItem* requestItems, int totalItemLength);
void readRequestItemByHeaderId(int headerId);
void readRequestHeaders();
RequestHeader getRequestHeaderById(int id, bool shouldPrint);
bool updateProduct();
int getTotalFileLength(char* fileName, int fileType);
bool updateRequestItem(int requestHeaderId);
bool updateRequestHeader(int requestHeaderId);
void deleteProduct();

int main(void)
{
	deleteProduct();
	readProducts();

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



/*
* Função que salva informações da struct Product para o arquivo bin
* @returns bool
*/
bool insertProduct() {
	FILE *productFile;
	
	Product newProduct;
	
	newProduct.id = returnLastId("product_data.bin", 1) + 1;

	productFile = fopen("product_data.bin", "a+");
	
	if (productFile == NULL){
		printf("erro");
		return false;
	}

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

	system("cls");
	printf("Produto adicionado com sucesso!");
	Sleep(1000);
	system("cls");
	
	return true;
}

/*
* Função que salva informações da em uma instancia da struct RequestItem
* @returns RequestItem
*/
RequestItem setRequestItemToObject(int lastItemId) {
	RequestItem newReqItem;

	newReqItem.id = lastItemId;
	newReqItem.id_h_product = returnLastId("requests_header_data.bin", 3) + 1;

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
	
	int lastItemId = returnLastId("requests_item_data.bin", 2) + 1;

	while (loopRunning > 0) {
		RequestItem reqItem = setRequestItemToObject(lastItemId);

		requestItensToAdd[currentItem].id = reqItem.id;
		requestItensToAdd[currentItem].id_h_product = reqItem.id_h_product;
		requestItensToAdd[currentItem].id_product = reqItem.id_product;
		requestItensToAdd[currentItem].total_price = reqItem.total_price;
		requestItensToAdd[currentItem].amount = reqItem.amount;

		printf("\nDeseja inserir mais um item?\n\n");
		printf("1. Sim\n");
		printf("2. Nao");

		int pressedKey = getch();

		if (pressedKey == 49) {
			system("cls");
			currentItem++;
			lastItemId++;
			requestItensToAdd = (RequestItem*)realloc(requestItensToAdd, sizeof(RequestItem) * (currentItem + 1));
			continue;
		}
		else 
		{
			for (int i = 0; i <= currentItem;i++) {
				insertRequestItem(requestItensToAdd[i]);
			}
			system("cls");
			insertRequestHeader(requestItensToAdd,currentItem+1);
			system("cls");
			loopRunning = 0;
			printf("Pedido cadastrado com sucesso!");
			Sleep(1000);
			system("cls");
		}
	}
}

/*
* Função que insere um header de um pedido, calculando o valor total baseado nos seus itens e no tamanho do array
* @param RequestItem* requestItems = Itens do pedido
* @param int totalItemLength = Tamanho total do array de itens do pedido
* @returns bool
*/
bool insertRequestHeader(RequestItem* requestItems, int totalItemLength) {
	float totalHeaderPrice = 0;

	for (int i = 0; i < totalItemLength; i++) {
		totalHeaderPrice += requestItems[i].total_price;
	}

	FILE* requestHeaderFile;
	RequestHeader newReqHeader;

	newReqHeader.id = requestItems[0].id_h_product;
	newReqHeader.total_price = totalHeaderPrice;

	printf("Insira o nome do cliente: ");
	scanf("%s", newReqHeader.client_name);

	requestHeaderFile = fopen("requests_header_data.bin", "a+");

	if (requestHeaderFile == NULL) {
		return false;
	}

	int writeAction = fwrite(&newReqHeader, sizeof(RequestHeader), 1, requestHeaderFile);

	if (writeAction == 0) {
		return false;
	}

	fclose(requestHeaderFile);

	return true;
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
        printf ("Id: %d | Nome do Produto: %s | Descricao: %s | Preco: R$ %.2f\n", 
				product.id,
        		product.name, 
				product.desc,
				product.price);
 
    fclose(productFile);
}

/*
* Função que printa as informações de todos os headers de pedidos dentro do arquivo
* @returns void
*/
void readRequestHeaders() {
	FILE* requestHeaderFile;
	RequestHeader requestHeader;

	requestHeaderFile = fopen("requests_header_data.bin", "r");

	if (requestHeaderFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return;
	}

	while (fread(&requestHeader, sizeof(RequestHeader), 1, requestHeaderFile))
		printf("Id: %d | Nome do Cliente: %s | Preco Total: R$ %.2f\n",
			requestHeader.id,
			requestHeader.client_name,
			requestHeader.total_price);

	fclose(requestHeaderFile);
}

/*
* Função que printa todos os itens de um pedido baseado no Id do header do pedido
* @param int headerId = Id do header do pedido
* @returns void
*/
void readRequestItemByHeaderId(int headerId) {
	FILE* requestItemFile;
	RequestItem requestItem;

	requestItemFile = fopen("requests_item_data.bin", "r");

	if (requestItemFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return;
	}

	while (fread(&requestItem, sizeof(RequestItem), 1, requestItemFile)) {
		if (requestItem.id_h_product = headerId) {
			Product product = getProductById(requestItem.id_product, false);
			printf("Id: %d | Id Pedido: %d | Id Produto: %d | Nome do Produto: %s | Quantidade: %d | R$%.2f\n",
				requestItem.id,
				requestItem.id_h_product,
				requestItem.id_product,
				product.name,
				requestItem.amount,
				requestItem.total_price);
		}
	}

	fclose(requestItemFile);
}

/*
* Função que retorna o produto baseado no Id
* @param int id = Id do produto
* @param bool shouldPrint = Boolean para caso se deva printar ou não o produto
* @returns Product
*/
Product getProductById(int id, bool shouldPrint) {
	FILE *productFile;
	Product product;
	
	productFile = fopen("product_data.bin", "r");
	
    if (productFile == NULL)
    {
        fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		product.id = -1;
        return product;
    }
     
    while(fread(&product, sizeof(Product), 1, productFile)) {
    	if (id == product.id && shouldPrint == true) {
    		printf ("Id: %d | Nome: %s | Descrição: %s | R$ %.2f\n", 
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
* Função que retorna o item de pedido por id, com habilidade de printar na tela
* @param int id = Id do item do pedido
* @param bool shouldPrint = Boolean para caso se deva printar ou não o item do pedido
* @returns RequestItem
*/
RequestItem getRequestItemById(int id, bool shouldPrint) {
	FILE* requestItemFile;
	RequestItem requestItem;

	requestItemFile = fopen("requests_item_data.bin", "r");

	if (requestItemFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return requestItem;
	}

	while (fread(&requestItem, sizeof(RequestItem), 1, requestItemFile)) {
		if (id == requestItem.id && shouldPrint == true) {
			Product product = getProductById(requestItem.id_product, false);
			printf("Id: %d | Id do Pedido: %d | Id do Produto: %d | Nome do Produto: %s | Quantidade: %d | R$%.2f\n",
				requestItem.id,
				requestItem.id_h_product,
				requestItem.id_product,
				product.name,
				requestItem.amount,
				requestItem.total_price);
		}
	}

	fclose(requestItemFile);

	return requestItem;
}

/*
* Função que busca o header de um pedido por id
* @param int id = Id do header
* @param bool shouldPrint = Boolean para caso se deva printar ou não o header do pedido
* @returns RequestHeader
*/
RequestHeader getRequestHeaderById(int id, bool shouldPrint) {
	FILE* requestHeaderFile;
	RequestHeader requestHeader;

	requestHeaderFile = fopen("requests_header_data.bin", "r");

	if (requestHeaderFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return requestHeader;
	}

	while (fread(&requestHeader, sizeof(RequestHeader), 1, requestHeaderFile)) {
		if (id == requestHeader.id && shouldPrint == true) {
			printf("Id: %d | Nome do Cliente: %s | R$%.2f\n",
				requestHeader.id,
				requestHeader.client_name,
				requestHeader.total_price);
		}
	}

	fclose(requestHeaderFile);

	return requestHeader;
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
			RequestHeader requestHeader;
			fseek(fileToRead, sizeof(RequestHeader) * -1, SEEK_END);
			fread(&requestHeader, sizeof(RequestHeader), 1, fileToRead);
			idToReturn = requestHeader.id;
			break;
	}

	//printf("%d", idToReturn);
	
	fclose(fileToRead);
	
	return idToReturn;
}

/*
* Função que atualiza produtos baseado no Id informado
* @returns bool
*/
bool updateProduct() {
	int idProd;
	readProducts();

	printf("\nInsira o Id do produto para editar: ");
	scanf("%d", &idProd);

	system("cls");

	getProductById(idProd, true);
	FILE* productFile = fopen("product_data.bin", "r+");
	Product currentProduct;

	if (productFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return false;
	}

	int currentIdx = 0;
	while (fread(&currentProduct, sizeof(Product), 1, productFile)) {
		if (idProd == currentProduct.id) {
			int treshHold = currentIdx * sizeof(Product);
			fseek(productFile, treshHold, SEEK_SET);

			printf("\n\nInsira o novo nome do produto: ");
			scanf("%s", currentProduct.name);
			printf("\nInsira a nova descricao do produto: ");
			scanf("%s", currentProduct.desc);
			printf("\nInsira o novo preco do produto: ");
			scanf("%f", &currentProduct.price);

			fwrite(&currentProduct, sizeof(Product), 1, productFile);
			break;
		}
		currentIdx++;
	}

	fclose(productFile);

	system("cls");
	printf("Produto atualizado com sucesso!");
	Sleep(1000);
	system("cls");

	return true;
}

/*
* Função que atualiza produtos baseado no Id informado
* @returns bool
*/
bool updateRequestItem(int requestHeaderId) {
	int idReqItem;
	readRequestItemByHeaderId(requestHeaderId);

	printf("\nInsira o Id do item para editar: ");
	scanf("%d", &idReqItem);
	
	system("cls");

	getRequestItemById(idReqItem, true);

	FILE* requestItemFile = fopen("requests_item_data.bin", "r+");
	RequestItem currentReqItem;

	if (requestItemFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return false;
	}

	int currentIdx = 0;
	while (fread(&currentReqItem, sizeof(RequestItem), 1, requestItemFile)) {
		if (idReqItem == currentReqItem.id) {
			int treshHold = currentIdx * sizeof(RequestItem);
			fseek(requestItemFile, treshHold, SEEK_SET);

			printf("Insira um Id de Produto: ");
			scanf("%d", &currentReqItem.id_product);
			printf("\n\nInsira uma nova quantidade de Produtos: ");
			scanf("%d", &currentReqItem.amount);

			Product product = getProductById(currentReqItem.id_product, false);

			currentReqItem.total_price = product.price * currentReqItem.amount;

			fwrite(&currentReqItem, sizeof(RequestItem), 1, requestItemFile);
			break;
		}
		currentIdx++;
	}

	fclose(requestItemFile);

	bool updatedFull = updateRequestHeader(requestHeaderId);
	
	if (updatedFull) {
		system("cls");
		printf("Pedido atualizado com sucesso!");
		Sleep(1000);
		system("cls");

		return true;
	}
	return false;
}

/*
* Função que atualiza o header de um pedido baseado no preço dos itens dos pedidos
* @param int requestHeaderId = Id do header do pedido
* @returns bool
*/
bool updateRequestHeader(int requestHeaderId) {
	RequestHeader reqHeader;

	FILE* requestItemFile = fopen("requests_item_data.bin", "r");
	RequestItem currentReqItem;

	if (requestItemFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return false;
	}

	float newTotal;
	while (fread(&currentReqItem, sizeof(RequestItem), 1, requestItemFile)) {
		if (requestHeaderId == currentReqItem.id_h_product) {
			newTotal += currentReqItem.total_price;
		}
	}

	fclose(requestItemFile);

	int currentIdx = 0;
	FILE* requestHeaderFile = fopen("requests_header_data.bin", "r+");
	if (requestHeaderFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return false;
	}
	while (fread(&reqHeader, sizeof(RequestHeader), 1, requestHeaderFile)) {
		if (requestHeaderId == reqHeader.id) {
			int treshHold = currentIdx * sizeof(RequestItem);
			fseek(requestItemFile, treshHold, SEEK_SET);
			
			reqHeader.total_price = newTotal;

			fwrite(&reqHeader, sizeof(RequestHeader), 1, requestHeaderFile);
			break;
		}
		currentIdx++;
	}

	fclose(requestHeaderFile);

	return true;
}

/*
* Função que retorna a quantidade de registros de um arquivo
* @param char* fileName = Nome do arquivo para pesquisar
* @param int fileType = Struct do arquivo para pesquisar sendo 1- Product, 2- Request Item e 3 - Request Header
* @returns int
*/
int getTotalFileLength(char* fileName, int fileType) {
	FILE* fileToRead;
	int totalLength = 0;

	fileToRead = fopen(fileName, "rb");
	if (fileToRead == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return 0;
	}
	fseek(fileToRead, 0, SEEK_END);

	switch (fileType) {
		case 1:
			totalLength = ftell(fileToRead) / sizeof(Product);
			break;
		case 2:
			totalLength = ftell(fileToRead) / sizeof(RequestItem);
			break;
		case 3:
			totalLength = ftell(fileToRead) / sizeof(RequestHeader);
			break;
	}

	fclose(fileToRead);

	return totalLength;
}

/*
* Função que deleta um produto do arquivo
* @returns void
*/
void deleteProduct() {
	int productId;
	system("cls");
	readProducts();

	printf("\n\nDigite o Id do produto a ser deletado: ");
	scanf("%d", &productId);

	system("cls");
	getProductById(productId, true);
	printf("\n\nTem certeza que deseja deletar o produto?\n");
	printf("1.Sim\n");
	printf("2.Nao\n");

	int confirmation = getch();

	if (confirmation == 49 || confirmation == 13) {
		FILE* productFile = fopen("product_data.bin", "r");
		FILE* productFileTemp = fopen("temp_file.bin", "w");
		Product product;

		while (fread(&product, sizeof(Product), 1, productFile)) {
			if (product.id != productId) {
				fwrite(&product, sizeof(Product), 1, productFileTemp);
			}
		}

		fclose(productFile);
		fclose(productFileTemp);

		productFile = fopen("product_data.bin", "w");
		productFileTemp = fopen("temp_file.bin", "r");

		while (fread(&product, sizeof(Product), 1, productFileTemp)) {
			fwrite(&product, sizeof(Product), 1, productFile);
		}

		fclose(productFile);
		fclose(productFileTemp);
		remove("temp_file.bin");
	}
}
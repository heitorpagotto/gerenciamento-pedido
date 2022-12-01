#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>

// product_data.dat
typedef struct
{
	int id;
	char name[50];
	float price;
	char desc[150];
} Product;

// requests_item_data.dat
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
void readProducts(bool shouldReturnToMenu);
Product getProductById(int id, bool shouldPrint);
int returnLastId(char *fileName, int fileType);
RequestItem setRequestItemToObject(int lastItemId);
void saveRequestItems();
bool insertRequestItem(RequestItem newReqItem);
bool insertRequestHeader(RequestItem *requestItems, int totalItemLength);
void readRequestItemByHeaderId(int headerId);
void readRequestHeaders();
RequestHeader getRequestHeaderById(int id, bool shouldPrint);
bool updateProduct();
int getTotalFileLength(char *fileName, int fileType);
bool updateRequestItem(int requestHeaderId);
bool updateRequestHeader(int requestHeaderId);
bool deleteProduct();
bool deleteRequestItem(int requestHeaderId, bool deleteAll);
bool deleteRequest();
void genericOptionsRender();
void renderProductMenu();
void renderErrorMessage();
void renderRequestMenu();

int main(void)
{
	setlocale(LC_ALL, "Portuguese");
	mainMenuRender();
	return 0;
}

void mainMenuRender()
{
	printf("Selecione o que gostaria de gerenciar:\n\n");
	printf("1.Pedidos\n");
	printf("2.Produtos\n");
	printf("3.Sair");

	int keyCode;

	keyCode = getch();

	system("cls");

	if (keyCode == 49)
	{
		renderRequestMenu();
	}
	if (keyCode == 50)
	{
		renderProductMenu();
	}
	if (keyCode == 51)
	{
		return;
	}
}

void renderProductMenu()
{
	printf("Selecione uma ação para executar:\n\n");
	genericOptionsRender();

	int keyCode = getch();

	system("cls");

	if (keyCode == 49)
	{
		bool insert = insertProduct();

		if (insert == false)
		{
			renderErrorMessage();
			return;
		}
		else
		{
			renderProductMenu();
		}
	}

	if (keyCode == 50)
	{
		readProducts(true);
	}

	if (keyCode == 51)
	{
		bool updateResult = updateProduct();
		if (updateResult == false)
		{
			renderErrorMessage();
			return;
		}
		else
		{
			renderProductMenu();
		}
	}

	if (keyCode == 52)
	{
		bool deleteResult = deleteProduct();
		if (deleteResult == false)
		{
			renderErrorMessage();
			return;
		}
		else
		{
			renderProductMenu();
		}
	}

	if (keyCode == 53)
	{
		system("cls");
		mainMenuRender();
	}
}

void renderRequestMenu()
{
	printf("Selecione uma ação para executar:\n\n");
	genericOptionsRender();

	int keyCode = getch();

	system("cls");

	if (keyCode == 49)
	{
		saveRequestItems();
		system("cls");
		renderRequestMenu();
	}

	if (keyCode == 50)
	{
		readRequestHeaders();
		printf("\nDeseja ver itens de um pedido?\n\n");
		printf("1.Sim\n");
		printf("2.Não\n");
		int confirmationCode = getch();

		if (confirmationCode == 49)
		{
			int requestId;
			printf("\nInsira o id do pedido: ");
			scanf("%d", &requestId);
			system("cls");
			readRequestItemByHeaderId(requestId);
			printf("\n\nPressione [ENTER] para retornar...");
			int enterKey = getch();
			if (enterKey == 13)
			{
				system("cls");
				renderRequestMenu();
			}
		}
		else
		{
			system("cls");
			renderRequestMenu();
		}
	}

	if (keyCode == 51)
	{
		readRequestHeaders();
		printf("\n\nInisira o Id do pedido para editar: ");
		int requestHeaderId;
		scanf("%d", &requestHeaderId);
		system("cls");
		bool updateResult = updateRequestItem(requestHeaderId);
		if (updateResult == true)
		{
			system("cls");
			renderRequestMenu();
		}
		else
		{
			renderErrorMessage();
			return;
		}
	}

	if (keyCode == 52)
	{
		bool deleteResult = deleteRequest();
		if (deleteResult == true)
		{
			system("cls");
			renderRequestMenu();
		}
		else
		{
			renderErrorMessage();
			return;
		}
	}

	if (keyCode == 53)
	{
		system("cls");
		mainMenuRender();
	}
}

void renderErrorMessage()
{
	system("cls");
	printf("Ação não executada!");
	Sleep(1000);
	system("cls");
	mainMenuRender();
}

void genericOptionsRender()
{
	printf("1.Adicionar\n");
	printf("2.Consultar\n");
	printf("3.Atualizar\n");
	printf("4.Deletar\n");
	printf("5.Sair");
}

/*
 * Fun��o que salva informa��es da struct Product para o arquivo bin
 * @returns bool
 */
bool insertProduct()
{
	FILE *productFile;

	Product newProduct;

	newProduct.id = returnLastId("product_data.bin", 1) + 1;

	productFile = fopen("product_data.bin", "a+");

	if (productFile == NULL)
	{
		return false;
	}

	printf("Insira o nome do Produto: ");
	scanf("%s", newProduct.name);
	printf("\n\nInsira a descrição do Produto: ");
	scanf(" %s", newProduct.desc);
	printf("\n\nInsira o preco do Produto: ");
	scanf("%f", &newProduct.price);

	int writeAction = fwrite(&newProduct, sizeof(Product), 1, productFile);

	if (writeAction == 0)
	{
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
 * Fun��o que salva informa��es da em uma instancia da struct RequestItem
 * @returns RequestItem
 */
RequestItem setRequestItemToObject(int lastItemId)
{
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
 * Fun��o que faz um loop para adi��o de diversos itens de um pedido
 * @returns void
 */
void saveRequestItems()
{
	system("cls");
	int currentItem = 0;
	int loopRunning = 1;
	RequestItem *requestItensToAdd;

	requestItensToAdd = (RequestItem *)malloc(sizeof(RequestItem));

	int lastItemId = returnLastId("requests_item_data.bin", 2) + 1;

	while (loopRunning > 0)
	{
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

		if (pressedKey == 49)
		{
			system("cls");
			currentItem++;
			lastItemId++;
			requestItensToAdd = (RequestItem *)realloc(requestItensToAdd, sizeof(RequestItem) * (currentItem + 1));
			continue;
		}
		else
		{
			for (int i = 0; i <= currentItem; i++)
			{
				insertRequestItem(requestItensToAdd[i]);
			}
			system("cls");
			insertRequestHeader(requestItensToAdd, currentItem + 1);
			system("cls");
			loopRunning = 0;
			printf("Pedido cadastrado com sucesso!");
			Sleep(1000);
			system("cls");
		}
	}
}

/*
 * Fun��o que insere um header de um pedido, calculando o valor total baseado nos seus itens e no tamanho do array
 * @param RequestItem* requestItems = Itens do pedido
 * @param int totalItemLength = Tamanho total do array de itens do pedido
 * @returns bool
 */
bool insertRequestHeader(RequestItem *requestItems, int totalItemLength)
{
	float totalHeaderPrice = 0;

	for (int i = 0; i < totalItemLength; i++)
	{
		totalHeaderPrice += requestItems[i].total_price;
	}

	FILE *requestHeaderFile;
	RequestHeader newReqHeader;

	newReqHeader.id = requestItems[0].id_h_product;
	newReqHeader.total_price = totalHeaderPrice;

	printf("Insira o nome do cliente: ");
	scanf("%s", newReqHeader.client_name);

	requestHeaderFile = fopen("requests_header_data.bin", "a+");

	if (requestHeaderFile == NULL)
	{
		return false;
	}

	int writeAction = fwrite(&newReqHeader, sizeof(RequestHeader), 1, requestHeaderFile);

	if (writeAction == 0)
	{
		return false;
	}

	fclose(requestHeaderFile);

	return true;
}

/*
 * Fun��o que insere no arquivo um item de pedido
 * @param RequestItem newReqItem = Instancia da struct para inserir no arquivo
 * @returns bool
 */
bool insertRequestItem(RequestItem newReqItem)
{
	FILE *requestItemFile;

	requestItemFile = fopen("requests_item_data.bin", "a+");

	if (requestItemFile == NULL)
	{
		return false;
	}

	int writeAction = fwrite(&newReqItem, sizeof(RequestItem), 1, requestItemFile);
	if (writeAction == 0)
	{
		return false;
	}

	fclose(requestItemFile);
	return true;
}

/*
 * Fun��o que printa as informa��es de todos os produtos dentro do arquivo
 * @returns void
 */
void readProducts(bool shouldReturnToMenu)
{
	FILE *productFile;
	Product product;

	productFile = fopen("product_data.bin", "r");

	if (productFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return;
	}

	while (fread(&product, sizeof(Product), 1, productFile))
		printf("Id: %d | Nome do Produto: %s | Descrição: %s | Preço: R$ %.2f\n",
					 product.id,
					 product.name,
					 product.desc,
					 product.price);

	fclose(productFile);

	if (shouldReturnToMenu == true)
	{
		printf("\n");
		printf("Pressione [ENTER] para retornar...");
		int finishedCode = getch();

		if (finishedCode == 13)
		{
			system("cls");
			renderProductMenu();
		}
	}
}

/*
 * Fun��o que printa as informa��es de todos os headers de pedidos dentro do arquivo
 * @returns void
 */
void readRequestHeaders()
{
	FILE *requestHeaderFile;
	RequestHeader requestHeader;

	requestHeaderFile = fopen("requests_header_data.bin", "r");

	if (requestHeaderFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return;
	}

	while (fread(&requestHeader, sizeof(RequestHeader), 1, requestHeaderFile))
		printf("Id: %d | Nome do Cliente: %s | Preço Total: R$ %.2f\n",
					 requestHeader.id,
					 requestHeader.client_name,
					 requestHeader.total_price);

	fclose(requestHeaderFile);
}

/*
 * Fun��o que printa todos os itens de um pedido baseado no Id do header do pedido
 * @param int headerId = Id do header do pedido
 * @returns void
 */
void readRequestItemByHeaderId(int headerId)
{
	FILE *requestItemFile;
	RequestItem requestItem;

	requestItemFile = fopen("requests_item_data.bin", "r");

	if (requestItemFile == NULL)
	{
		fprintf(stderr, "Erro ao abrir o arquivo.\n");
		return;
	}

	while (fread(&requestItem, sizeof(RequestItem), 1, requestItemFile))
	{
		if (requestItem.id_h_product == headerId)
		{
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
 * Fun��o que retorna o produto baseado no Id
 * @param int id = Id do produto
 * @param bool shouldPrint = Boolean para caso se deva printar ou n�o o produto
 * @returns Product
 */
Product getProductById(int id, bool shouldPrint)
{
	FILE *productFile;
	Product product;

	productFile = fopen("product_data.bin", "r");

	if (productFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		product.id = -1;
		return product;
	}

	while (fread(&product, sizeof(Product), 1, productFile))
	{
		if (id == product.id && shouldPrint == true)
		{
			printf("Id: %d | Nome: %s | Descrição: %s | R$ %.2f\n",
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
 * Fun��o que retorna o item de pedido por id, com habilidade de printar na tela
 * @param int id = Id do item do pedido
 * @param bool shouldPrint = Boolean para caso se deva printar ou n�o o item do pedido
 * @returns RequestItem
 */
RequestItem getRequestItemById(int id, bool shouldPrint)
{
	FILE *requestItemFile;
	RequestItem requestItem;

	requestItemFile = fopen("requests_item_data.bin", "r");

	if (requestItemFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return requestItem;
	}

	while (fread(&requestItem, sizeof(RequestItem), 1, requestItemFile))
	{
		if (id == requestItem.id && shouldPrint == true)
		{
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
 * Fun��o que busca o header de um pedido por id
 * @param int id = Id do header
 * @param bool shouldPrint = Boolean para caso se deva printar ou n�o o header do pedido
 * @returns RequestHeader
 */
RequestHeader getRequestHeaderById(int id, bool shouldPrint)
{
	FILE *requestHeaderFile;
	RequestHeader requestHeader;

	requestHeaderFile = fopen("requests_header_data.bin", "r");

	if (requestHeaderFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return requestHeader;
	}

	while (fread(&requestHeader, sizeof(RequestHeader), 1, requestHeaderFile))
	{
		if (id == requestHeader.id && shouldPrint == true)
		{
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
 * Fun��o que retorna o id do �ltimo item cadastrado no arquivo
 * @param char *fileName = Nome do arquivo com pontua��o
 * @param int fileType = Struct do arquivo para pesquisar sendo 1- Product, 2- Request Item e 3 - Request Header
 * @returns int
 */
int returnLastId(char *fileName, int fileType)
{
	FILE *fileToRead;
	int idToReturn = 0;

	fileToRead = fopen(fileName, "r");

	if (fileToRead == NULL)
	{
		return 0;
	}

	switch (fileType)
	{
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

	// printf("%d", idToReturn);

	fclose(fileToRead);

	return idToReturn;
}

/*
 * Fun��o que atualiza produtos baseado no Id informado
 * @returns bool
 */
bool updateProduct()
{
	int idProd;
	readProducts(false);

	printf("\nInsira o Id do produto para editar: ");
	scanf("%d", &idProd);

	system("cls");

	getProductById(idProd, true);
	FILE *productFile = fopen("product_data.bin", "r+");
	Product currentProduct;

	if (productFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return false;
	}

	int currentIdx = 0;
	while (fread(&currentProduct, sizeof(Product), 1, productFile))
	{
		if (idProd == currentProduct.id)
		{
			int treshHold = currentIdx * sizeof(Product);
			fseek(productFile, treshHold, SEEK_SET);

			printf("\n\nInsira o novo nome do produto: ");
			scanf("%s", currentProduct.name);
			printf("\nInsira a nova descrição do produto: ");
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
 * Fun��o que atualiza produtos baseado no Id informado
 * @returns bool
 */
bool updateRequestItem(int requestHeaderId)
{
	int idReqItem;
	readRequestItemByHeaderId(requestHeaderId);

	printf("\n\nInsira o Id do item para editar: ");
	scanf("%d", &idReqItem);

	system("cls");

	getRequestItemById(idReqItem, true);

	FILE *requestItemFile = fopen("requests_item_data.bin", "r+");
	RequestItem currentReqItem;

	if (requestItemFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return false;
	}

	int currentIdx = 0;
	while (fread(&currentReqItem, sizeof(RequestItem), 1, requestItemFile))
	{
		if (idReqItem == currentReqItem.id)
		{
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

	if (updatedFull)
	{
		system("cls");
		printf("Pedido atualizado com sucesso!");
		Sleep(1000);
		system("cls");

		return true;
	}
	return false;
}

/*
 * Fun��o que atualiza o header de um pedido baseado no pre�o dos itens dos pedidos
 * @param int requestHeaderId = Id do header do pedido
 * @returns bool
 */
bool updateRequestHeader(int requestHeaderId)
{
	RequestHeader reqHeader;

	FILE *requestItemFile = fopen("requests_item_data.bin", "r");
	RequestItem currentReqItem;

	if (requestItemFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return false;
	}

	float newTotal;
	while (fread(&currentReqItem, sizeof(RequestItem), 1, requestItemFile))
	{
		if (requestHeaderId == currentReqItem.id_h_product)
		{
			newTotal += currentReqItem.total_price;
		}
	}

	fclose(requestItemFile);

	int currentIdx = 0;
	FILE *requestHeaderFile = fopen("requests_header_data.bin", "r+");
	if (requestHeaderFile == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return false;
	}
	while (fread(&reqHeader, sizeof(RequestHeader), 1, requestHeaderFile))
	{
		if (requestHeaderId == reqHeader.id)
		{
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
 * Fun��o que retorna a quantidade de registros de um arquivo
 * @param char* fileName = Nome do arquivo para pesquisar
 * @param int fileType = Struct do arquivo para pesquisar sendo 1- Product, 2- Request Item e 3 - Request Header
 * @returns int
 */
int getTotalFileLength(char *fileName, int fileType)
{
	FILE *fileToRead;
	int totalLength = 0;

	fileToRead = fopen(fileName, "rb");
	if (fileToRead == NULL)
	{
		fprintf(stderr, "\nErro ao abrir o arquivo.\n");
		return 0;
	}
	fseek(fileToRead, 0, SEEK_END);

	switch (fileType)
	{
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
 * Fun��o que deleta um produto do arquivo
 * @returns bool
 */
bool deleteProduct()
{
	int productId;
	system("cls");
	readProducts(false);

	printf("\n\nDigite o Id do produto a ser deletado: ");
	scanf("%d", &productId);

	system("cls");
	getProductById(productId, true);
	printf("\n\nTem certeza que deseja deletar o produto?\n");
	printf("1.Sim\n");
	printf("2.Nao\n");

	int confirmation = getch();

	if (confirmation == 49 || confirmation == 13)
	{
		FILE *productFile = fopen("product_data.bin", "r");
		FILE *productFileTemp = fopen("temp_file.bin", "w");
		Product product;

		while (fread(&product, sizeof(Product), 1, productFile))
		{
			if (product.id != productId)
			{
				fwrite(&product, sizeof(Product), 1, productFileTemp);
			}
		}

		fclose(productFile);
		fclose(productFileTemp);

		productFile = fopen("product_data.bin", "w");
		productFileTemp = fopen("temp_file.bin", "r");

		while (fread(&product, sizeof(Product), 1, productFileTemp))
		{
			fwrite(&product, sizeof(Product), 1, productFile);
		}

		fclose(productFile);
		fclose(productFileTemp);
		remove("temp_file.bin");
		system("cls");
		printf("Produto deletado com sucesso!");
		Sleep(1000);
		system("cls");

		return true;
	}
	else
	{
		system("cls");
		return false;
	}
	return false;
}

/*
 * Fun��o que deleta os itens do pedido
 * @returns bool
 */
bool deleteRequestItem(int requestHeaderId, bool deleteAll)
{
	FILE *requestItemFile = fopen("requests_item_data.bin", "r");
	FILE *requestItemTempFile = fopen("temp_file.bin", "w");

	if (!deleteAll)
	{
		int itemToDeleteId;
		readRequestItemByHeaderId(requestHeaderId);
		printf("\n\nDigite o Id do item que deseja deletar: ");
		scanf("%d", &itemToDeleteId);

		system("cls");

		getRequestItemById(itemToDeleteId, true);

		printf("\n\nTem certeza que deseja deletar o item do pedido?\n");
		printf("1.Sim\n");
		printf("2.Nao\n");

		int confirmation = getch();

		if (confirmation == 49 || confirmation == 13)
		{
			RequestItem requestItem;

			while (fread(&requestItem, sizeof(RequestItem), 1, requestItemFile))
			{
				if (requestItem.id != itemToDeleteId)
				{
					fwrite(&requestItem, sizeof(RequestItem), 1, requestItemTempFile);
				}
			}

			fclose(requestItemFile);
			fclose(requestItemTempFile);

			requestItemFile = fopen("requests_item_data.bin", "w");
			requestItemTempFile = fopen("temp_file.bin", "r");

			while (fread(&requestItem, sizeof(RequestItem), 1, requestItemTempFile))
			{
				fwrite(&requestItem, sizeof(RequestItem), 1, requestItemFile);
			}

			fclose(requestItemFile);
			fclose(requestItemTempFile);
			remove("temp_file.bin");
			updateRequestHeader(requestHeaderId);

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		RequestItem requestItem;

		while (fread(&requestItem, sizeof(RequestItem), 1, requestItemFile))
		{
			printf("\n\nteste id: %d", requestItem.id_h_product);
			if (requestItem.id_h_product != requestHeaderId)
			{
				fwrite(&requestItem, sizeof(RequestItem), 1, requestItemTempFile);
			}
		}

		fclose(requestItemFile);
		fclose(requestItemTempFile);

		requestItemFile = fopen("requests_item_data.bin", "w");
		requestItemTempFile = fopen("temp_file.bin", "r");

		while (fread(&requestItem, sizeof(RequestItem), 1, requestItemTempFile))
		{
			fwrite(&requestItem, sizeof(RequestItem), 1, requestItemFile);
		}

		fclose(requestItemFile);
		fclose(requestItemTempFile);
		remove("temp_file.bin");

		return true;
	}
	return false;
}

/*
 * Fun��o que deleta o pedido ou itens do pedido
 * @returns bool
 */
bool deleteRequest()
{
	int requestId;
	readRequestHeaders();

	printf("\n\nDigite o Id do Pedido para deletar: ");
	scanf("%d", &requestId);

	system("cls");

	printf("O que deseja deletar?\n\n");
	printf("1.Pedido\n");
	printf("2.Item do Pedido\n");

	int optionSelected = getch();

	if (optionSelected != 49)
	{
		deleteRequestItem(requestId, false);
	}
	else
	{
		FILE *requestHeaderFile = fopen("requests_header_data.bin", "r");
		FILE *requestHeaderTempFile = fopen("temp_file.bin", "w");

		system("cls");

		printf("Tem certeza que deseja deletar o pedido?\n");
		printf("1.Sim\n");
		printf("2.Nao\n");

		int confirmation = getch();

		if (confirmation == 49 || confirmation == 13)
		{
			RequestHeader requestHeader;

			while (fread(&requestHeader, sizeof(RequestHeader), 1, requestHeaderFile))
			{
				if (requestHeader.id != requestId)
				{
					fwrite(&requestHeader, sizeof(RequestHeader), 1, requestHeaderTempFile);
				}
			}

			fclose(requestHeaderFile);
			fclose(requestHeaderTempFile);

			requestHeaderFile = fopen("requests_header_data.bin", "w");
			requestHeaderTempFile = fopen("temp_file.bin", "r");

			while (fread(&requestHeader, sizeof(RequestHeader), 1, requestHeaderTempFile))
			{
				fwrite(&requestHeader, sizeof(RequestHeader), 1, requestHeaderFile);
			}

			fclose(requestHeaderFile);
			fclose(requestHeaderTempFile);
			remove("temp_file.bin");

			deleteRequestItem(requestId, true);

			return true;
		}
		else
		{
			system("cls");
			return false;
		}
	}
	return false;
}
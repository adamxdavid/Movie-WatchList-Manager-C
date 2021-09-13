#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _movienode
{
	char title[35];
	char genre[35];
	double duration;
	struct _movienode *next;
} movieNode;

void freeList(movieNode *);
void printmovieInfo(movieNode *);
void printList(movieNode *);
double computeDuration(movieNode *);
movieNode *removemovie(movieNode **, movieNode *);
int deletemovie(movieNode **, movieNode *);
movieNode *searchByTitle(movieNode *, char *);
movieNode *createmovieNode(char *, char *, double);
int insertmovie(movieNode **head, movieNode *newNode, int position);
int getNodePosition(movieNode *head, char *search);
int savewatchlist(movieNode *head);
movieNode *loadwatchlist();
void alphabeticOrder(movieNode **head, movieNode *newNode);
movieNode *readTextFile(char *path);
void AddMenu(movieNode **watchList, movieNode *insertNode);
void libraryMenu(movieNode **libray, movieNode **watchlist);
void watchListMenu(char *filename);

int main(int argc, char **argv)
{
	watchListMenu(argv[1]);
	return 0;
}

/*traverses the linked list and frees the
allocated memory*/
void freeList(movieNode *head)
{
	movieNode *tmp = NULL;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}
/*traverses the linked list and prints the node*/
void printList(movieNode *head)
{
	movieNode *current = NULL;
	current = head;
	while (current != NULL)
	{
		printmovieInfo(current);
		current = current->next;
	}
}
/*reading the attributes of a node and printing it*/
void printmovieInfo(movieNode *movie)
{
	printf("%s %s %.2lf\n", movie->title, movie->genre, movie->duration);
}
/*traverses the linked list and sums the duration attribute*/
double computeDuration(movieNode *head)
{
	double sum = 0;
	movieNode *current = head;
	while (current != NULL)
	{
		sum += current->duration;
		current = current->next;
	}
	return sum;
}
/*removes a node from the linked list, double pointer in order to potential reassign a header*/
movieNode *removemovie(movieNode **head, movieNode *remNode)
{
	movieNode *tmp = NULL;
	movieNode *tmp2 = NULL;
	if (*head == NULL || remNode == NULL)
		return NULL;

	tmp = *head;
	if (*head == remNode)
	{
		*head = (*head)->next;
		tmp->next = NULL;
		return tmp;
	}

	while (tmp != NULL)
	{
		if (tmp->next == remNode)
		{
			tmp2 = tmp->next;
			tmp->next = tmp->next->next;
			tmp2->next = NULL;
			return tmp2;
		}
		tmp = tmp->next;
	}

	return NULL;
}
/*removes movie, deallocates*/
int deletemovie(movieNode **head, movieNode *delNode)
{
	movieNode *retMov = NULL;
	retMov = removemovie(head, delNode);
	if (retMov == NULL)
		return -1;
	free(retMov);
	return 0;
}
/*traverses list, and uses string compare to find by title*/
movieNode *searchByTitle(movieNode *head, char *search)
{
	int i = 0;
	movieNode *current = head;
	if (head == NULL || search == NULL)
		return NULL;

	char *str = malloc(sizeof(char) * (strlen(search) + 1));
	for (i = 0; search[i] != '\0'; i++)
	{
		if (search[i] >= 'a' && search[i] <= 'z')
		{
			str[i] = search[i] - 32;
		}
		else
		{
			str[i] = search[i];
		}
	}
	str[i] = '\0';
	while (current != NULL)
	{
		if (strcmp(current->title, str) == 0){
			free(str);
			return current;
		}
		current = current->next;
	}
	free(str);
	return NULL;
}
/*allocates memory and then assigns passed in attributes*/
movieNode *createmovieNode(char *title, char *genre, double duration)
{
	movieNode *newMov = NULL;
	if ((newMov = malloc(sizeof(movieNode))) == NULL)
	{
		printf("Cant allocate memory");
		return NULL;
	}
	strcpy(newMov->title, title);
	strcpy(newMov->genre, genre);
	newMov->duration = duration;
	newMov->next = NULL;
	return newMov;
}
/*traverses the linked list and adds a node at a position*/
int insertmovie(movieNode **head, movieNode *newNode, int position)
{
	movieNode *current = NULL;
	int i = 0;

	if (*head == NULL)
	{
		*head = newNode;
		return 0;
	}
	if (position == 0)
	{
		newNode->next = *head;
		*head = newNode;
		return 0;
	}
	current = *head;
	for (i = 0; i < position - 1; i++)
	{
		if (current->next == NULL)
			break;
		current = current->next;
	}

	newNode->next = current->next;
	current->next = newNode;

	return i + 1;
}
/*traverses the linked list and finds the same node, returns positions*/
int getNodePosition(movieNode *head, char *search)
{
	int i = 0;
	movieNode *current = head;
	if (head == NULL || search == NULL)
		return -1;

	char *str = malloc(sizeof(char) * (strlen(search) + 1));
	for (i = 0; search[i] != '\0'; i++)
	{
		if (search[i] >= 'a' && search[i] <= 'z')
		{
			str[i] = search[i] - 32;
		}
		else
		{
			str[i] = search[i];
		}
	}
	str[i] = '\0';
	i = 0;
	while (current != NULL)
	{
		if (strcmp(current->title, str) == 0){
			free(str);
			return i;}
		i++;
		current = current->next;
	}
	free(str);
	return -1;
}
/*traverses the list, prints out values to a file similar to example*/
int savewatchlist(movieNode *head)
{
	char filename[128];
	char input[128];
	FILE *fp = NULL;
	movieNode *current = head;
	printf("Enter filename: ");
	fgets(input,128,stdin);
	sscanf(input,"%s", filename);
	

	if ((fp = fopen(filename, "w")) == NULL)
	{
		printf("cannot create %s ", filename);
		return -1;
	}
	while (current != NULL)
	{
		fprintf(fp, "%s\r\n", current->title);
		fprintf(fp, "%s\r\n", current->genre);
		if (current->next == NULL)
		{
			fprintf(fp, "%.2lf", current->duration);
		}
		else
			fprintf(fp, "%.2lf\r\n", current->duration);
		current = current->next;
	}
	fclose(fp);
	return 0;
}
/*line by line, it takes in input from file, assigns to node attributes
takes into account an empty file or badly formatted file*/
movieNode *loadwatchlist()
{
	movieNode *head = NULL;
	movieNode *current = NULL;
	FILE *fp = NULL;
	char filename[128];
	char input[128];
	char temp[50];
	char title[35];
	char genre[35];
	double duration = 0;

	printf("Enter filename: ");
	fgets(input,128,stdin);
	sscanf(input,"%s", filename);

	if ((fp = fopen(filename, "r")) == NULL)
	{
		printf("%s not found", filename);
		return NULL;
	}

	if (fgets(temp, 50, fp) == NULL)
		return head;
	if (sscanf(temp, "%[^\r\n]s", title) == 0)
		return head;

	if (fgets(temp, 50, fp) == NULL)
		return head;
	if (sscanf(temp, "%[^\r\n]s", genre) == 0)
		return head;

	if (fgets(temp, 50, fp) == NULL)
		return head;
	if (sscanf(temp, "%lf", &duration) == 0)
		return head;
	head = createmovieNode(title, genre, duration);
	current = head;

	while (!feof(fp))
	{
		if (fgets(temp, 50, fp) == NULL)
			return head;
		if (sscanf(temp, "%[^\r\n]s", title) == 0)
			return head;

		if (fgets(temp, 50, fp) == NULL)
			return head;
		if (sscanf(temp, "%[^\r\n]s", genre) == 0)
			return head;

		if (fgets(temp, 50, fp) == NULL)
			return head;
		if (sscanf(temp, "%lf", &duration) == 0)
			return head;
		current->next = createmovieNode(title, genre, duration);
		current = current->next;
	}
	fclose(fp);
	return head;
}
/*line by line, it takes in input from file, assigns to node attributes
takes into account an empty file or badly formatted file*/
movieNode *readTextFile(char *path)
{
	movieNode *head = NULL;
	FILE *fp = NULL;
	char temp[50];
	char title[35];
	char genre[35];
	double duration = 0;

	if ((fp = fopen(path, "r")) == NULL)
	{
		printf("%s not found", path);
		exit(1);
	}

	if (fgets(temp, 50, fp) == NULL)
		return head;
	if (sscanf(temp, "%[^\r\n]s", title) == 0)
		return head;

	if (fgets(temp, 50, fp) == NULL)
		return head;
	if (sscanf(temp, "%[^\r\n]s", genre) == 0)
		return head;

	if (fgets(temp, 50, fp) == NULL)
		return head;
	if (sscanf(temp, "%lf", &duration) == 0)
		return head;
	head = createmovieNode(title, genre, duration);

	while (!feof(fp))
	{
		if (fgets(temp, 50, fp) == NULL)
			return head;
		if (sscanf(temp, "%[^\r\n]s", title) == 0)
			return head;

		if (fgets(temp, 50, fp) == NULL)
			return head;
		if (sscanf(temp, "%[^\r\n]s", genre) == 0)
			return head;

		if (fgets(temp, 50, fp) == NULL)
			return head;
		if (sscanf(temp, "%lf", &duration) == 0)
			return head;
		alphabeticOrder(&head, createmovieNode(title, genre, duration));
	}
	fclose(fp);
	return head;
}
/*sorts nodes into order using titles*/
void alphabeticOrder(movieNode **head, movieNode *newNode)
{
	movieNode *Current = *head;
	if (head == NULL || newNode == NULL)
		return;

	if (strcmp(Current->title, newNode->title) >= 0)
	{
		newNode->next = *head;
		*head = newNode;
		return;
	}
	while (Current->next != NULL)
	{
		if (strcmp(Current->next->title, newNode->title) >= 0)
		{
			break;
		}
		Current = Current->next;
	}
	newNode->next = Current->next;
	Current->next = newNode;
}
/*allows user to enter number, and then navigate to different functions*/
void watchListMenu(char *filename)
{
	movieNode *watchLibraryHead = NULL, *watchListHead = NULL,*movie=NULL;
	movieNode *node = NULL;
	int choice = 0;
	char tmp[128],title[35],input[128];
	int pos=0;
	int x=1;
	watchLibraryHead = readTextFile(filename);

	while (x == 1)
	{

		printf("1. Print watchlist\n");
		printf("2. Show duration\n");
		printf("3. Search by title\n");
		printf("4. Move a movie up\n");
		printf("5. Move a movie down\n");
		printf("6. Remove a movie\n");
		printf("7. Save watchlist\n");
		printf("8. Load watchlist\n");
		printf("9. Go to movie Library\n");
		printf("10. Quit\n");

		fgets(input,128,stdin);
		sscanf(input,"%d", &choice);
		switch (choice)
		{
		case 1:
			printList(watchListHead);
			break;
		case 2:
			printf("%.2lf\n", computeDuration(watchListHead));
			break;
		case 3:
			printf("Movie Title: ");
			fgets (tmp, 35, stdin);
			sscanf(tmp,"%[^\n]s", title);
			movie=searchByTitle(watchListHead, title);
			if(movie!=NULL){
				printmovieInfo(movie);
			}else printf("Movie doesnt exist\n");
			break;
		case 4:
			printf("Movie Title: ");
			fgets (tmp, 35, stdin);
			sscanf(tmp,"%[^\n]s", title);
			pos = getNodePosition(watchListHead, title);
			if (pos > 0)
			{
				node = removemovie(&watchListHead, searchByTitle(watchListHead, title));
				insertmovie(&watchListHead, node, pos - 1);
			}
			break;
		case 5:
			printf("Movie Title: ");
			fgets (tmp, 35, stdin);
			sscanf(tmp,"%[^\n]s", title);
			pos = getNodePosition(watchListHead, title);
			if (pos > 0)
			{
				node = removemovie(&watchListHead, searchByTitle(watchListHead, title));
				insertmovie(&watchListHead, node, pos + 1);
			}
			break;
		case 6:
			printf("Movie Title: ");
			fgets (tmp, 35, stdin);
			sscanf(tmp,"%[^\n]s", title);
			node =removemovie(&watchListHead, searchByTitle(watchListHead, title));
			alphabeticOrder(&watchLibraryHead,node);
			break;
		case 7:
			savewatchlist(watchListHead);
			break;
		case 8:
			watchListHead = loadwatchlist();
			break;
		case 9:
			libraryMenu(&watchLibraryHead,&watchListHead);
			break;
		case 10:
			x = 0;
			break;
		}
	}
	freeList(watchLibraryHead);
	freeList(watchListHead);
}
/*allows user to enter number, and then navigate to different functions*/
void libraryMenu(movieNode **libray, movieNode **watchlist)
{
	char tmp[128],title[35],input[128];
	int intchoice = 0;
	movieNode *userNode = NULL;
	int x = 1;
	while (x == 1)
	{
		printf("1. View all movies \n");
		printf("2. Search by title \n");
		printf("3. Add a movie to watchlist \n");
		printf("4. Back to watchlist\n");

		fgets(input,128,stdin);
		sscanf(input,"%d", &intchoice);
		switch (intchoice)
		{
		case 1:
			printList(*libray);
			break;
		case 2:
			printf("Movie Title: ");
			fgets (tmp, 35, stdin);
			sscanf(tmp,"%[^\n]s", title);
			printmovieInfo(searchByTitle(*libray, title));
			break;
		case 3:
			printf("Movie Title: ");
			fgets (tmp, 35, stdin);
			sscanf(tmp,"%[^\n]s", title);
			userNode = removemovie(libray,searchByTitle(*libray, title));
			if (userNode != NULL)
			{
				AddMenu(watchlist,createmovieNode(userNode->title,userNode->genre,userNode->duration));
			}
			else
			{
				printf("Sorry, not available\n");
			}
			break;
		case 4:
			x = 0;
			return;
		}
	}
}
/*allows user to enter number, and then navigate to different functions*/
void AddMenu(movieNode **watchList, movieNode *insertNode)
{
	int intchoice = 0, pos = -1;
	char input[128];
	int x = 1;
	while (x == 1)
	{
		printf("1. Add movie to the end\n");
		printf("2. Add movie at the beginning\n");
		printf("3. Insert movie at a specific position\n");
		fgets(input,128,stdin);
		sscanf(input,"%d", &intchoice);

		switch (intchoice)
		{
		case 1:
			insertmovie(watchList, insertNode, 999999);
			return;
			break;
		case 2:
			insertmovie(watchList, insertNode, 0);
			return;
			break;
		case 3:
			while (pos < 0)
			{
				printf("position: ");
				fgets(input,128,stdin);
				sscanf(input,"%d", &pos);
				if (pos < 0)
				{
					printf("inccorect value");
					continue;
				}
				insertmovie(watchList, insertNode, pos);
				return;
			}
		}
	}
}

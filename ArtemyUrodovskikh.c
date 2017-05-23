#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DESCENDANT 1
#define SIBLING 2
#define N 256

struct node {
	int key;
	struct node *leftnode;
	struct node *rightnode;
};

int i, read, write, nodes, internal;

int insertnode (struct node *, int, int, int);
struct node initnode(int);
struct node * searchtree(struct node *, int );
int showtree(struct node *tree, int whitespace);
int classify(FILE *fp, struct node *tree);
struct node * parent(struct node *tree, int value);
int traverse(struct node *tree, int *, int *);

int main(int argc, char const *argv[])
{
	FILE *fou, *fin;

	char *buffer = (char*) malloc (sizeof(char) * N);

	if (argc == 1) {
	printf("Usage: %s options\n", argv[0]);
	printf("Optional fields:\n");
	printf("-r<file> read from file\n-w<file> write to file\n");
	}
	for (i = 1; i < argc; i++)
		if (strstr(argv[i], "-r") == argv[i]) {
			strcpy(buffer, strstr(argv[i], "-r")+2);
			if (fopen(buffer, "r"))
				fin = fopen(buffer, "r");
			else {
				printf("unable to %s for reading\n", buffer);
				continue;
			}
			read = 1;
		} else if (strstr(argv[i], "-w") == argv[i]) {
			strcpy(buffer, strstr(argv[i], "-w")+2);
			fou = fopen(buffer, "w");
			write = 1;
		}

	char answer[30];
	int value, of;
	struct node tree;


	if (read) {
		fscanf(fin, "root %d", &value);
		tree = initnode(value);
		while(fscanf(fin, "%d of parent %d", &value, &of) == 2)
			insertnode(&tree, value, DESCENDANT, of);
		fclose(fin);
	} else {
		printf("Root:\n");
		scanf("%d", &value);
		tree = initnode(value);
	}

	printf("Enter your command in the following form:\n");
	printf("number of number as sibling/descendant\n");

	char as[15];
	while (scanf("%d of %d as %s", &value, &of, &as) == 3)
		if (strcmp(as, "descendant") == 0) 
			insertnode(&tree, value, DESCENDANT, of);
		else if (strcmp(as, "sibling") == 0)
			insertnode(&tree, value, SIBLING, of);

	showtree(&tree, 0);
	traverse(&tree, &nodes, &internal);
	printf("%d nodes, %d leaf, %d internal\n", nodes, nodes-internal, internal);
	if (write) {
		fprintf(fou, "root %d\n", tree.key);
		classify(fou, &tree);
		fclose(fou);
	}

	return 0;
}

struct node initnode(int value) {
	struct node *pointer;	
	pointer = malloc (sizeof(struct node));
	(*pointer).key = value;
	(*pointer).rightnode = NULL;
	(*pointer).leftnode = NULL;
	return *pointer;
}

int insertnode(struct node *tree, int value, int as, int of) {
	struct node *ison;
	if (as == DESCENDANT)
		ison = searchtree(tree, of);
	else if (as == SIBLING) {
		ison = parent(tree, of);
	}

	if ((ison == NULL) || ((ison->leftnode != NULL) && (ison->rightnode != NULL)))
			return 1;

	if (ison->leftnode == NULL) {
			ison->leftnode = (struct node *) malloc(sizeof(struct node));
			*(ison->leftnode) = initnode(value);
	} else if (ison->rightnode == NULL) {
			ison->rightnode = (struct node *) malloc(sizeof(struct node));
			*(ison->rightnode) = initnode(value);
	}

}

struct node * searchtree(struct node *tree, int value) {
	static struct node *found;
	if (tree != NULL) {
		searchtree(tree->leftnode, value);
		if (value == tree->key){
			found = tree;
		}
		searchtree(tree->rightnode, value);
	}
	return found;
}


int showtree(struct node *tree, int whitespace) {
	for (int i = 0; i < whitespace; i++)
			printf("	");
	if (tree != NULL) {
		printf("%d\n", tree->key);
		showtree(tree->leftnode, whitespace+1);
		showtree(tree->rightnode, whitespace+1);
	} else
		printf("N\n");
}

int classify(FILE *fp, struct node *tree) {
	if (tree != NULL) {
		if (tree->leftnode != NULL)
			fprintf(fp, "%d of parent %d\n", tree->leftnode->key, tree->key);
		if (tree->rightnode != NULL)
			fprintf(fp, "%d of parent %d\n", tree->rightnode->key, tree->key);
		classify(fp, tree->leftnode);
		classify(fp, tree->rightnode);
	}
}

struct node * parent(struct node *tree, int value) {
	static struct node *found;
	if (tree != NULL) {
		if (tree->leftnode != NULL)
			if (tree->leftnode->key == value)
				found = tree;
		if (tree->rightnode != NULL) 
			if (tree->rightnode->key == value) 
				found = tree;
		parent(tree->leftnode, value);
		parent(tree->rightnode, value);
	}
	return found;
}

int traverse(struct node *tree, int *nodes, int *internal) {
	if (tree != NULL) {
		if ((tree->leftnode != NULL) || (tree->leftnode != NULL))
			(*internal)++;
		(*nodes)++;
		traverse(tree->leftnode, nodes, internal);
		traverse(tree->rightnode, nodes, internal);
	}
}

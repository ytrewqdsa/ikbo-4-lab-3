#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct Node
{
    struct Node *parent;
    struct Node *right;
    struct Node *left;
    int num;
} Node;

Node* createroot(int num);
int addnode(int num,Node* current,int target,char type,int launchflag);
void checkprint(Node *tree,int n);
void printmas(Node *tree);
void statistics(Node *tree);
int maximaldepth(Node * tree,int n);
int fisheap(Node *tree, int curdepth);
void writenode(Node *tree,FILE *f);
void loadnode(Node *tree, FILE *f);
void removetree(Node *tree);
int removeelement(Node *tree, int numtos, char typedel);

#define PARENT 1
#define SIBLING 2
#define SELF 3
#define DESCENDANTS 4
#define FAMILY 5

int leavesnum, internalnum, nodesnum, isheap, maxdepth;

int main(int argc, char *argv[])
{
    int i,numtow,numtos;
    Node *tree=0;
    FILE *f=0;
    char command[500],flag=8,*c=0;
    if (argc>1)
    {
        if ((f=fopen(argv[1],"rb"))==0)
            printf("Error opening \"%s\" file from command line! (Enter help to see command list)\n",argv[1]);
        else
        {
            fread(&numtow,sizeof(int),1,f);
            tree=createroot(numtow);
            loadnode(tree,f);
            fclose(f);
        }
    }
    else
        printf("No tree was loaded from command line arguments. (Enter help to see command list)\n");
    while (flag)
    {
        leavesnum=internalnum=nodesnum=maxdepth=0;
        statistics(tree);
        maxdepth=maximaldepth(tree,0);
        isheap=fisheap(tree,0);
        checkprint(tree,0);
        printf("Number of nodes: %d.\nNumber of internal nodes: %d.\nNumber of leaves: %d.\nMaximal depth of tree: %d.\n",nodesnum,internalnum,leavesnum,maxdepth);
        if (isheap) printf("The tree IS NOT binary heap.\n");
        else printf("The tree IS binary heap.\n");
        printf("Enter command:\n");
        scanf("%s",command);
        if (!strcmp(command,"add"))
        {
            scanf("%s",command);
            if (!strcmp(command,"root"))
            {
                if (tree==0)
                {
                    flag=10;
                    scanf("%s",command);
                    i=0;
                    while(command[i])
                    {
                        if ((!(command[i]>='0'))||(!(command[i]<='9')))
                            flag=2;
                        ++i;
                    }
                    if (flag==2)
                    {
                        printf("Not correct number!\n");
                    }
                    else if (flag==10)
                    {
                        sscanf(command,"%d",&numtow);
                        tree=createroot(numtow);
                    }
                    else printf("UNKNOWN ERROR!\n");
                    flag=8;
                }
                else
                    printf("Root already exists.\n");
            }
            else if (!strcmp(command,"node"))
            {
                flag=10;
                scanf("%s",command);
                i=0;
                while(command[i])
                {
                    if ((!(command[i]>='0'))||(!(command[i]<='9')))
                        flag=2;
                    ++i;
                }
                if (flag==2)
                {
                    printf("Not correct number!\n");
                }
                else if (flag==10)
                {
                    sscanf(command,"%d",&numtow);
                    scanf("%s",command);
                    if (!strcmp(command,"parent"))
                    {
                        scanf("%s",command);
                        if (!strcmp(command,"root"))
                        {
                            if (tree)
                            {
                                if (!tree->left)
                                {
                                    tree->left=createroot(numtow);
                                    tree->left->parent=tree;
                                }
                                else if(!tree->right)
                                {
                                    tree->right=createroot(numtow);
                                    tree->right->parent=tree;
                                }
                                else
                                    printf("%s","Root has no free descendants!\n");
                            }
                            else
                                printf("There is no root!\n");
                        }
                        else
                        {
                            flag=10;
                            i=0;
                            while(command[i])
                            {
                                if ((!(command[i]>='0'))||(!(command[i]<='9')))
                                    flag=2;
                                ++i;
                            }
                            if (flag==2)
                            {
                                printf("Not correct number!\n");
                            }
                            else if (flag==10)
                            {
                                sscanf(command,"%d",&numtos);
                                if (addnode(numtow,tree,numtos,PARENT,1));
                                else printf("No element with number %d or it already has 2 descendants!\n",numtos);
                            }
                        }
                    }
                    else if (!strcmp(command,"sibling"))
                    {
                        scanf("%s",command);
                        flag=10;
                        i=0;
                        while(command[i])
                        {
                            if ((!(command[i]>='0'))||(!(command[i]<='9')))
                                flag=2;
                            ++i;
                        }
                        if (flag==2)
                        {
                            printf("Not correct number!\n");
                        }
                        else if (flag==10)
                        {
                            sscanf(command,"%d",&numtos);
                            if (addnode(numtow,tree,numtos,SIBLING,1));
                            else printf("No element with number %d or it already has a sibling!\n",numtos);
                        }
                    }
                    else
                        printf("%s","Wrong command! Enter help to know more.\n");
                    flag=8;
                }
                else printf("UNKNOWN ERROR!\n");
                flag=8;
            }
            else
                printf("%s","Wrong command! Enter help to know more.\n");
        }
        else if (!strcmp(command,"rem"))
        {
            scanf("%s",command);
            if (!strcmp(command,"whole"))
            {
                removetree(tree);
                tree=0;
                printf("The tree was deleted.\n");
            }
            else if (!strcmp(command,"node"))
            {
                flag=10;
                scanf("%s",command);
                i=0;
                while(command[i])
                {
                    if ((!(command[i]>='0'))||(!(command[i]<='9')))
                        flag=2;
                    ++i;
                }
                if (flag==2)
                {
                    printf("Not correct number!\n");
                }
                else if (flag==10)
                {
                    sscanf(command,"%d",&numtos);
                    scanf("%s",command);
                    if (!strcmp(command,"self"))
                    {
                        if (!removeelement(tree,numtos,SELF)) printf("Element not found or has descendants.\n");
                    }
                    else if (!strcmp(command,"sibling"))
                    {
                        if (!removeelement(tree,numtos,SIBLING)) printf("Element not found, has no sibling, or sibling has descendants.\n");
                    }
                    else if (!strcmp(command,"descendants"))
                    {
                        if (!removeelement(tree,numtos,DESCENDANTS)) printf("Element not found.\n");
                    }
                    else if (!strcmp(command,"family"))
                    {
                        if (!removeelement(tree,numtos,FAMILY)) printf("Element not found.\n");
                    }
                    else
                        printf("%s","Wrong command! Enter help to know more.\n");
                }
                else
                    printf("UNKNOWN ERROR!\n");
                flag=8;
            }
            else
                printf("%s","Wrong command! Enter help to know more.\n");
        }
        else if (!strcmp(command,"save"))
        {
            fgets(command,sizeof(command),stdin);
            c=command;
            while ((*(c))!='\n') ++c;
            *c='\0';
            if ((f=fopen(command,"wb"))!=0)
            {
                writenode(tree,f);
                fclose(f);
            }
            else
                printf("Error creating a file!\n");
        }
        else if (!strcmp(command,"load"))
        {
            fgets(command,sizeof(command),stdin);
            c=command;
            while ((*(c))!='\n') ++c;
            *c='\0';
            if ((f=fopen(command,"rb"))!=0)
            {
                if (tree)
                {
                    removetree(tree);
                    tree=0;
                }
                fread(&numtow,sizeof(int),1,f);
                tree=createroot(numtow);
                loadnode(tree,f);
                fclose(f);
            }
            else
                printf("File doesn\'t exist!\n");
        }
        else if (!strcmp(command,"help"))
        {
printf("To load tree you can use a command or command line arguments.\n\
Enter:\n\
load - to load the tree\n\
   *filename*. - name of file with tree\n\
save - to save the tree\n\
   *filename*. - name of file with tree\n\
add - create\n\
   root - the root\n\
      *number*. - and fill with number\n\
   node - a new node\n\
      *number* - to fill with number\n\
         parent - as a descendant of node with number\n\
            *number*. - target number\n\
            root. - as a descendant of root\n\
         sibling - as a descendant of parent of node with number\n\
            *number*. - target number\n\
rem - delete\n\
   whole. - the whole tree\n\
   node - element of node (can\'t be used for root)\n\
      *number* - the target number\n\
         self. - the node itself (must have no descendants and not to be head)\n\
         sibling. - the sibling of this node (must have no descendants)\n\
         descendants. - delete all the descendants of the node\n\
         family. - delete all the descendants and itself\n\
Press enter to continue...\n");
        fflush(stdin);
        getchar();
        }
        else if (!strcmp(command,"exit"))
            return 0;
        else
        {
            printf("%s","Wrong command! Enter help to know more.\n");
        }
        fflush(stdin);
    }
    return 0;
}

int removeelement(Node *tree, int numtos, char typedel)
{
    static int flag;
    int tmp=0;
    if (tree)
    {
        if (typedel==SELF)
        {
            if (!flag)
            if (tree->left)
            {
                if (tree->left->num==numtos)
                {
                        if ((tree->left->left==0)&&(tree->left->right==0))
                            {
                                tmp=flag=1;
                                free(tree->left);
                                tree->left=0;
                            }
                }
            }
            if (!flag)
            if (tree->right)
            {
                if (tree->right->num==numtos)
                {
                        if ((tree->right->left==0)&&(tree->right->right==0))
                            {
                                tmp=flag=1;
                                free(tree->right);
                                tree->right=0;
                            }
                }
            }
        }
        else if (typedel==SIBLING)
        {
            if (!flag)
            if (tree->left)
            {
                if (tree->left->num==numtos)
                {
                    if (tree->right)
                        if ((tree->right->left==0)&&(tree->right->right==0))
                            {
                                tmp=flag=1;
                                free(tree->right);
                                tree->right=0;
                            }
                }
            }
            if (!flag)
            if (tree->right)
            {
                if (tree->right->num==numtos)
                {
                    if (tree->left)
                        if ((tree->left->left==0)&&(tree->left->right==0))
                            {
                                tmp=flag=1;
                                free(tree->left);
                                tree->left=0;
                            }
                }
            }
        }
        else if (typedel==DESCENDANTS)
        {
            if (!flag)
            if (tree->left)
            {
                if (tree->left->num==numtos)
                {
                    if ((tree->left->left))
                    {
                        removetree(tree->left->left);
                        tree->left->left=0;
                    }
                    if ((tree->left->right))
                    {
                        removetree(tree->left->right);
                        tree->left->right=0;
                    }
                    tmp=flag=1;
                }
            }
            if (!flag)
            if (tree->right)
            {
                if (tree->right->num==numtos)
                {
                    if ((tree->right->left))
                    {
                        removetree(tree->right->left);
                        tree->right->left=0;
                    }
                    if ((tree->right->right))
                    {
                        removetree(tree->right->right);
                        tree->right->right=0;
                    }
                    tmp=flag=1;
                }
            }
        }
        else if (typedel==FAMILY)
        {
            if (!flag)
            if (tree->left)
            {
                if (tree->left->num==numtos)
                {
                    removetree(tree->left);
                    tree->left=0;
                    tmp=flag=1;
                }
            }
            if (!flag)
            if (tree->right)
            {
                if (tree->right->num==numtos)
                {
                    removetree(tree->right);
                    tree->right=0;
                    tmp=flag=1;
                }
            }
        }
        else
            printf("Wrong 3 parameter of removeelement function!\n");
    }
    else
        return 0;
    if (!flag) tmp=removeelement(tree->left,numtos,typedel);
    if (!flag) tmp=removeelement(tree->right,numtos,typedel);
    if (!tree->parent) flag=0;
    return tmp;
}

void removetree(Node *tree)
{
    if (!tree) return;
    if (tree->left)
        removetree(tree->left);
    if (tree->right)
        removetree(tree->right);
    free(tree);
}

void loadnode(Node *tree, FILE *f)
{
    int targetn,num;
    if (!fread(&targetn,sizeof(int),1,f)) return;
    if (!fread(&num,sizeof(int),1,f)) return;
    addnode(num,tree,targetn,PARENT,1);
    loadnode(tree,f);
}

void writenode(Node *tree,FILE *f)
{
    if (tree->parent)
    {
        fwrite(&tree->parent->num,sizeof(int),1,f);
        fwrite(&tree->num,sizeof(int),1,f);
    }
    else
    {
        fwrite(&tree->num,sizeof(int),1,f);
    }
    if (tree->left) writenode(tree->left,f);
    if (tree->right) writenode(tree->right,f);
}

int fisheap(Node *tree,int curdepth)
{
    extern int maxdepth;
    static int flag,flagel;
    int tmp=0;
    ++curdepth;
    if (tree)
    {
        if ((!flag))
            if(tree->parent)
                if ((tree->parent->num)<(tree->num))
                    flag=tmp=1;
        if (!flag)
            if ((curdepth+1)<(maxdepth))
            {
                if (!((tree->left)&&(tree->right)))
                    tmp=flag=1;
            }
        if (curdepth==maxdepth)
                if (flagel)
                    tmp=flag=1;
        if (!flag)
            tmp=fisheap(tree->left,curdepth);

        if (!flag)
            tmp=fisheap(tree->right,curdepth);
    }
    else
    {
        if (!flag)
            if (curdepth==maxdepth)
                flagel=1;

    }
    if (curdepth==1) {flag=0; flagel=0;}
    return tmp;
}

int maximaldepth(Node *tree,int n)
{
    int a=0,b=0;
    if (tree)
    {
        ++n;
        a=maximaldepth(tree->left,n);
        b=maximaldepth(tree->right,n);
        if (a>n) n=a;
        if (b>n) n=b;
    }
    return n;
}

void statistics(Node *tree)
{
    extern int leavesnum, internalnum, nodesnum;
    if (tree)
    {
        ++nodesnum;
        if ((tree->right)||(tree->left))
            ++internalnum;
        else
            ++leavesnum;
        if (tree->right)
            statistics(tree->right);
        if (tree->left)
            statistics(tree->left);
    }
}

Node* createroot(int num)
{
    Node *temp;
    temp=(Node*)malloc(sizeof(Node));
    temp->left=temp->right=temp->parent=0;
    temp->num=num;
    return temp;
}

int addnode(int num,Node* current,int target,char type,int launchflag)
{
    static int flag;
    int ntmp=0;
    Node *temp;
    if (flag) return 1;
    if (!current)
    {
        printf("Node is empty!\n");
        return 0;
    }
    if (type==PARENT)
    {
        if (current->num==target)
        {
            if (current->left==0)
            {
                temp=(Node*)malloc(sizeof(Node));
                temp->left=temp->right=0;
                temp->parent=current;
                current->left=temp;
                temp->num=num;
                flag=ntmp=1;
            }
            else if (current->right==0)
            {
                temp=(Node*)malloc(sizeof(Node));
                temp->left=temp->right=0;
                temp->parent=current;
                current->right=temp;
                temp->num=num;
                flag=ntmp=1;
            }
        }

        if (!flag)
        {
            if (current->left)
            {
                if (current->right)
                {
                    ntmp=addnode(num,current->left,target,PARENT,0);
                    if (!flag) ntmp=addnode(num,current->right,target,PARENT,0);
                }
                else
                    ntmp=addnode(num,current->left,target,PARENT,0);
            }
            else
            {
                if (current->right)
                    ntmp=addnode(num,current->right,target,PARENT,0);
            }
        }
    }
    else if (type==SIBLING)
    {
        if (current->left)
        {
            if (current->right)
            {
                ntmp=addnode(num,current->left,target,SIBLING,0);
                if (!flag) ntmp=addnode(num,current->right,target,SIBLING,0);
            }
            else
            {
                if (current->left->num==target)
                {
                    temp=(Node*)malloc(sizeof(Node));
                    temp->left=temp->right=0;
                    temp->parent=current;
                    current->right=temp;
                    temp->num=num;
                    flag=ntmp=1;
                }
                else
                {
                    ntmp=addnode(num,current->left,target,SIBLING,0);
                }
            }
        }
        else if (current->right)
        {
            if (current->left)
            {
                ntmp=addnode(num,current->left,target,SIBLING,0);
                if (!flag) ntmp=addnode(num,current->right,target,SIBLING,0);
            }
            else
            {
                if (current->right->num==target)
                {
                    temp=(Node*)malloc(sizeof(Node));
                    temp->left=temp->right=0;
                    temp->parent=current;
                    current->left=temp;
                    temp->num=num;
                    flag=ntmp=1;
                }
                else
                {
                    ntmp=addnode(num,current->right,target,SIBLING,0);
                }
            }
        }
        else
            return 0;
    }
    else
    {
        printf("%s","Wrong 4 argument of addnode!!!\n");
        return 0;
    }
    if ((flag)&&(launchflag)) flag=0;
    return(ntmp);
}

void printmas(Node *tree)
{
    if (tree)
    {
        printmas(tree->left);
        printf("%d ", tree->num);
        printmas(tree->right);
    }
}

void checkprint(Node *tree,int n)
{
   int i;
   if (tree)
   {
      checkprint(tree->right, n+4);
      for (i = 0; i < n; i++)
         putchar(' ');
      printf("%d\n", tree->num);
      checkprint(tree->left, n+4);
   }
}

#include<stdio.h>
#include<stdlib.h>
typedef char chain[256];
struct Node
{

        int num;
        struct Node* next;
};


void add_nodes(struct Node **head, int num)
{
	struct Node *aux_node=(struct Node *)malloc(sizeof(struct Node));
	aux_node->num = num;
	aux_node->next=NULL;
	if(*head==NULL)
	{
		*head = aux_node;
	}
	else
	{
		struct Node *p;
		p = *head;
		while(p->next!=NULL)
		{
			p = p->next;

		}
		p->next=aux_node;
	}

}
void out_list(struct Node **head,int *value)
{
        struct Node *current; //Actual node
        for(current = *head; current!=NULL; current = current->next)
        {
                //printf("%d -->\t",current->num);
		*value = current->num;
        }
	//printf("%s","NULL");
}
void display_list(struct Node **move, int n)
{
	int node;
        for(int i=0; i<n; i++)
        {
                out_list(move++,&node);
		printf("%d--->\t",node);
                printf("\n");
        }
	printf("%s","NULL");

}
void display2(struct Node **admat, int n_reg)
{
	for(int k=0; k<n_reg; k++)
	{
        	struct Node *current;
		for(current=*admat; current!=NULL; current = current->next)
		{
			printf("%d -->\t",current->num);

		}
		admat++;
		printf("%s","NULL");
		printf("\n");
	}

}
int main()
{
	FILE *input;
	int nreg,num,i,j;
	chain filename;
	i=0;
	j=0;
	printf("FILE NAME\n");
	scanf("%s",filename);
	if((input = fopen(filename,"r"))==NULL)
	{
		printf("FAILED TO OPEN FILE\n");
	}
	fscanf(input,"%d%*[^\n]\n",&nreg);
	printf("ARRAY OF LINKED LISTS\n");
	printf("NUM OF REGIONS = %d\n",nreg);
	struct Node *elem[nreg]; //Memory allocation for the array of linked lists
	for(int i=0;i<nreg; i++)
	{
		elem[i] = NULL;
	}
	fscanf(input,"%d",&num);
	while(!feof(input))
	{
		if(num!=-1)
		{
			j=num;
			add_nodes(&elem[i],j);
			j++;
		}
		else
		{
			i++;
		}
		fscanf(input,"%d",&num);
		
	}
	fclose(input);
        //display2(elem,nreg);
	int l=0;
	do
	{
		printf("INDIDUO %d\n",l);
        	display_list(elem,nreg);
		l++;
	}while(l<9);

	return 0;
}

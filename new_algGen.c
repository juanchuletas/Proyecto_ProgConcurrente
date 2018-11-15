#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
typedef char chain[256];
//------ MODULES TO OBTAIN THE ARRAY OF LINEKD LIST---------
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
void display2(struct Node **admat,int n_reg)
{
	int i=0;
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
struct Node  **get_adymatrix()
{
	int num,i,j,k,n_reg;
	FILE *input;
	chain filename;
	k=0;
	j=0;
	printf("GRAPH FILE NAME\n");
        scanf("%s",filename);
        if((input = fopen(filename,"r"))==NULL)
        {
                printf("FAILED TO OPEN FILE\n");
        }
        fscanf(input,"%d%*[^\n]\n",&n_reg);
        struct Node *admat[n_reg]; //Memory allocation for the array of linked lists
        //struct Node *auxmat[n_reg]; //Memory allocation for the array of linked lists
        for(int i=0;i<n_reg; i++)
        {
                admat[i] = NULL;
                //auxmat[i] = NULL;
        }
        fscanf(input,"%d",&num);
        while(!feof(input))
        {
                if(num!=-1)
                {
                        j=num;
                        add_nodes(&admat[k],j);
                        j++;
                }
                else
                {
                        k++;
                }
                fscanf(input,"%d",&num);

        }
        fclose(input);

	return admat;

}

// ----- END OF ARRAY OF LINKED LIST MODULE-----------------------------------------
//
//
int **create_matrix(int n_ind,int n_reg)
{
	int *p1 = NULL;
	int **p2 = NULL;

	p1 = (int *)malloc(sizeof(int)*(n_ind*n_reg));
	if(p1!=NULL)
	{
		p2=(int **)malloc(sizeof(int *)*n_ind);
		if(p2!=NULL)
		{
			for(int i=0; i<n_ind; i++)
			{
				p2[i] = &(p1[i*n_reg]);

			

			}
		}
		else{printf("NO MEMORY\n");}
	}
	else{printf("NO MEMORY\n");}

	return p2;

}
void display_pop(int **point2, int n_ind, int n_reg)
{
        printf("YOUR MATRIX WITH %d INDIVUDUALS AND %d REGIONS TO COLOURING IS:\n",n_ind,n_reg);
        for(int i=0; i<n_ind; i++)
        {
                 printf("ind[%d] \t",i);
                for(int j=0; j<n_reg; j++)
                {
                        printf("     %d  \t",point2[i][j]);
                }
                        printf("\n");
        }
}
void generate_rand_pop(int **p2,int n_ind,int n_reg,int colour)
{
	for(int i=0; i<n_ind; i++)
	{
		for(int j=0; j<n_reg; j++)
		{
			p2[i][j] = rand()%colour;
		}
	}
	

}
int main()
{
	srand(time(NULL));
	int **pop,n_reg,n_ind,n_colour;
	FILE *input;
	chain filename;
	int k=0,j=0,num;

	// **pop refers to the population;
	// population = Num of individuals and Num of regions for colouring
	// n_ind -> stores the number of individuals
	// n_reg -> stores the number of regions to colouring
	// n_col -> Number of colours

	printf("ENTER THE POULATION SIZE\n");
	scanf("%d",&n_ind);
	printf("ENTER THE COLOUR REGIONS SIZE\n");
	scanf("%d",&n_reg);
	printf("ENTER THE No. OF COLOURS\n");
	scanf("%d",&n_colour);
	struct Node *adMat[n_reg];

	adMat = get_adymatrix();

	display2(adMat,n_reg);
	//pop = create_matrix(n_ind,n_reg); //--> allocate the memory for the population matrix
	//generate_rand_pop(pop,n_ind,n_reg,n_colour); //creates randomly the initial population
	//display_pop(pop,n_ind,n_reg); // Display matrix
	//get_fitness(pop,admat,fitness,n_ind,n_reg);

//	display_pop(pop,n_ind,n_reg,fitness); // Display matrix


}

//***************************************************
//  PROGRAM TO SOLVE THE COLOURING PROBLEM 
//  BY A GENETIC ALGORITHM APPROACH
//
//  DEVELOPERS: 
//
//  LUIS ALFREDO PÉREZ MENDOZA
//  ANGEL JESÚS AMADOR CAMPUZANO
//  JULIO GRANCIANO
//  JUAN JOSÉ GARCÍA MIRANDA
//
//  UNIVERSIDAD AUTÓNOMA METROPOLITANA
//          UNIDAD IZTAPALAPA
//
//***************************************************


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
typedef char chain[256];

// -------- MODULES FOR THE ARRAY OF LINKED LIST CREATION---------------//
//
struct Node //STRUCTS DECLARATION
{
        int num;
        struct Node* next;
};
void add_nodes(struct Node **head, int num)
{
	//THIS FUNCTION ADD NODES TO THE ARRAY OF LINKED LISTS
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
void disp_ad_matrix(struct Node **admat,int n_reg)
{
	//THIS FUNCTION DISPLAYS TO THE ARRAY OF LINKED LISTS
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
//
// -------- END OF THE MODULES FOR THE ARRAY OF LINKED LIST CREATION---------------//
//
// --------- MODULES TO CREATE A TWO DIMENSIONAL MATRIX ---------------------------//
// --------- USEFUL TO INITIALIZE THE POPULATION MATRIX----------------------------//
//
int **create_matrix(int n_ind,int n_reg)
{
	//THIS FUNCTION CREATES A TWO DIMENSIONAL MATRIX
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
	// THIS FUNCTION DISPLAYS THE POPULATION MATRIX
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
	//THIS FUNCTION CREATES THE FIRST POPULATION RANDOMLY
	for(int i=0; i<n_ind; i++)
	{
		for(int j=0; j<n_reg; j++)
		{
			p2[i][j] = rand()%colour;
		}
	}
	

}
//--------------- END OF THE MODULES TO CREATE THE POPULATION MATRIX--------------------------------//
//
// ------------ MODULES TO COMPUTE THE FITNESS -----------------------------------------------------//
// ------------- USING THE ADJACENCY MATRIX -------------------------------------------------------//
int get_sum(int **pop,struct Node **admat,int n_reg,int iter)
{
 	int count = 0;	                                                                                                                                                                                   
	for(int k=0; k<n_reg; k++)
	{
		struct Node *current; 
		for(current=*admat; current!=NULL; current = current->next)
		{
			if(pop[iter][k]==pop[iter][current->num])
			{
				count = count + 1;
			}
		}
		admat++;
	}
	return count;
}
void get_fitness(int **pop,struct Node **admat,int fitness[],int n_ind,int n_reg)
{
	//THIS MODULE COMPUTES THE FITNESS OF AN INDIVIDUAL IN THE
	//POPULATION MATRIX: RETURNS THE FITNESS ARRAY FILLED
	int i,j,count;
	int value;
	i=0; //Index for the number of individuals inside the population
	//count = 0;
	do 
	{
		value = get_sum(pop,admat,n_reg,i);
		fitness[i]=value;
		i++;
	}while(i<n_ind);
}
void get_ad_matrix(struct Node **admat,int n_reg,chain filename)
{
	//THIS MODULE GETS THE ADJACENCY MATRIX USING THE 
	// GRAPH CONECTIONS IN THE GRAPH FILE 
	FILE *input;
	int k=0,j=0,num,Nreg;
        if((input = fopen(filename,"r"))==NULL)
        {
                printf("FAILED TO OPEN FILE\n");
        }
        fscanf(input,"%d%*[^\n]\n",&Nreg);
	if(n_reg!=Nreg)
	{
		printf("WRONG FILE: NUMBRE OF REGIONS ARE DIFFERENT FROM THE GRAPH FILE\n");
	}
        for(int i=0;i<n_reg; i++)
        {
                admat[i] = NULL;
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

}
// ------------ END OF THE MODULES TO COMPUTE THE FITNESS--------------------------------------//
//
// ------- MODULE TO GET THE BEST FITNESS-------------------------------------------------------//
int get_best(int **pop,int fitness[],int mejor_ind[],int n_ind,int n_reg,int *prueba)
{
    int i,indiv,mejor;
    mejor = fitness[0];
    indiv=0;
    for(int i=1; i<(n_ind)-1; i++)
    {
        if(fitness[i]<mejor) //fitness i es el fitness del i-esimo individuo
        {
            mejor = fitness[i];
            *prueba = i;
            indiv = i;
        }
    }
    printf("fitness = %d\n",mejor);
    //printf("La solucion es:\n");
    for(int j=0; j<n_reg; j++)
    {
        mejor_ind[j] = pop[indiv][j];
	//printf("%d\n",mejor_ind[j]);
    }

    return mejor;
}
// ------- END MODULE TO GET THE BEST FITNESS----------------------------------------------------//
//
//
//
void mutation(int **aux_pop,int n_ind,int n_reg,int n_colour )
{

	int rnd_reg,rnd_color;
	for(int i=0; i<n_ind; i++)
	{
		rnd_reg = rand()%n_reg;
		rnd_color = rand()%n_colour;
		aux_pop[i][rnd_reg] = rnd_color;
		
	}


}
void cruzamiento(int **point2, int **point2_aux, int n_ind, int n_reg)
{
     int cruz_point;

     cruz_point=rand()%(n_reg-1)+1; //crea el punto de cruzamiento


      for(int i=0; i<n_ind; i++)
        {
                for(int j=0; j<cruz_point; j++)
                {
                    point2_aux[i][j]=point2[i][j];
                }

        }

       for(int i=0; i<n_ind; i=i+2) //llena el cruze en los pares
        {
                for(int j=cruz_point; j<n_reg; j++)
                {
                    point2_aux[i][j]=point2[i+1][j];

                }
        }

        for(int i=1; i<n_ind; i=i+2) //llena el cruze en los impares
        {
                for(int j=cruz_point; j<n_reg; j++)
                {
                    point2_aux[i][j]=point2[i-1][j];

                }
        }

}
void clonacion(int **pop, int **aux_pop, int n_ind, int n_reg){

    int i,j;

    for(i=0; i<n_ind; i++){
        for(j=0; j<n_reg; j++){
            aux_pop[i][j] = pop[i][j];
        }
    }

}
//
// ----------- MODULES TO COMPARE THE NEW SOLUTIONS ------------------------------------------//
//
void compara_mut(int aux_fitness[],int fitness[], int **pop, int **aux_pop,int n_ind, int n_reg)
{
   for(int i=0; i<n_ind; i++)
   {
      if(aux_fitness[i]<=fitness[i])
      {
         fitness[i] = aux_fitness[i];
         for(int j=0; j<n_reg; j++)
         {
            pop[i][j] = aux_pop[i][j];
         }
      }
   }  
}
void compara_cruz(int aux_fitness[],int fitness[], int **pop, int **aux_pop,int n_ind, int n_reg)
{
   for(int i=0; i<n_ind; i++)
   {
      if(aux_fitness[i]<fitness[i])
      {
         fitness[i] = aux_fitness[i];
         for(int j=0; j<n_reg; j++)
         {
            pop[i][j] = aux_pop[i][j];
         }
      }
   }
}
// ------ END OF THE MODULES TO COMPARE THE NEW SOLUTIONS ------------------------------------//
int main ()
{	clock_t t_in,t_fin;
	double secs;
	srand(time(NULL));
        int **pop,**aux_pop,n_reg,n_ind,n_colour,best;
        int Nreg,bestInd;
        FILE *input;
        chain filename;

        // **pop refers to the population;
        // population = Num of individuals and Num of regions for colouring
        // n_ind -> stores the number of individuals
        // n_reg -> stores the number of regions to colouring
        // n_col -> Number of colours

        do
	{
		printf("ENTER THE POULATION SIZE\n");
        	scanf("%d",&n_ind);
	
	}while(n_ind%2!=0);
        printf("ENTER THE COLOUR REGIONS SIZE\n");
        scanf("%d",&n_reg);
        printf("ENTER THE No. OF COLOURS\n");
        scanf("%d",&n_colour);
        printf("GRAPH FILE NAME\n");
        scanf("%s",filename);
	struct Node *admat[n_reg]; //A structure to allocate the array of linked lists for the adjacency matrix
	int fitness[n_ind],mejor_ind[n_reg],aux_fitness[n_ind]; // An array to store the fitness of each individual
	
	t_in = clock();
	get_ad_matrix(admat,n_reg,filename); //gets the adjacency matrix
	disp_ad_matrix(admat,n_reg);// shows the adjacency matrix
	pop = create_matrix(n_ind,n_reg); //allocate the memory for the population matrix
	aux_pop = create_matrix(n_ind,n_reg); //allocate the memory for the population matrix
	generate_rand_pop(pop,n_ind,n_reg,n_colour); //creates randomly the initial population
	//display_pop(pop,n_ind,n_reg); // Display population matrix
	get_fitness(pop,admat,fitness,n_ind,n_reg);// computes the fitness for each individual
	best = get_best(pop,fitness,mejor_ind,n_ind,n_reg,&bestInd);

	int N_gen=0;
	while(best!=0)
	{
		cruzamiento(pop,aux_pop,n_ind,n_reg);
		get_fitness(aux_pop,admat,aux_fitness,n_ind,n_reg);
		compara_cruz(aux_fitness,fitness,pop,aux_pop,n_ind,n_reg);
		best = get_best(pop,fitness,mejor_ind,n_ind,n_reg,&bestInd);
		if(best==0)
		{
			break;
		}	
		clonacion(pop,aux_pop,n_ind,n_reg);
		mutation(aux_pop,n_ind,n_reg,n_colour);
		get_fitness(aux_pop,admat,aux_fitness,n_ind,n_reg);
		compara_mut(aux_fitness,fitness,pop,aux_pop,n_ind,n_reg);
		best = get_best(pop,fitness,mejor_ind,n_ind,n_reg,&bestInd);

		N_gen++;
	

	}
	printf("SOLUCION ENCONTRADA DESPUÉS DE %d GENERACIONES Y FUE EL INDIVIDUO %d\n",N_gen,bestInd);

	/*int **sol;
	sol = create_matrix(sqrt(n_reg),sqrt(n_reg));
	int k=0;
	for(int i=0; i<sqrt(n_reg); i++)
	{
		for(int j=0; j<sqrt(n_reg);j++)
		{
			sol[i][j] = mejor_ind[k];
			k++;
			printf("%d   \t",sol[i][j]);
		}
		printf("\n");
	}while(k<n_reg);*/

	t_fin = clock();

	secs = (double)(t_fin - t_in)/CLOCKS_PER_SEC;

	printf("EL TIEMPO TOTAL DE EJECUCIÓN ES %lf segundos \n", secs);


	return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#define BEST_LOCAL 50
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


int *create_pop(int nrows, int ncols, int colores) {
    int *matrix;
    int h, i, j;

    if (( matrix = malloc(nrows*ncols*sizeof(int))) == NULL) {
        printf("Malloc error");
        exit(1);
    }

    for (h=0; h<nrows*ncols; h++) {
        matrix[h] = rand()%colores; //colocar random para numeros aleatorios
    }

    return matrix;
}

void printArray (int *row, int nElements) {
    int i;
    for (i=0; i<nElements; i++) {
        printf(" [%d] ", row[i]);
    }
    printf("\n");
}



void rellena(int **auxpop,int *vector,int n_ind,int n_reg)
{
	//THIS FUNCTION CREATES THE FIRST POPULATION RANDOMLY
	int ij=0;
	for(int i=0; i<n_ind; i++)
	{
		for(int j=0; j<n_reg; j++)
		{
			auxpop[i][j] = vector[ij];
			ij++;
		}
	}
	

}

int *crea_vector(int num_elems)
  {
  int *ap;
  ap=(int*)malloc(sizeof(int)*num_elems);
  
  if (ap==NULL)
    {
     printf("Error... Memoria insuficiente\n");
    }
  
  return ap;
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

int get_best(int **pop,int fitness[],int mejor_ind[],int n_ind,int n_reg,int *prueba)
{
    int i,indiv,mejor;
    mejor = fitness[0];
    indiv=0;
    for(int i=1; i<n_ind; i++)
    {
        if(fitness[i]<mejor) //fitness i es el fitness del i-esimo individuo
        {
            mejor = fitness[i];
            *prueba = i;
            indiv = i;
        }
    }
    //printf("fitness= %d\n",mejor);
    //printf("La solucion es:\n");
    for(int j=0; j<n_reg; j++)
    {
        mejor_ind[j] = pop[indiv][j];
	//printf("%d\n",mejor_ind[j]);
    }

    return mejor;
}

int get_worst(int **pop,int fitness[],int n_ind,int n_reg)
{
    int i,indiv,worst;
    worst = fitness[0];
    indiv=0;
    for(int i=1; i<n_ind; i++)
    {
        if(fitness[i]>worst) //fitness i es el fitness del i-esimo individuo
        {
            worst = fitness[i];
            indiv = i;
        }
    }
    return indiv;
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

void clonacion(int **pop, int **aux_pop, int n_ind, int n_reg){

    int i,j;

    for(i=0; i<n_ind; i++){
        for(j=0; j<n_reg; j++){
            aux_pop[i][j] = pop[i][j];
        }
    }

}

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
void intercambia(int pos_worst,int **pop,int mejor_ind[],int n_reg)
{
	for(int i=0; i<n_reg;i++)
	{
		pop[pos_worst][i]=mejor_ind[i];
	}

}
int best_process(int vect_recep[],int size)
{
	int local,pos;
	local = vect_recep[0];
	pos = 0;
	for(int i=1;i<size;i++)
	{ 
		if(vect_recep[i]<local)
		{
			local = vect_recep[i];
			pos = i;

		}
	}
	return pos;
}
void print_best_sol(int **sol,int mejor_ind[],int M,int N)
{
	int k=0;
	for(int i=0; i<M; i++)
	{
		for(int j=0; j<N;j++)
		{
			sol[i][j] = mejor_ind[k];
			k++;
			printf("%d   \t",sol[i][j]);
		}
		printf("\n");
	}
}

//void inter_mejor_menor(int **pop, int *best_ind, int 

int main(int argc, char **argv)
  {

  srand(time(NULL));
  int id,size,i,var,*vector,best,bestInd;
  int *mat,**aux_pop,**pop;
  int n_reg,n_ind,n_colors;
  int M=30,N=30;
  int pos_worst;
  //chain filename;
 char filename[]="graf30x30.txt";
  
  n_reg=M*N;
  n_ind=120;
  n_colors=2;

   
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  MPI_Status Reporte;
  
  var=(n_reg*(n_ind/size));
  
  vector=crea_vector(var);
  pop=create_matrix(n_ind/size,n_reg);
  aux_pop=create_matrix(n_ind/size,n_reg);
  int pos;
  int flag=0;
  struct Node *admat[n_reg]; //A structure to allocate the array of linked lists for the adjacency matrix
  int fitness[n_ind/size],mejor_ind[n_reg],aux_fitness[n_ind/size];
  int vect_recep[size]; // An array to store the fitness of each individual
  get_ad_matrix(admat,n_reg,filename);
  //disp_ad_matrix(admat,n_reg);
 
  if (id==0)
  { 
       mat=create_pop(n_ind,n_reg,n_colors);
       //printf("Matriz Inicio:\n");
       //printArray(mat, n_ind*n_reg);
  }
  MPI_Scatter(mat,var,MPI_INT,vector,var,MPI_INT,0,MPI_COMM_WORLD);
  rellena(pop,vector,n_ind/size,n_reg);
  get_fitness(pop,admat,fitness,n_ind/size,n_reg);
  best=get_best(pop,fitness,mejor_ind,n_ind/size,n_reg,&bestInd);
  pos_worst = get_worst(pop,fitness,n_ind/size,n_reg);
  MPI_Gather(&best,1,MPI_INT,vect_recep,1,MPI_INT,0,MPI_COMM_WORLD);
  if(id==0)
  {
	  pos = best_process(vect_recep,size);
     	  printf("MEJOR FITNESS = %d\n",vect_recep[pos]);
	  if(vect_recep[pos]==0)
	  {
		  flag=1;
	  }
  }
  MPI_Bcast(&flag,1,MPI_INT,0,MPI_COMM_WORLD);
  while(flag==0)
	{
		MPI_Bcast(&pos,1,MPI_INT,0,MPI_COMM_WORLD);
  		MPI_Bcast(mejor_ind,n_reg,MPI_INT,pos,MPI_COMM_WORLD);
 		 if(id!=pos)
  		{
			intercambia(pos_worst,pop,mejor_ind,n_reg);
  		}
  		int N_gen;
		cruzamiento(pop,aux_pop,n_ind/size,n_reg);
		get_fitness(aux_pop,admat,aux_fitness,n_ind/size,n_reg);
		compara_cruz(aux_fitness,fitness,pop,aux_pop,n_ind/size,n_reg);
		best = get_best(pop,fitness,mejor_ind,n_ind/size,n_reg,&bestInd);
  		pos_worst = get_worst(pop,fitness,n_ind/size,n_reg);
  		MPI_Gather(&best,1,MPI_INT,vect_recep,1,MPI_INT,0,MPI_COMM_WORLD);
  		if(id==0)
  		{
			pos = best_process(vect_recep,size);
     			printf("MEJOR FITNESS = %d\n",vect_recep[pos]);
	  		if(vect_recep[pos]==0)
	  		{
				flag=1;
	  		}
  		}
  		MPI_Bcast(&flag,1,MPI_INT,0,MPI_COMM_WORLD);
  		MPI_Bcast(&pos,1,MPI_INT,0,MPI_COMM_WORLD);
  		MPI_Bcast(mejor_ind,n_reg,MPI_INT,pos,MPI_COMM_WORLD);
  		if(id!=pos)
  		{
			  intercambia(pos_worst,pop,mejor_ind,n_reg);
  		}
		clonacion(pop,aux_pop,n_ind/size,n_reg);
		mutation(aux_pop,n_ind/size,n_reg,n_colors);
		get_fitness(aux_pop,admat,aux_fitness,n_ind/size,n_reg);
		compara_mut(aux_fitness,fitness,pop,aux_pop,n_ind/size,n_reg);
		best = get_best(pop,fitness,mejor_ind,n_ind/size,n_reg,&bestInd);
  		pos_worst = get_worst(pop,fitness,n_ind/size,n_reg);
  		MPI_Gather(&best,1,MPI_INT,vect_recep,1,MPI_INT,0,MPI_COMM_WORLD);
  		if(id==0)
  		{
			pos = best_process(vect_recep,size);
     			printf("MEJOR FITNESS = %d\n",vect_recep[pos]);
	  		if(vect_recep[pos]==0)
	  		{
		      		flag=1;
	  		}
  		}
  		MPI_Bcast(&flag,1,MPI_INT,0,MPI_COMM_WORLD);
  		MPI_Bcast(&pos,1,MPI_INT,0,MPI_COMM_WORLD);
  		MPI_Bcast(mejor_ind,n_reg,MPI_INT,pos,MPI_COMM_WORLD);
  		if(id!=pos)
  		{
			  intercambia(pos_worst,pop,mejor_ind,n_reg);
  		}

		N_gen++;
	

	}
  if(id==0)
  {
	int **sol;
  	sol=create_matrix(M,N);

	print_best_sol(sol,mejor_ind,M,N);
  }
  MPI_Finalize();

  return 0;
}

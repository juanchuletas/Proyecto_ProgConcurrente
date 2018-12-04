//Codigo realizado por Luis Alfredo Pérez Mendoza
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>


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

int main(int argc, char **argv)
  {

  srand(time(NULL));
  int **dato,id,size,i,var;
  int **pop;
  int n_reg,n_ind;

  n_reg=4;
  n_ind=8;

  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  var=(n_reg*(n_ind/size));
  
  dato=create_matrix((n_ind/size),n_reg);
 
  if (id==0)
    { 
       pop=create_matrix(n_ind,n_reg);
       generate_rand_pop(pop,n_ind,n_reg,5);
       display_pop(pop,n_ind,n_reg);
    }


  MPI_Scatter(pop,var,MPI_INT,dato,var,MPI_INT,0,MPI_COMM_WORLD);
  display_pop(dato,n_ind/size,n_reg);

  MPI_Finalize();

  return 0;
  }

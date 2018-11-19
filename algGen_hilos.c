#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<pthread.h>

#define N_IND 4
#define N_REG 4
#define N_THREADS 4
#define COLOURS 2
int pop[N_IND][N_REG];
int aux_pop[N_IND][N_REG];
int fitness[N_IND];
pthread_mutex_t c1;


typedef char chain[256];
struct Node
{

        int num;
        struct Node* next;
};

struct Node *admat[N_REG]; //GLOBAL ACCESS TO THE ADJACENCY MATRIX

void add_nodes(int num,int k)
{
        struct Node *aux_node=(struct Node *)malloc(sizeof(struct Node));
        aux_node->num = num;
        aux_node->next=NULL;
        if(admat[k]==NULL)
        {
                admat[k] = aux_node;
        }
        else
        {
                struct Node *p;
                p = admat[k];
                while(p->next!=NULL)
                {
                        p = p->next;

                }
                p->next=aux_node;
        }

}
void display()
{
        for(int k=0; k<N_REG; k++)
        {
                struct Node *current;
                for(current=admat[k]; current!=NULL; current = current->next)
                {
                        printf("%d -->\t",current->num);

                }
                admat[k]++;
                printf("%s","NULL");
                printf("\n");
        }

}
void get_ad_matrix(chain filename)
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
        if(Nreg!=N_REG)
        {       
                printf("WRONG FILE: NUMBER OF REGIONS ARE DIFFERENT FROM THE GRAPH FILE\n");
                printf("COLOURING REGIONS INTO THE CODE: %d\n",N_REG);
		exit(1);
        }
        for(int i=0;i<Nreg; i++)
        {       
                admat[i] = NULL;
        }
        fscanf(input,"%d",&num);
        while(!feof(input))
        {       
                if(num!=-1)
                {       
                        j=num;
                        add_nodes(j,k);
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
void *generate_rand_pop(void *id)
{
	int init,fin;
	long thread_id;
	thread_id = (long)id;
	init = (N_IND/N_THREADS)*thread_id;
	fin = (N_IND/N_THREADS)*(thread_id + 1);
	
        //THIS FUNCTION CREATES THE FIRST POPULATION RANDOMLY
        for(int i=init; i<fin; i++)
        {
                for(int j=0; j<N_REG; j++)
                {
                        //pthread_mutex_lock(&m1);
			pop[i][j] = rand()%COLOURS;
                        //pthread_mutex_unlock(&m1);
                }
        }
	//printf("HOLA SOY EL HILO %ld\n",thread_id);
	pthread_exit(NULL);


}
/*int get_sum(int iter)
{
        int count = 0;
        for(int k=0; k<N_REG; k++)
        {
                struct Node *current;
                for(current=admat[k]; current!=NULL; current = current->next)
                {
                        if(pop[iter][k]==pop[iter][current->num])
                        {
                                count = count + 1;
                        }
                }
                admat[k]++;
        }
        return count;
}
void *get_fitness(void *id)
{
        //THIS MODULE COMPUTES THE FITNESS OF AN INDIVIDUAL IN THE
        //POPULATION MATRIX: RETURNS THE FITNESS ARRAY FILLED
	int thread_id, init,fin,value,count;
	thread_id = (intptr_t)id;
	init = (N_IND/N_THREADS)*thread_id;
	fin = (N_IND/N_THREADS)*(thread_id + 1);
        for(int i=init; i<fin; i++)
        {
                pthread_mutex_lock(&c1);
		value  = get_sum(i);
                //fitness[i]=value;
                pthread_mutex_unlock(&c1);
	
        }
	printf("Soy Hilo %d y mi fitness es %d\n",thread_id,value);
	pthread_exit(NULL);
}*/
void display_pop()
{
        // THIS FUNCTION DISPLAYS THE POPULATION MATRIX
        printf("YOUR MATRIX WITH %d INDIVUDUALS AND %d REGIONS TO COLOURING IS:\n",N_IND,N_REG);
        for(int i=0; i<N_IND; i++)
        {
                 printf("ind[%d] \t",i);
                for(int j=0; j<N_REG; j++)
                {
                        printf("     %d  \t",pop[i][j]);
                }
                        printf("\n");
        }
}
int main ()
{
	chain filename;
	pthread_t threads[N_THREADS];
	int rc;
	long ids[N_THREADS];
	printf("GRAPH FILE NAME\n");
        scanf("%s",filename);

	get_ad_matrix(filename);

	display();

	for(int i=0;i<N_THREADS;i++)
	{
  		ids[i] = i;
  		pthread_create(&threads[i], NULL,generate_rand_pop, (void *) ids[i]);
  		//pthread_create(&threads[i], NULL,get_fitness, (void *) ids[i]);
	}
        for(int i=0;i<N_THREADS;i++) 
	{
		pthread_join(threads[i],NULL);
	}
	display_pop();
	/*for(int i=0; i<N_IND; i++)
        {
                printf("ind[%d]  = %d\n",i,fitness[i]);
        }*/

	return 0;


}

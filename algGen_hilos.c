#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<semaphore.h>
#include<pthread.h>

#define N_IND 8
#define N_REG 4
#define N_THREADS 2
#define COLOURS 2
int best =-1; //variable global compartida
struct Node
{

        int num;
        struct Node* next;
}*admat[N_REG]; //GLOBAL ACCESS TO THE ADJACENCY MATRIX

int pop[N_IND][N_REG];
int aux_pop[N_IND][N_REG];
int fitness[N_IND];
int contador=0;
int flag=1;

pthread_mutex_t c1;
//pthread_mutex_t mutex;
sem_t S;  //declaracion del semaforo S
sem_t C;  //declaracion del semaforo C
typedef char chain[256];

void add_nodes(struct Node **temp,int num)
{
        struct Node *aux_node=(struct Node *)malloc(sizeof(struct Node));
        aux_node->num = num;
        aux_node->next=NULL;
        if(*temp==NULL)
        {
 *temp = aux_node;
        }
        else
        {
                struct Node *p;
                p = *temp;
                while(p->next!=NULL)
                {
                        p = p->next;

                }
                p->next=aux_node;
        }

}
void get_ad_matrix(chain filename)
{
        //THIS MODULE GETS THE ADJACENCY MATRIX USING THE
        // GRAPH CONECTIONS IN THE GRAPH FILE
        FILE *input;
        int k=0,j=0,num,Nreg,i;
        if((input = fopen(filename,"r"))==NULL)
        {
                printf("FAILED TO OPEN FILE\n");
 exit(1);
        }
        fscanf(input,"%d%*[^\n]\n",&Nreg);
        if(Nreg!=N_REG)
        {
                printf("WRONG FILE: NUMBER OF REGIONS ARE DIFFERENT FROM THE GRAPH FILE\n");
                printf("COLOURING REGIONS INTO THE CODE: %d\n",N_REG);
 exit(1);
        }
for( i=0; i<N_REG; i++)
{
 admat[i]=NULL;
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
void display()
{
struct Node *tmp;
int i;
for(i=0; i<N_REG; i++)
{
 tmp=admat[i];
 while(tmp!=NULL)
 {
  printf("%d-->\t",tmp->num);
  tmp = tmp -> next;
 }
 printf("%s","NULL");
 printf("\n");
}

}
void barrera(int id)
{

 // int valsem;
  //int valsem2;
  int band=0,i;
  //int thread_id = (int )id;
//band=0;
   sem_wait(&C);
   contador++;
   printf("%d - %d\n",id,contador);

if(contador == N_THREADS)
 {
    contador =0;

    for(i=0; i < N_THREADS-1; i++)
    {
      sem_post(&S);
    }
 }
        else{
  band = 1;
 }
sem_post(&C);

if(band==1)
 sem_wait(&S);

}

void generate_rand_pop(void *id)
{
int init,fin,i,j;
//printf("MODULO POBLACION ALEATORIA\n");
int *thread_id = (int *)id;
init = (N_IND/N_THREADS)*(*thread_id);
fin = (N_IND/N_THREADS)*(*thread_id + 1);

        //THIS FUNCTION CREATES THE FIRST POPULATION RANDOMLY
        for(i=init; i<fin; i++)
        {
                for( j=0; j<N_REG; j++)
                {
  pop[i][j] = rand()%COLOURS;
                }
}
//barrera(thread_id);
//pthread_exit(NULL);
}
int get_sum(int iter)
{
struct Node *current;
        int count = 0,k;
        for(k=0; k<N_REG; k++)
        {
 current = admat[k];
 while(current!=NULL)
 {
  if(pop[iter][k]==pop[iter][current->num])
  {
   count = count + 1;
  }
  current = current->next;
 }

        }
        return count;
}
void get_fitness(void *id)
{
        //THIS MODULE COMPUTES THE FITNESS OF AN INDIVIDUAL IN THE
        //POPULATION MATRIX: RETURNS THE FITNESS ARRAY FILLED
int init,fin,value,count,valsem,valsem2,band=0;
int *thread_id = (int *)id,k,i;
struct Node *current;
init = (N_IND/N_THREADS)*(*thread_id);
fin = (N_IND/N_THREADS)*(*thread_id + 1);
        for(i=init; i<fin; i++)
        {
 //printf("Soy hilo %ld y ejecuto inicio %d hasta fin %d\n",thread_id,init,fin);
 count = 0;
 for(k=0; k<N_REG; k++)
 {
  current=admat[k];
  while(current!=NULL)
  {
   if(pop[i][k]==pop[i][current->num])
   {
    count = count + 1;
   }
   current = current -> next;
  }
 }
 fitness[i] = count;
        }
}

void get_mejor(void *id)
{  int i,mejor,indiv;
    int j,inicio,fin;
    int mloc[N_THREADS];

   int *thread_id = (int *)id;
   inicio = (N_IND/N_THREADS)*(*thread_id);
   fin = (N_IND/N_THREADS)*(*thread_id+1);
   mejor = fitness[inicio];

   for(i=inicio; i<fin; i++)
    {
        if(fitness[i]<=mejor) //fitness i es el fitness del i-esimo individuo
        {
            mejor = fitness[i];
            indiv = i;
        }
    }
    mloc[*thread_id] = mejor;

    pthread_mutex_lock(&c1 );
    if(best<0)
	  best=mejor;
    else
	   if(mejor < best)
           	best = mejor;
    pthread_mutex_unlock(&c1 );

   //printf("Soy hilo %d y mi mejor fitness es: %d \n", *thread_id,mejor);
}

void mutacion(void *id )
{

int init,fin,i;

int *thread_id = (int *)id;
init = (N_IND/N_THREADS)*(*thread_id);
fin = (N_IND/N_THREADS)*(*thread_id + 1);

int rnd_reg,rnd_color;
for(i = init; i < fin; i++)
{
 rnd_reg = rand()%N_REG;
 rnd_color = (rand()%COLOURS);
 pop[i][rnd_reg] = rnd_color;

}

//barrera(id);
}

void cruzamiento(void *id)
{
     int cruz_point,inicio,fin,j,i;
     int *thread_id = (int *)id;
     cruz_point=rand()%(N_REG-1)+1; //crea el punto de cruzamiento
     inicio = (N_IND/N_THREADS)*(*thread_id);
     fin = (N_IND/N_THREADS)*(*thread_id+1);

     for(i=inicio; i<fin; i++)
     {
     	for(j=0; j<N_REG; j++)
    	{
           aux_pop[i][j] = pop[i][j];
        }

     }
     
     for( i=inicio; i<fin; i=i+2)
     {
     	for(j=0; j<cruz_point; j++)
    	{
           aux_pop[i][j] = pop[i+1][j];
	   aux_pop[i+1][j] = pop[i][j];
        }

     }

     for(i=inicio; i<fin; i++)
     {
     	for(j=0; j<N_REG; j++)
    	{
           pop[i][j] = aux_pop[i][j];
        }

     }

   // barrera(id);

}

void display_pop()
{ int i,j;
        // THIS FUNCTION DISPLAYS THE POPULATION MATRIX
        printf("YOUR MATRIX WITH %d INDIVUDUALS AND %d REGIONS TO COLOURING IS:\n",N_IND,N_REG);
        for(i=0; i<N_IND; i++)
        {
                 printf("ind[%d] \t",i);
                for(j=0; j<N_REG; j++)
                {
                        printf("     %d  \t",pop[i][j]);
                }
                        printf("\n");
        }
}
void *genetic_pool(void *id)
{
	int  *thread_id = (int *)id;
	flag=1;
	//printf("WELCOME THE GENETIC POOL I'M THREAD %ld\n",thread_id);
	generate_rand_pop(id);
        get_fitness(id);
        get_mejor(id);
        barrera(*thread_id);
         if(*thread_id==0 && best==0)
             flag=0;
         barrera(*thread_id);
         while(flag!=0)
         {
         
	      	cruzamiento(id);
            	mutacion(id);
            	get_fitness(id);
            	get_mejor(id);
                barrera(*thread_id);
        	if(*thread_id==0 && best==0)
        	    flag=0;
        	if(*thread_id== 0)
        	    printf("FITNESS = %d \n",best);
                barrera(*thread_id);
    }

}


int main ()
{
srand(time(NULL));
chain filename;
pthread_t threads[N_THREADS];
int rc,i;

pthread_mutex_init(&c1,NULL);
//pthread_mutex_init(&mutex,NULL);
sem_init(&S,0,0); //inicializacion del semaforo S = 0
sem_init(&C,0,1); //inicializacion del semaforo S = 1
int ids[N_THREADS];
printf("GRAPH FILE NAME\n");
        scanf("%s",filename);

get_ad_matrix(filename);
display();
contador = 0;
for(i=0;i<N_THREADS;i++)
{
    ids[i] = i;
    pthread_create(&threads[i], NULL,genetic_pool, (void *) &ids[i]);
}
for(i=0;i<N_THREADS;i++)
{
 pthread_join(threads[i],NULL);
}

display_pop();
for(i=0; i<N_IND; i++)
{
 printf("IND[%d] fitness = %d\n",i,fitness[i]);
}

return 0;


}


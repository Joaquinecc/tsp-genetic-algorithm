#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "data29.h"


// Number of cities 
// Initial population size for the algorithm
#define POP_SIZE 100
#define MAX_LOOP 10000
#define TRUE 1
#define FALSE 0




struct individual {
	char gnome[V+1] ;
	int fitness;
};

void print_map(){
	printf("\n");

	for(int i=0;i<V;i++)
		printf("%c\t", CITIES[i]);
	printf("\n");

	for(int i=0;i<V;i++){
		printf("%c\t", CITIES[i]);
		for(int j=0;j<V;j++){
			printf("%d\t", map[i][j]);
		}
			printf("\n");
		}

}
void copy_struct_individual(struct individual a[POP_SIZE],struct individual b[POP_SIZE]){
	for(int i=0;i<POP_SIZE;i++)
		a[i]=b[i];
}
int repeat(char s[V+1], char ch)
{
	for (int i = 0; i < V; i++) {
		if (s[i] == ch)
			return TRUE;
	}
	return FALSE;
}
void create_gnome(char gnome[V+1]){
	int i=0;
	char x;
	//fill array
	for(int j = 0; j < V ; gnome[j++] ='0');
	while (i<V) {
		x=CITIES[rand()%V];
		if (!repeat(gnome,x))
			gnome[i++] = x;	
	}
	gnome[i]=0;
}
int cal_fitness(char gnome[V+1]){
	int map_copy[V][V];
	memcpy(map_copy,map, V*V*sizeof(int));

	//Cal first distance of the las city to the first city
	int return_distance=map_copy[gnome[V-1] - 65][gnome[0] - 65];
	if(return_distance == INFINITE)
		return INFINITE;
	int f = 0;
	for (int i = 0; i < V - 1; i++) {
		//Misma ciudad
		if(gnome[i + 1] - 65 == gnome[i ] - 65 )
			return INFINITE;

		if (map_copy[gnome[i] - 65][gnome[i + 1] - 65] == INFINITE)
			return INFINITE;

		f += map_copy[gnome[i] - 65][gnome[i + 1] - 65];

		//Set to infinite all road to current city
		for(int j=0;j<V;j++)
			map_copy[j][gnome[i] - 65]=INFINITE;
		
	}
	return f+return_distance;
}

int find_min(struct individual population[POP_SIZE]){
	int min=population[0].fitness;
	for(int i=1;i<POP_SIZE;i++)
		if(min>population[i].fitness)
			min=population[i].fitness;

	return min;
}
void select_individual(struct individual current_population[POP_SIZE],struct individual selected[POP_SIZE]){
	int bottom=find_min(current_population);
	float temp;
	int j=0;
	while(j < POP_SIZE){

		for(int i=0;i<POP_SIZE;i++){
			float ratio = (float)bottom/current_population[i].fitness;
			temp = rand() % 100 +1;
			if(ratio<=temp){
				selected[j]=current_population[i];
				j++;
				}
		}
	}
}
void interchange_chars(char gnome[V],int index1,int index2){
	char temp=gnome[index1];
	gnome[index1]=gnome[index2];
	gnome[index2]=temp;
}

void cross_over(struct individual population[POP_SIZE],struct individual result[POP_SIZE]){
	int pair,index1,index2;
	char temp;
	copy_struct_individual(result,population);
	for(int i=0;i<POP_SIZE;i+=2){
		pair= rand() % V;
		for(int j=0;j<pair;j++){

			temp=CITIES[rand() % V];
			index1 = (int)(strchr(population[i].gnome,temp)-population[i].gnome);//[pointer of the character temp] - pointer = index of temp in the string
			index2 = (int)(strchr(population[i+1].gnome,temp)-population[i+1].gnome);
			interchange_chars(result[i].gnome,index1,index2);


			temp=CITIES[rand() % V];
			index1 = (int)(strchr(population[i+1].gnome,temp)-population[i+1].gnome);
			index2 = (int)(strchr(population[i].gnome,temp)-population[i].gnome);
			interchange_chars(result[i+1].gnome,index1,index2);
		}		

		result[i].fitness = cal_fitness(result[i].gnome);
		result[i+1].fitness = cal_fitness(result[i+1].gnome);
	}
}
void mutate(struct individual population[POP_SIZE],struct individual result[POP_SIZE]){
	int mutate;
	// memcpy(result,population, POP_SIZE*sizeof(struct individual));
	copy_struct_individual(result,population);
	int temp1,temp2;
	char temp3;
	for(int i=0;i<POP_SIZE;i++){
		mutate=rand()%2;
		if(mutate){
			//Exchange cities
			temp1=rand() % V;
			temp2=rand() % V;
			temp3=result[i].gnome[temp1];
			result[i].gnome[temp1]=result[i].gnome[temp2];
			result[i].gnome[temp2]=temp3;
			result[i].fitness=cal_fitness(result[i].gnome);
			}
			
	}
}
int compare( const void* p1, const void* p2){
     struct individual a = * ( (struct individual*) p1 );
     struct individual b = * ( (struct individual*) p2 );

     if ( a.fitness == b.fitness ) return 0;
     else if ( a.fitness < b.fitness) return -1;
     else return 1;
}
void choose_best_ones(struct individual population[POP_SIZE*2], struct individual result[POP_SIZE]){	
	qsort( population, POP_SIZE*2, sizeof(struct individual), compare );
	// memcpy(result,population, POP_SIZE*sizeof(struct individual));
	copy_struct_individual(result,population);

}
void print_population(struct individual population[POP_SIZE]){
	for (int i = 0; i < POP_SIZE; i++)
		printf("%d Individual : %s with fittness : %d\n",i+1,population[i].gnome,population[i].fitness);
	printf("\n");
}

int main(int argc, char **argv)	 {

/* Intializes random number generator */
	// srand( time( NULL ) );

	printf("Map\n");
	print_map();

 
	struct individual population[POP_SIZE];

	// Populating the GNOME pool.
	for (int i = 0; i < POP_SIZE; i++) {
		create_gnome(population[i].gnome);
		population[i].fitness = cal_fitness(population[i].gnome);
	}

	//Print population
	printf("Individual Population\n");
	print_population(population);

	for(int iter=0;iter<MAX_LOOP;iter++){
		printf("Iteration %d\n",iter);
		//Select pair to cross
		struct individual selected[POP_SIZE];
		select_individual(population,selected);
		printf("Individual Selected\n");
		print_population(selected);	

		// Cross-over
		struct individual cross_population[POP_SIZE];
		cross_over(selected,cross_population);
		printf("Individual cross over\n");
		print_population(cross_population);		

		//Mutate
		struct individual mutate_population[POP_SIZE];
		mutate(cross_population,mutate_population);
		printf("Individual Mutation\n");
		print_population(mutate_population);	

		
		//Choose best ones
		// struct individual full_population[POP_SIZE*2];
		// memcpy(full_population,population, POP_SIZE*sizeof(struct individual));
		// memcpy(&full_population[POP_SIZE],mutate_population, POP_SIZE*sizeof(struct individual));
		// choose_best_ones(full_population,population);
		// printf("Individual Best ones\n");
		// print_population(population);

		struct individual full_population[POP_SIZE*2];
		copy_struct_individual(full_population,population);
		copy_struct_individual(&full_population[POP_SIZE],mutate_population);
		choose_best_ones(full_population,population);
		printf("Individual Best ones\n");
		print_population(population);
	
	}


	return 0;
}




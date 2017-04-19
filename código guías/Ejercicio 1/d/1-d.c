#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int crear(int N, float P, int a[]);       // Crea la red con 0 y 1 con probabilidad P. 
int clases(int N, int a[]);		          // Coloca los labels a los diferentes clusters de la red.
int Ns(int N, int a[], int tam[]); 		  // Calcula el valor de Ns.

int main(){

	/* Defino los parametros */
	srand(time(NULL));
	int N, i, j, iteraciones = 27000;
	float P;
	N = 128;
	P = 0.597123; // Pc del punto a.
	int a[N*N];
	int tam[N*N+1];
	float ns[N*N+1];

	for (i=0; i<(N*N+1); i++){ // Lleno el vector ns.
		ns[i]=0;
	}

	FILE* fichero;
    fichero = fopen("1-d-N128.txt", "wt");


	/* Calculos */
	for (j=0; j<iteraciones; j++){

		// system("cls"); // Win.
		// system("clear"); // Linux.
		printf("cargando - %i porciento\n",100*j/iteraciones);

		*a = crear(N,P,a);
		*a = clases(N,a);
		*tam = Ns(N,a,tam);

		for (i=0; i<(N*N+1); i++){
			if (tam[i]!=0){
				ns[i] = ns[i] + tam[i];
			}
		}

	}

	for (i=0; i<(N*N+1); i++){ // Lleno el vector ns.
		if (ns[i]!=0){
			ns[i]=ns[i]/(iteraciones*N*N);
			fprintf(fichero,"%3i    %f\n",i, ns[i]);
		}
	}

	fclose(fichero);

	return 0;

}


/*** Funciones auxiliares ***/


int crear(int N, float P, int b[]){ // Crea la red con 0 y 1 con probabilidad P. 

	float R;
	int i;

	for (i=0; i<N*N; i++){
		R = rand()%100 + 1;
		R = R/100;
		if (R<=P){
			b[i] = 1;
		}else{
			b[i] = 0;
		}
	}

	return *b;

}


int clases(int N, int a[]){ // Coloca los labels a los diferentes clusters de la red.

	int clase[N*N/2];
	int ult_clase = 2;
	int i, j, c, s1, s2;

	for (i=0; i<N*N/2; i++){ // Lleno el vector clase.
		clase[i]=0;
	}

	/* Defino las etiquetas */
	for (i=0; i<N; i++){

		for (j=0; j<N; j++){
			if (a[i*N+j]==1){ // Si la posicion en la que estoy parado esta llena.

				if (i==0 && j==0){ // Si estoy en la esquina superior izquierda (no tiene vecinos).
					a[i*N+j] = ult_clase;
					clase[ult_clase] = ult_clase;
					ult_clase=ult_clase+1;
				}

				else if (i==0){ // Si estoy en la fila superior (no tiene vecinos superiores).
					if (a[i*N+j-1]==0){
						a[i*N+j] = ult_clase;
						clase[ult_clase] = ult_clase;
						ult_clase=ult_clase+1;
					}
					else if (a[i*N+j-1]!=1){ 
						s1 = a[i*N+j-1];
						while(clase[s1]<0){
							s1 = -clase[s1];
						}
						a[i*N+j] = s1;
					}
				}

				else if (j==0){	// Si estoy en la columna izquierda (no tiene vecinos izquierdos).			
					if (a[i*N+j-N]==0){
						a[i*N+j] = ult_clase;
						clase[ult_clase] = ult_clase;
						ult_clase=ult_clase+1;
					}
					else if (a[i*N+j-N]!=1){ 
						s1 = a[i*N+j-N];
						while(clase[s1]<0){
							s1 = -clase[s1];
						}
						a[i*N+j] = s1; 
					}
				}

				else {
					if (a[i*N+j-1]==0 && a[i*N+j-N]==0){ // Los dos vecinos son cero.
						a[i*N+j] = ult_clase;
						clase[ult_clase] = ult_clase;
						ult_clase=ult_clase+1;
					}
					else if (a[i*N+j-1]==0 && a[i*N+j-N]!=1){ // Solo uno de sus vecinos es cero.
						s1 = a[i*N+j-N];
						while(clase[s1]<0){
							s1 = -clase[s1];
						}
						a[i*N+j] = s1; 
					}
					else if (a[i*N+j-1]!=1 && a[i*N+j-N]==0){ // Solo uno de sus vecinos es cero.
						s1 = a[i*N+j-1];
						while(clase[s1]<0){
							s1 = -clase[s1];
						}
						a[i*N+j] = s1; 
					}
					else if (a[i*N+j-1]!=1 && a[i*N+j-N]!=1 && a[i*N+j-1]!=a[i*N+j-N]){ // Ambos vecinos son diferentes de cero.
						s1 = a[i*N+j-1];
						s2 = a[i*N+j-N];
						while(clase[s1]<0){
							s1 = -clase[s1];
						}
						while(clase[s2]<0){
							s2 = -clase[s2];
						}
						if (s1<s2){
							clase[s2] = -s1;
							a[i*N+j] = s1;
						}
						else if (s2<s1){
							clase[s1] = -s2;
							a[i*N+j] = s2;
						}
						else{	// Si ambos vecinos son iguales.
							a[i*N+j] = s1;
						}
					}
					else {
						s1 = a[i*N+j-1];
						while(clase[s1]<0){
							s1 = -clase[s1];
						}
						a[i*N+j] = s1; 
					}
				}
				
			}
		}
	}

	/* Retiquetado */
	for (i=0; i<N*N; i++){
		s1 = a[i];
		while(clase[s1]<0){
			s1 = -clase[s1];
		}
		a[i] = s1;
	}

	return *a;

}


int Ns(int N, int a[], int tam[]){ // Calcula el valor de ns.

	int clases[N*N/2];
	int i;

	for (i=0; i<N*N/2; i++){ // Lleno el vector clases.
		clases[i]=0;
	}

	for (i=0; i<N*N; i++){
		if (a[i]!=0){
			clases[a[i]] = clases[a[i]] + 1; // Es un vector que en cada posicion me dice cuantos nodos hay de esa clase.
		}
	}

	for (i=0; i<(N*N+1); i++){ // Lleno el vector tamaños.
		tam[i]=0;
	}

	for (i=0; i<N*N/2; i++){
		if (clases[i]!=0){
			tam[clases[i]] = tam[clases[i]] + 1;
		}
	}

	return *tam;

}
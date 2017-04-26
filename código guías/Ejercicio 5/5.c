#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int crear(int N, float P, int a[]);  // Crea la red con 0 y 1 con probabilidad P. 
int clases(int N, int a[]);		     // Coloca los labels a los diferentes clusters de la red.
int Ns(int N, int a[], int tam[]);   // Calcula el valor de Ns.

int main(){

	srand(time(NULL));
	int N, i, j, c;
	N = 128;
	int iteraciones = 27000;
	int puntos = 1000;
	float P; 
	float P_vec[puntos], S_vec[100*puntos];
	int tam[N*N+1];
	float ns[N*N+1];
	int a[N*N];


	FILE* fichero;
    fichero = fopen("5-N128.txt", "wt");

	for (i=0; i<puntos; i++){ // Genero el vector de probabilidades.
		P_vec[i] = i+1;
	}
	for (i=0; i<puntos; i++){
		P_vec[i] = P_vec[i]/puntos;
	}


	for (j=0; j<puntos; j++){
		P = P_vec[j];
		printf("cargando - %i porciento\n",100*j/puntos);
		for (i=0; i<(N*N+1); i++){ // Lleno el vector ns.
			ns[i]=0;
		}
		for (i=0; i<iteraciones; i++){			
			*a = crear(N,P,a);
			*a = clases(N,a);
			*tam = Ns(N,a,tam);
			for (c=0; c<(N*N+1); c++){
				ns[c] = ns[c] + tam[c];
			}
		}
		for (i=0; i<(N*N+1); i++){ //Normalizo ns.
			ns[i]=ns[i]/(iteraciones*N*N);
		}
		for (i=1; i<100; i++){ 
			S_vec[j*100+i] = ns[i];			
		}
	}


	for (j=0; j<puntos; j++){
		fprintf(fichero,"%f    ", P_vec[j]);
		for (i=1; i<100; i++){
			fprintf(fichero,"%f    ", S_vec[j*100+i]);
		}
		fprintf(fichero,"\n");	
	}
	fprintf(fichero,"\n\n\n\n");
	float max, p;
	for (i=1; i<100; i++){
		max = 0;
		p = 0;
		for (j=0; j<puntos; j++){
			if(S_vec[j*100+i]>max){
				max = S_vec[j*100+i];
				p = P_vec[j];	
			}
		}
		fprintf(fichero,"%2i    %f\n", i, p);
	}


	fclose(fichero);

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


int Ns(int N, int a[], int tam[]){ // Calcula el valor de Ns.

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
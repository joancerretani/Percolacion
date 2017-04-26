#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int crear(int N, float P, int a[]);  // Crea la red con 0 y 1 con probabilidad P. 
int clases(int N, int a[]);		     // Coloca los labels a los diferentes clusters de la red.
int percolo2(int N, int a[]);	     // Me pone con que label el sistema percolo.
int Ns(int N, int a[], int tam[]);   // Calcula el valor de Ns sin tener en cuenta el del cluster infinito.
float valor_medio(float c[], int b); // calcula el valor medio del vector c de tamaño b.

int main(){

	/* Defino los parametros */
	srand(time(NULL));
	int i, j, c;
	int N = 16;
	int puntos = 100;
	int iteraciones = 27000;
	float P_vec[puntos], m2_vec[puntos], P;
	float medias[iteraciones], cuenta;
	int a[N*N];
	int tam[N*N+1];

	FILE* fichero;
    fichero = fopen("6-N16.txt", "wt");

	for (i=0; i<puntos; i++){ // Genero el vector de probabilidades.
		P_vec[i] = i+1;
	}
	for (i=0; i<puntos; i++){
		P_vec[i] = P_vec[i]/puntos;
	}

	// Pongo a P entre 0.5 y 0.7.
	/*for (i=0; i<puntos; i++){
		P_vec[i] = P_vec[i]/5;   // P va de 0 a 0.2
	}
	for (i=0; i<puntos; i++){
		P_vec[i] = P_vec[i]+0.5; // P va de 0.5 a 0.7
	}*/


	for (j=0; j<puntos; j++){
		printf("cargando - %i porciento\n",100*j/puntos);
		P = P_vec[j];
		for (i=0; i<iteraciones; i++){
			*a = crear(N,P,a);	
			*a = clases(N,a);
			*tam = Ns(N,a,tam);
			cuenta = 0;
			for (c=0;c<N*N+1;c++){ //calculo m2
				cuenta = cuenta + c*c*tam[c]/(N*N);
			}
			medias[i] = cuenta;
		}
		m2_vec[j] = valor_medio(medias, iteraciones);
	}


	for (j=0; j<puntos; j++){
		fprintf(fichero,"%f    %f \n", P_vec[j], m2_vec[j]);
	}


	fclose(fichero);
	return 0;

}


/*** Funciones auxiliares ***/

float valor_medio(float c[], int b){ // calcula el valor medio del vector c de tamaño b.
	float suma=0, media;
	int i;
	for (i=0; i<b; i++){
		suma = suma + c[i];
	}
	media = suma/b;

	return media;

}


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
	int i, perc;
	perc = percolo2(N,a);

	for (i=0; i<N*N/2; i++){ // Lleno el vector clases.
		clases[i]=0;
	}

	for (i=0; i<N*N; i++){
		if (a[i]!=0 && a[i]!=perc){
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


int percolo2(int N, int a[]){ // Me pone si el sitema percoló.
	int i, perc=0;
	int sup[N*N/2];
	int inf[N*N/2];
	for (i=0; i<N*N/2; i++){
		sup[i] = 0;
		inf[i] = 0;
	}
	for (i=0; i<N; i++){
		if (a[i]!=0){
			sup[a[i]] = 1; 
		}
	}
	for (i=0; i<N; i++){
		if (a[(N-1)*N+i]!=0){
			inf[a[(N-1)*N+i]] = 1; 
		}
	}
	for (i=0; i<N*N/2; i++){
		if (sup[i]==1 && inf[i]==1){
			perc = i;
			break;
		}
	}

	return perc;

}
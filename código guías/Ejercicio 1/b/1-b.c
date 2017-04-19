#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int crear(int N, float P, int a[]);  // Crea la red con 0 y 1 con probabilidad P. 
int clases(int N, int a[]);		     // Coloca los labels a los diferentes clusters de la red.
int percolo(int N, int a[]);	     // Me pone si el sitema percoló.
float valor_medio(float c[], int b); // calcula el valor medio del vector c de tamaño b.

int main(){

	srand(time(NULL));
	int N, perc, i, j;
	int iteraciones = 27000;
	int puntos = 100;
	float P; 
	float medias[iteraciones];
	float P_vec[puntos], F_P[puntos];
	N=128;
	int a[N*N];


	FILE* fichero;
    fichero = fopen("1-b_N128.txt", "wt");

	for (i=0; i<puntos; i++){ // Genero el vector de probabilidades.
		P_vec[i] = i+1;
	}
	for (i=0; i<=puntos; i++){
		P_vec[i] = P_vec[i]/puntos;
	}


	for (j=0; j<puntos; j++){
		P = P_vec[j];

		// system("cls"); // Win.
		// system("clear"); // Linux.
		printf("cargando - %i porciento\n",100*j/puntos);

		for (i=0; i<iteraciones; i++){
			*a = crear(N,P,a);
			*a = clases(N, a);
			perc = percolo(N,a);
			medias[i] = perc;
		}

		F_P[j] = valor_medio(medias, iteraciones);

	}

	for (j=0; j<puntos; j++){
		printf("%f    %f \n", P_vec[j], F_P[j]);
		fprintf(fichero,"%f    %f \n", P_vec[j], F_P[j]);

	}


	fclose(fichero);

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


int percolo(int N, int a[]){ // Me pone si el sitema percoló.
	int i, perc=0;
	int sup[N*N/2];
	int inf[N*N/2];
	int clase;
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
			perc = 1;
			clase = i;
			break;
		}
	}

	return perc;

}
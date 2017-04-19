#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void imprimir(int *red, int n);
void llenar(int *red,int n,float prob);
void corregir_etiqueta(int *red,int *clase,int n);
void etiqueta_falsa(int *red,int *clase,int s1,int s2);
int actualizar(int *red,int *clase,int s,int frag);
int hoshen(int *red,int n);
int percola(int *red,int n);
int Ns(int *red, int n);

int main(){

	srand(time(NULL));
	int n = 20;
	int *red;
	red=(int *)malloc(n*n*sizeof(int));
	float prob = 0.6;
	llenar(red,n,prob);	
	imprimir(red,n);
	hoshen(red,n);
	imprimir(red,n);
	percola(red,n);
	Ns(red,n);

}


// Funciones auxiliares //

void llenar(int *red,int n,float prob){

	float R;
	int i;	

	for (i=0; i<n*n; i++){
		R = rand()%100 + 1;
		R = R/100;
		if (R<=prob){
			*(red+i) = 1;
		}else{
			*(red+i) = 0;
		}
	}

}


void imprimir(int *red, int n){ // Imprime la red en pantalla.

	int i;	

	printf("\n");
	for (i=0; i<n*n; i++){
 		if ((i+1)%n == 0){
 			printf("%3i\n", *(red+i));
 		}
 		else{
 			printf("%3i", *(red+i));
 		}
	}
	printf("\n");

}


int hoshen(int *red,int n){ // Coloca los labels a los diferentes clusters de la red.

	int *clase;
	clase=(int *)malloc((n*n)/2*sizeof(int));
	int frag = 2;
	int i, j, c, s1, s2;

	for (i=0; i<n*n/2; i++){ // Lleno el vector clase.
		*(clase+i)=0;
	}

	/* Defino las etiquetas */
	for (i=0; i<n; i++){

		for (j=0; j<n; j++){
			if (*(red+i*n+j)==1){ // Si la posicion en la que estoy parado esta llena.

				if (i==0 && j==0){ // Si estoy en la esquina superior izquierda (no tiene vecinos).
					*(red+i*n+j) = frag;
					*(clase+frag) = frag;
					frag=frag+1;
				}

				else if (i==0){ // Si estoy en la fila superior (no tiene vecinos superiores).
					if (*(red+i*n+j-1)==0){
						*(red+i*n+j) = frag;
						*(clase+frag) = frag;
						frag=frag+1;
					}
					else if (*(red+i*n+j-1)!=1){ 
						s1 = *(red+i*n+j-1);
						while(*(clase+s1)<0){
							s1 = -*(clase+s1);
						}
						*(red+i*n+j) = s1;
					}
				}

				else if (j==0){	// Si estoy en la columna izquierda (no tiene vecinos izquierdos).			
					if (*(red+i*n+j-n)==0){
						*(red+i*n+j) = frag;
						*(clase+frag) = frag;
						frag=frag+1;
					}
					else if (*(red+i*n+j-n)!=1){ 
						s1 = *(red+i*n+j-n);
						while(*(clase+s1)<0){
							s1 = -*(clase+s1);
						}
						*(red+i*n+j) = s1; 
					}
				}

				else {
					if (*(red+i*n+j-1)==0 && *(red+i*n+j-n)==0){ // Los dos vecinos son cero.
						*(red+i*n+j) = frag;
						*(clase+frag) = frag;
						frag=frag+1;
					}
					else if (*(red+i*n+j-1)==0 && *(red+i*n+j-n)!=1){ // Solo uno de sus vecinos es cero.
						s1 = *(red+i*n+j-n);
						while(*(clase+s1)<0){
							s1 = -*(clase+s1);
						}
						*(red+i*n+j) = s1; 
					}
					else if (*(red+i*n+j-1)!=1 && *(red+i*n+j-n)==0){ // Solo uno de sus vecinos es cero.
						s1 = *(red+i*n+j-1);
						while(*(clase+s1)<0){
							s1 = -*(clase+s1);
						}
						*(red+i*n+j) = s1; 
					}
					else if (*(red+i*n+j-1)!=1 && *(red+i*n+j-n)!=1 && *(red+i*n+j-1)!=*(red+i*n+j-n)){ // Ambos vecinos son diferentes de cero.
						s1 = *(red+i*n+j-1);
						s2 = *(red+i*n+j-n);
						etiqueta_falsa(red+i*n+j,clase,s1,s2);
					}
					else {
						s1 = *(red+i*n+j-1);
						while(*(clase+s1)<0){
							s1 = -*(clase+s1);
						}
						*(red+i*n+j) = s1; 
					}
				}
				
			}
		}
	}


	corregir_etiqueta(red,clase,n);
	free(clase);
	return 0;

}


void corregir_etiqueta(int *red,int *clase,int n){

	int i,s;

	for (i=0; i<n*n; i++){
		s = *(red+i);
		while(*(clase+s)<0){
			s = -*(clase+s);
		}
		*(red+i) = s;
	}

}


void etiqueta_falsa(int *red,int *clase,int s1,int s2){

	while(*(clase+s1)<0){
		s1 = -*(clase+s1);
	}
	while(*(clase+s2)<0){
		s2 = -*(clase+s2);
	}
	if (s1<s2){
		*(clase+s2) = -s1;
		*red = s1;
	}
	else if (s2<s1){
		*(clase+s1) = -s2;
		*red = s2;
	}
	else{	// Si ambos vecinos son iguales.
		*red = s1;
	}	

}


int percola(int *red,int n){

	int i, perc=0;
	int sup[n*n/2];
	int inf[n*n/2];
	int clase;

	for (i=0; i<n*n/2; i++){
		sup[i] = 0;
		inf[i] = 0;
	}
	for (i=0; i<n; i++){
		if (*(red+i)!=0){
			sup[*(red+i)] = 1; 
		}
	}
	for (i=0; i<n; i++){
		if (*(red+(n-1)*n+i)!=0){
			inf[*(red+(n-1)*n+i)] = 1; 
		}
	}
	for (i=0; i<n*n/2; i++){
		if (sup[i]==1 && inf[i]==1){
			perc = 1;
			clase = i;
			break;
		}
	}

	if (perc==1){
		printf("el sistema SI percolo con el cluster %i.\n",clase);
	}else{
		printf("el sistema NO percolo.\n");
	}

	return perc;

}


int Ns(int *red, int n){ // Calcula el valor de Ns.

	int clases[n*n/2];
	int i;
	int tam[n*n+1];


	for (i=0; i<n*n/2; i++){ // Lleno el vector clases.
		clases[i]=0;
	}

	for (i=0; i<n*n; i++){
		if (*(red+i)!=0){
			clases[*(red+i)] = clases[*(red+i)] + 1; // Es un vector qu en cada posicion me dice cuantos nodos hay de esa clase.
		}
	}

	for (i=0; i<(n*n+1); i++){ // Lleno el vector tamaños.
		tam[i]=0;
	}

	for (i=0; i<n*n/2; i++){
		if (clases[i]!=0){
			tam[clases[i]] = tam[clases[i]] + 1;
		}
	}

	printf("\n");
	printf("   s -  Ns \n");
	for (i=0; i<(n*n+1); i++){
		if (tam[i]!=0){
			printf(" %3i - %2i\n",i, tam[i]);
		}
	}
	printf("\n");

	return *tam;

}


int actualizar(int *red,int *clase,int s,int frag){



}
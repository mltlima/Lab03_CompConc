/**
 * Miguel Lima Tavares
 * dre: 119161571
 * Lab 03
 * somo concorrente de uma serie que se aproxime do 
 * valor de pi
 */

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<math.h>

long long int N; //dimensao do vetor de entrada
int nthreads; //numero de threads


//fluxo das threads
void * tarefa(void * arg) {
   long int id = (long int) arg; //identificador da thread
   double *somaLocal = 0; //variavel local da soma de elementos
   somaLocal = (double*) malloc(sizeof(double));
   if(somaLocal==NULL) {exit(1);}
   long int tamBloco = N/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = N;
   else fim = ini + tamBloco; //trata o resto se houver

   //soma os elementos do bloco da thread
   for(long int i=ini; i<fim; i++){
      if (i%2 == 0){
         *somaLocal -= (1.0/(2 * i + 1));
      } else{
         *somaLocal += (1.0/(2 * i + 1));
      }  
   }

   //retorna o resultado da soma local 
   return (void *) somaLocal;
}

//fluxo principal
int main(void) {

   double piSequ= 1.0; //soma sequencial de pi
   double piConc= 0; //soma concorrente de pi
   pthread_t *tid; //identificadores das threads no sistema
   void  *retorno; //valor de retorno das threads
   char input[100]; //string para receber o long long int

   //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
   printf("Digite o numero de elementos da serie\n");
   fgets (input, 100, stdin);
   N = atoll(input); // conversao para long long int
   
   printf("Digite o numero de threads\n");
   scanf("%d",&nthreads);
   
   //soma sequencial dos elementos de pi
   int temp = 1;
   clock_t tempo1 = clock();
   for(long int i=3; i<N*2; i+=2){
      if (temp%2 == 0){
         piSequ += 1.0/i;
         temp++;
      } else{
         piSequ -= 1.0/i;
         temp ++;   
      }
   }
   tempo1 = clock() - tempo1;
   double tempoExecucao1 = (double)(tempo1) / CLOCKS_PER_SEC;
   printf("\nTempo sequencial: %f\n", tempoExecucao1);

   //soma concorrente dos elementos de pi
   clock_t tempo2 = clock(); // inicio contagem do tempo de execucao concorrente
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //criar as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   //aguardar o termino das threads
   for(long int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), &retorno);
      double num = *(double *)retorno;
      //soma global
      piConc += num;
   }
   tempo2 = clock() - tempo2;
   double tempoExecucao2 = (double)(tempo2) / CLOCKS_PER_SEC;
   printf("\nTempo concorrente: %f\n\n", tempoExecucao2);

   //exibir os resultados
   printf("Soma seq:  %.15lf\n", 4 * piSequ);
   printf("Pi real:   %.15lf\n",M_PI);
   printf("Soma conc: %.15lf\n", 4 * -1 * piConc);

   //libera as areas de memoria alocadas
   //free(vetor);
   free(tid);

   return 0;
}

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

unsigned int my_rand_r(unsigned int *seedp) {
    // Parâmetros clássicos de um Gerador Linear Congruencial (LCG)
    *seedp = *seedp * 1103515245 + 12345;
    return (unsigned int)(*seedp / 65536) % (RAND_MAX + 1);
}

// Definição global do número de passos para consistência
const long NUM_PASSOS = 100000000;
long pontos_no_circulo = 0;//variavel conpartilhada pelas thread

//versão Paralela
void pi_paralel_for() {
        #pragma omp parallel for
        for (long i = 0; i < NUM_PASSOS; i++){ 
            unsigned int seed = omp_get_thread_num(); //semente unica por thread 

            double x = (double)my_rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double y = (double)my_rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            if (x * x + y * y < 1.0) {
                pontos_no_circulo++;//aqui esta a condição de corrida
            }
        }
}

int main() {
    double start_time, end_time;

    printf("Iniciando analise de desempenho para %ld passos.\n", NUM_PASSOS);
    start_time = omp_get_wtime();
    pi_paralel_for();
    end_time = omp_get_wtime();
    double tempo_paralelo = end_time - start_time;
    double pi_estimado = 4.0 * pontos_no_circulo / NUM_PASSOS;
    
    printf("Estimativa paralela de pi = %f\n", pi_estimado);
    printf("Tempo Paralelo: %f segundos\n", tempo_paralelo);

    return 0;
}
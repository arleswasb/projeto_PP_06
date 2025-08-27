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

// Versão 1: Sequencial
void pi_sequencial() {
    long pontos_no_circulo = 0;
    unsigned int seed = 12345; // Semente fixa para repetibilidade

    for (long i = 0; i < NUM_PASSOS; i++) {
        double x = (double)my_rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)my_rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y < 1.0) {
            pontos_no_circulo++;
        }
    }
    double pi = 4.0 * pontos_no_circulo / NUM_PASSOS;
    // printf("Sequencial: pi = %f\n", pi);
}

// Versão 2: Paralelo com 'critical'
void pi_paralelo_critical() {
    long pontos_no_circulo = 0;
    
    #pragma omp parallel
    {
        // Cada thread precisa de sua própria semente
        unsigned int seed = omp_get_thread_num();
        
        #pragma omp for
        for (long i = 0; i < NUM_PASSOS; i++) {
            double x = (double)my_rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            double y = (double)my_rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
            if (x * x + y * y < 1.0) {
                #pragma omp critical
                pontos_no_circulo++;
            }
        }
    }
    double pi = 4.0 * pontos_no_circulo / NUM_PASSOS;
    // printf("Critical:   pi = %f\n", pi);
}

// Versão 3: Paralelo com 'reduction'
void pi_paralelo_reduction() {
    long pontos_no_circulo = 0;
    
    #pragma omp parallel for reduction(+:pontos_no_circulo)
    for (long i = 0; i < NUM_PASSOS; i++) {
        unsigned int seed = i; // Semente pode ser simples aqui
        double x = (double)my_rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)my_rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y < 1.0) {
            pontos_no_circulo++;
        }
    }
    double pi = 4.0 * pontos_no_circulo / NUM_PASSOS;
    // printf("Reduction:  pi = %f\n", pi);
}


int main() {
    double start_time, end_time;

    printf("Iniciando análise de desempenho para %ld passos.\n", NUM_PASSOS);

    // Teste Sequencial
    start_time = omp_get_wtime();
    pi_sequencial();
    end_time = omp_get_wtime();
    double tempo_sequencial = end_time - start_time;
    printf("Tempo Sequencial: %f segundos\n", tempo_sequencial);

    // Teste com Critical
    start_time = omp_get_wtime();
    pi_paralelo_critical();
    end_time = omp_get_wtime();
    double tempo_critical = end_time - start_time;
    printf("Tempo Paralelo (Critical): %f segundos\n", tempo_critical);
    printf("  -> Speedup vs Sequencial: %.2fx\n", tempo_sequencial / tempo_critical);

    // Teste com Reduction
    start_time = omp_get_wtime();
    pi_paralelo_reduction();
    end_time = omp_get_wtime();
    double tempo_reduction = end_time - start_time;
    printf("Tempo Paralelo (Reduction): %f segundos\n", tempo_reduction);
    printf("  -> Speedup vs Sequencial: %.2fx\n", tempo_sequencial / tempo_reduction);

    return 0;
}
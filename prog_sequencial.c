#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h> // Cabeçalho necessário para omp_get_wtime()

// Definição global do número de passos para consistência
const long NUM_PASSOS = 100000000;


void pi_sequencial() {
    long pontos_no_circulo = 0;
    unsigned int seed = 12345; // Semente fixa para repetibilidade

    for (long i = 0; i < NUM_PASSOS; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y < 1.0) {
            pontos_no_circulo++;
        }
    }
    double pi = 4.0 * pontos_no_circulo / NUM_PASSOS;
    printf("Sequencial: pi = %f\n", pi);
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

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h> // Incluir a biblioteca OpenMP
#include <time.h>

int main() {
    long num_passos = 100000000;
    long pontos_no_circulo = 0;
    double x, y;

    srand(time(NULL));

    // Diretiva para paralelizar o loop
    #pragma omp parallel for
    for (long i = 0; i < num_passos; i++) {
        // Gera coordenadas x, y entre -1.0 e 1.0
        x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y < 1.0) {
            pontos_no_circulo++; // <<< PONTO CRÍTICO DO PROBLEMA!
        }
    }

    double pi_estimado = 4.0 * pontos_no_circulo / num_passos;
    printf("Estimativa (incorreta) de pi = %f\n", pi_estimado);

    return 0;
}
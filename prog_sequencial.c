#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    long num_passos = 100000000; // Número total de pontos (iterações)
    long pontos_no_circulo = 0;
    double x, y;

    // Semente para geração de números aleatórios
    srand(time(NULL)); 

    for (long i = 0; i < num_passos; i++) {
        // Gera coordenadas x, y entre -1.0 e 1.0
        x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y < 1.0) {
            pontos_no_circulo++;
        }
    }

    double pi_estimado = 4.0 * pontos_no_circulo / num_passos;
    printf("Estimativa sequencial de pi = %f\n", pi_estimado);

    return 0;
}
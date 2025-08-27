#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int main() {
    long num_passos = 100000000;
    long pontos_no_circulo = 0;
    int i;

    // A cláusula reduction faz tudo para nós!
    #pragma omp parallel for private(i) reduction(+:pontos_no_circulo)
    for (i = 0; i < num_passos; i++) {
        unsigned int seed = time(NULL) ^ omp_get_thread_num() ^ i; // Semente ainda mais variada
        double x = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        
        if (x * x + y * y < 1.0) {
            pontos_no_circulo++;
        }
    }

    double pi_estimado = 4.0 * pontos_no_circulo / num_passos;
    printf("Estimativa (com reduction) de pi = %f\n", pi_estimado);

    return 0;
}
Com base na apresentação e nos códigos do **Projeto 06 (Estimativa de π)**, aqui está um arquivo `README.md` completo e formatado para o seu repositório Git.

-----

# Estimativa de π com OpenMP

> [cite\_start]Um Estudo sobre Sincronização e Gerenciamento de Escopo de Variáveis [cite: 416]

[cite\_start]**Autor:** Werbert Arles de Souza Barradas [cite: 417]  
[cite\_start]**Disciplina:** Programação Paralela - DCA3703 [cite: 418]  
[cite\_start]**Instituição:** Universidade Federal do Rio Grande do Norte (UFRN) [cite: 418]  
[cite\_start]**Data:** 31 de agosto de 2025 [cite: 419]

-----

## 📝 Sobre o Projeto

[cite\_start]Este projeto explora a paralelização da estimativa do valor de $\\pi$ utilizando o método de Monte Carlo, uma tarefa computacionalmente intensiva e ideal para testar os limites do paralelismo[cite: 424]. [cite\_start]O objetivo é analisar o ganho de desempenho (speedup), demonstrar experimentalmente o problema da **condição de corrida** e explorar diferentes soluções de sincronização e gerenciamento de escopo de variáveis com a API OpenMP[cite: 426].

## ⚙️ Versões Implementadas

[cite\_start]O código evolui através de quatro abordagens distintas para ilustrar os desafios e as soluções na programação paralela[cite: 427]:

1.  [cite\_start]**Versão Sequencial:** Uma implementação de thread única que serve como *baseline* para validar a correção dos resultados e medir o desempenho base[cite: 428, 429].
2.  [cite\_start]**Versão Paralela Ingênua:** Uma primeira tentativa de paralelização com `#pragma omp parallel for` que intencionalmente expõe o problema da condição de corrida[cite: 430], resultando em valores incorretos para $\\pi$.
3.  [cite\_start]**Versão Paralela com `critical`:** Uma correção que resolve a condição de corrida utilizando uma seção crítica[cite: 431]. [cite\_start]Esta versão produz o resultado correto, mas demonstra o alto custo de desempenho da sincronização excessiva[cite: 431].
4.  [cite\_start]**Versão Paralela Otimizada:** A solução final e eficiente, que utiliza variáveis privadas para contagem local (padrão de redução manual) para alcançar tanto a correção do resultado quanto um ganho de desempenho significativo[cite: 432].

## 🔬 Conceitos Chave Abordados

### Condição de Corrida: Por que a Versão Ingênua Falha?

[cite\_start]A instrução `pontos_no_circulo++` não é uma operação atômica[cite: 546]. [cite\_start]Ela consiste em três passos: ler, incrementar e escrever o valor de volta na memória[cite: 548, 549, 550]. [cite\_start]Quando múltiplas threads executam esses passos simultaneamente, uma pode sobrescrever o trabalho da outra, levando à perda de incrementos e a um resultado final incorreto[cite: 557].

#### Cenário de Conflito:

  * [cite\_start]Thread A lê `total` (ex: 100)[cite: 553].
  * [cite\_start]Thread B lê `total` (ainda 100)[cite: 554].
  * [cite\_start]Thread A escreve `101`[cite: 555].
  * [cite\_start]Thread B também escreve `101`[cite: 556].

[cite\_start]**Resultado:** Dois pontos foram encontrados, mas o contador foi incrementado apenas uma vez[cite: 557].

### Estratégias de Sincronização e Escopo

  * [cite\_start]**`#pragma omp critical`**: Garante a correção ao permitir que apenas uma thread por vez modifique a variável compartilhada, mas cria um gargalo de performance[cite: 431].
  * [cite\_start]**Redução Manual**: A versão otimizada utiliza uma variável privada (`pontos_no_circulo_local`) para que cada thread conte seus pontos de forma independente[cite: 432]. A soma final é feita em uma seção `critical` executada apenas uma vez por thread, minimizando o custo da sincronização.
  * **Gerenciamento de Escopo**: O projeto também explora o uso de cláusulas como `shared`, `private` e `default(none)` para gerenciar explicitamente como as variáveis são tratadas pelas threads.

## 📊 Resultados

A tabela abaixo resume os resultados obtidos ao executar o programa para estimar $\\pi$ com $10^8$ iterações.

| Versão | Tempo de Execução (s) |
| :--- | :--- |
| **Sequencial** | [cite\_start]1.069 [cite: 568] |
| **Paralela (Ingênua)** | [cite\_start]1.05 [cite: 569] |
| **Paralela (Crítica)** | [cite\_start]5.176 [cite: 573] |
| **Paralela (Otimizada)**| [cite\_start]**0.39** [cite: 574] |

### Análise de Desempenho (Speedup)

Comparando a versão sequencial com a otimizada:
[cite\_start]$S = \\frac{T\_{sequencial}}{T\_{paralelo}} = \\frac{1.069s}{0.390s} \\approx 2.74$ [cite: 583]

[cite\_start]A paralelização otimizada resultou em um programa **2.74 vezes mais rápido** que a versão sequencial[cite: 584].

## 🚀 Como Compilar e Executar

#### Pré-requisitos

  - Um compilador C com suporte a OpenMP (por exemplo, **GCC**).

#### Compilação

Abra um terminal na pasta do projeto e use os seguintes comandos para compilar cada uma das versões:

```bash
# Versão Sequencial
gcc -o prog_sequencial prog_sequencial.c -fopenmp -lm

# Versão Paralela Ingênua
gcc -o prog_paralel_for prog_paralel_for.c -fopenmp -lm

# Versão com Critical (Lenta)
gcc -o prog_paralel_for_critical prog_paralel_for_critical.c -fopenmp -lm

# Versão Otimizada com variável private
gcc -o prog_paralel_for_critical_private prog_paralel_for_critical_private.c -fopenmp -lm
```

  * `-fopenmp`: **Flag essencial** que habilita a compilação com as bibliotecas OpenMP.
  * `-lm`: Link para a biblioteca matemática.

#### Execução

Após a compilação, execute qualquer uma das versões com:

```bash
./<nome_do_executavel>
# Exemplo:
./prog_paralel_for_critical_private
```

## 📚 Conclusão

[cite\_start]O experimento demonstrou o potencial de ganho de desempenho do OpenMP, com um speedup de 2.74x na versão otimizada, e os riscos inerentes à programação concorrente[cite: 593]. [cite\_start]A escolha da estratégia de sincronização mostrou-se crucial, pois o uso inadequado (`critical` dentro do laço) pode degradar a performance, enquanto a abordagem correta (redução manual) a otimiza[cite: 595]. [cite\_start]Compreender e aplicar mecanismos de sincronização e gerenciamento de escopo é fundamental para desenvolver software paralelo que seja não apenas rápido, mas também correto[cite: 597].
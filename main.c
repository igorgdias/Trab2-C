#include <stdio.h>

typedef struct Data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct CoronaVirus {
    Data dataInfeccao;
    int grauDeContagio;
    int grauDeLetalidade;
} CoronaVirus;

typedef struct Pessoa {
    char nome[100];
    int idade;
    char sexo;
    CoronaVirus coronaVirus;
} Pessoa;

typedef struct Cidade {
    char nome[100];
    Pessoa pessoas[10000];
    int populacao;
    Pessoa infectados[10000];
    int numInfectados;
    float infeccaoProporcional;
    float letalidadeProporcional;
    char arquivoCidade[100];
} Cidade;

typedef struct Estado {
    char nome[100];
    Cidade cidades[1000];
    int numCidades;
    int populacao;
    float infeccaoProporcional;
    float letalidadeProporcional;
} Estado;

typedef struct Pais {
    Estado estados[100];
    int numEstados;
} Pais;

char *lerLinha() {}

Pais MontaEstados() {}

Pais MontaCidades() {}

int intervaloData() {}

void printPessoa(Pessoa pessoa) {
    char grau[4][5] = {"Zero", "Baixo", "Medio", "Alto"};
    CoronaVirus corona = pessoa.coronaVirus;
    Data data = corona.dataInfeccao;
    printf("%s %d %c", pessoa.nome, pessoa.idade, pessoa.sexo);
    printf(" %d/%d/%d", data.dia, data.mes, data.ano);
    printf(" %s %s\n", grau[corona.grauDeContagio], grau[corona.grauDeLetalidade]);
}

void estatisticas(Pais pais) {
    Estado estado;
    Cidade cidade;
    Cidade cidadeMaiorInfeccao[2];
    Cidade cidadeMaiorLetalidade[2];
    Estado estadoMaiorInfeccao[2];
    Estado estadoMaiorLetalidade[2];
    estadoMaiorInfeccao[0] = pais.estados[0];
    estadoMaiorLetalidade[0] = pais.estados[0];
    for (int i = 0; i < pais.numEstados; ++i) {
        estado = pais.estados[i];

        if (estadoMaiorInfeccao[0].infeccaoProporcional < estado.infeccaoProporcional) {
            estadoMaiorInfeccao[0] = estado;
            estadoMaiorInfeccao[1] = estado;
        } else if (estadoMaiorInfeccao[0].infeccaoProporcional == estado.infeccaoProporcional) {
            estadoMaiorInfeccao[1] = estado;
        }

        if (estadoMaiorLetalidade[0].letalidadeProporcional < estado.letalidadeProporcional) {
            estadoMaiorLetalidade[0] = estado;
            estadoMaiorLetalidade[1] = estado;
        } else if (estadoMaiorLetalidade[0].letalidadeProporcional == estado.letalidadeProporcional) {
            estadoMaiorLetalidade[1] = estado;
        }

        printf("%s:\n", estado.nome);

        cidadeMaiorInfeccao[0] = estado.cidades[0];
        cidadeMaiorLetalidade[0] = estado.cidades[0];

        for (int j = 0; j < estado.numCidades; ++j) {
            cidade = estado.cidades[j];

            if (cidadeMaiorInfeccao[0].infeccaoProporcional < cidade.infeccaoProporcional) {
                cidadeMaiorInfeccao[0] = cidade;
                cidadeMaiorInfeccao[1] = cidade;
            } else if (cidadeMaiorInfeccao[0].infeccaoProporcional == cidade.infeccaoProporcional) {
                cidadeMaiorInfeccao[1] = cidade;
            }

            if (cidadeMaiorLetalidade[0].letalidadeProporcional < cidade.letalidadeProporcional) {
                cidadeMaiorLetalidade[0] = cidade;
                cidadeMaiorLetalidade[1] = cidade;
            } else if (cidadeMaiorLetalidade[0].letalidadeProporcional == cidade.letalidadeProporcional) {
                cidadeMaiorLetalidade[1] = cidade;
            }

            printf("\n\t%s\n", cidade.nome);

            for (int k = 0; k < cidade.numInfectados; ++k) {
                printf("\t\t");
                printPessoa(cidade.infectados[k]);
            }
        }
    }
}

int main() {
    printf("Hello, World!\n");
    return 0;
}

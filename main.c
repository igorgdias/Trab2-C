#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Data {
    int dia;
    int mes;
    int ano;
    int null;
} Data;

typedef struct CoronaVirus {
    Data dataInfeccao;
    int grauDeContagio;
    int grauDeLetalidade;
} CoronaVirus;

typedef struct Pessoa {
    char *nome;
    int idade;
    char sexo;
    CoronaVirus coronaVirus;
} Pessoa;

typedef struct Cidade {
    char *nome;
    Pessoa *pessoas;
    int populacao;
    Pessoa *infectados;
    int numInfectados;
    float infeccaoProporcional;
    char *estado;
} Cidade;

typedef struct Estado {
    char *nome;
    Cidade *cidades;
    int numCidades;
    int populacao;
} Estado;

typedef struct Pais {
    Estado *estados;
    int numEstados;
} Pais;

char *lerLinha(FILE *file) {
    int size = 50;
    char *linha = malloc(size * sizeof(char));
    int cont = 0;
    linha[cont] = (char) fgetc(file);
    if ((int) linha[cont] == EOF) return "\0\0";
    while (linha[cont] != '\n') {
        cont++;
        if (cont == size) {
            size += 50;
            linha = realloc(linha, size * sizeof(char));
        }
        linha[cont] = (char) fgetc(file);
    }
    return linha;
}

Estado sortCidade(Estado listCidades) {
    int aOrdenar = listCidades.numCidades - 1;
    Cidade temp;
    for (int i = 0; i < (listCidades.numCidades - 1); ++i) {
        for (int j = 0; j < aOrdenar; ++j) {
            if (strcmp(listCidades.cidades[j].estado, listCidades.cidades[j + 1].estado) > 0) {
                temp = listCidades.cidades[j];
                listCidades.cidades[j] = listCidades.cidades[j + 1];
                listCidades.cidades[j + 1] = temp;
            }
        }
    }
    return listCidades;
}

Pessoa montaPessoa(FILE *file) {
    char nome[50];
    int idade;
    char sexo;
    char data[10];
    Data dataStruct;
    char contagio[5];
    char letalidade[5];
    int cont = 0;
    int let = 0;
    Pessoa pessoa;

    if (fscanf(file, "%s %d %c %s %s %s", nome, &idade, &sexo, data, contagio, letalidade) == 0) {
        pessoa.nome = "\0\0";
        return pessoa;
    }

    if (strcmp(contagio, "Zero") == 0) {
        cont = 0;
    } else if (strcmp(contagio, "Baixo") == 0) {
        cont = 1;
    } else if (strcmp(contagio, "Medio") == 0) {
        cont = 2;
    } else if (strcmp(contagio, "Alto") == 0) {
        cont = 3;
    }

    if (strcmp(letalidade, "Zero") == 0) {
        let = 0;
    } else if (strcmp(letalidade, "Baixa") == 0) {
        let = 1;
    } else if (strcmp(letalidade, "Media") == 0) {
        let = 2;
    } else if (strcmp(letalidade, "Alta") == 0) {
        let = 3;
    }

    if (strcmp(data, "xx/xx/xxxx") == 0) {
        dataStruct.null = 1;
    } else {
        sscanf(data, "%2d %*c %2d %*c %d", &dataStruct.dia, &dataStruct.mes, &dataStruct.ano);
        dataStruct.null = 0;
    }

    pessoa.nome = nome;
    pessoa.sexo = sexo;
    pessoa.idade = idade;
    pessoa.coronaVirus.grauDeLetalidade = let;
    pessoa.coronaVirus.grauDeContagio = cont;
    pessoa.coronaVirus.dataInfeccao = dataStruct;

    return pessoa;
}

Estado montaCidades(FILE *file) {
    Estado tempEstado;
    Pessoa pessoa;
    FILE *arquivoCidade;
    char nome[50];
    char arquivo[100];
    char estado[2];
    int size = 50;
    int sizePop = 50;
    int sizeInfect = 50;
    int cont = 0;
    Cidade *cidades = malloc(size * sizeof(Cidade));
    Pessoa *pessoas = malloc(sizePop * sizeof(Pessoa));
    Pessoa *infectados = malloc(sizeInfect * sizeof(Pessoa));

    char *linha = lerLinha(file);

    while (strcmp(linha, "\0\0") != 0) {
        sscanf(linha, "%s %s %s", nome, estado, arquivo);

        cidades[cont].nome = nome;
        cidades[cont].estado = estado;

        arquivoCidade = fopen(arquivo, "r");
        pessoa = montaPessoa(arquivoCidade);

        cidades[cont].numInfectados = 0;
        cidades[cont].populacao = 0;
        cidades[cont].infeccaoProporcional = 0;

        while (strcmp(pessoa.nome, "\0\0") != 0) {
            if (pessoa.coronaVirus.dataInfeccao.null != 0 && pessoa.coronaVirus.grauDeContagio != 0 &&
                pessoa.coronaVirus.grauDeLetalidade != 0) {
                infectados[cidades[cont].numInfectados] = pessoa;
                cidades[cont].infeccaoProporcional += (float) pessoa.coronaVirus.grauDeContagio;
                cidades[cont].numInfectados++;
                if (cidades[cont].numInfectados == sizeInfect) {
                    sizeInfect += 50;
                    infectados = realloc(infectados, sizeInfect * sizeof(Pessoa));
                }
            }
            pessoas[cidades[cont].populacao] = pessoa;
            cidades[cont].populacao++;
            if (cidades[cont].populacao == sizePop) {
                sizePop += 50;
                pessoas = realloc(pessoas, sizePop * sizeof(Pessoa));
            }
        }

        pessoas = realloc(pessoas, cidades[cont].populacao * sizeof(Pessoa));
        infectados = realloc(infectados, cidades[cont].numInfectados * sizeof(Pessoa));
        cidades[cont].infectados = infectados;
        cidades[cont].pessoas = pessoas;
        cidades[cont].infeccaoProporcional /= (float) cidades[cont].populacao;
        sizePop = 50;
        sizeInfect = 50;
        pessoas = malloc(sizePop * sizeof(struct Pessoa));
        infectados = malloc(sizeInfect * sizeof(struct Pessoa));
        cont++;
        if (cont == size) {
            size += 50;
            cidades = realloc(cidades, size * sizeof(Cidade));
        }
    }
    cidades = realloc(cidades, cont * sizeof(Cidade));
    tempEstado.cidades = cidades;
    tempEstado.numCidades = cont;
    return tempEstado;
}

Pais montaEstados(FILE *file) {
    Pais pais;
    int size = 50;
    int sizeCity = 50;
    Estado listCidades = sortCidade(montaCidades(file));
    Estado tempEstado;
    Cidade *tempCidades = malloc(sizeCity * sizeof(Cidade));
    Cidade *cidades = listCidades.cidades;
    Estado *estados = malloc(size * sizeof(Estado));
    pais.numEstados = 0;
    tempEstado.numCidades = 0;
    tempEstado.nome = cidades[0].estado;
    for (int i = 0; i < listCidades.numCidades; ++i) {
        if (strcmp(tempEstado.nome, cidades[i].nome) != 0) {
            tempCidades = realloc(tempCidades, tempEstado.numCidades * sizeof(Cidade));
            tempEstado.cidades = tempCidades;
            estados[pais.numEstados] = tempEstado;
            pais.numEstados++;
            if (pais.numEstados == size) {
                size += 50;
                estados = realloc(estados, size * sizeof(Estado));
            }

            tempEstado.nome = cidades[i].nome;
            tempEstado.numCidades = 0;
            tempEstado.populacao = 0;
        }
        tempCidades[tempEstado.numCidades] = cidades[i];
        tempEstado.populacao += cidades[i].populacao;
        tempEstado.numCidades++;
        if (tempEstado.numCidades == sizeCity) {
            sizeCity += 50;
            tempCidades = realloc(tempCidades, sizeCity * sizeof(Cidade));
        }
    }
    estados = realloc(estados, pais.numEstados * sizeof(Estado));
    pais.estados = estados;
    return pais;
}

int intervaloData() {}

void printPessoa(Pessoa *pessoa) {
    char grauCont[4][5] = {"Zero", "Baixo", "Medio", "Alto"};
    char grauLet[4][5] = {"Zero", "Baixa", "Media", "Alta"};
    CoronaVirus *corona = &pessoa->coronaVirus;
    Data *data = &corona->dataInfeccao;
    printf("%s %d %c", pessoa->nome, pessoa->idade, pessoa->sexo);
    printf(" %d/%d/%d", data->dia, data->mes, data->ano);
    printf(" %s %s\n", grauCont[corona->grauDeContagio], grauLet[corona->grauDeLetalidade]);
}

void estatisticas(Pais *pais) {
    int size = 50;
    int cont = 0;
    float letalidadeEstadoIntervalo = 0;
    float letalidadeCidadeIntervalo = 0;
    float maiorLetalidadeEstadoIntervalo = 0;
    float maiorLetalidadeCidadeIntervalo = 0;
    Estado *estado;
    Cidade *cidade;
    Pessoa *infectado;
    Cidade *cidadeMaiorInfeccao[2];
    Cidade *cidadeMaiorLetalidadeIntervalo[2];
    Estado *estadoMaiorLetalidadeIntervalo[2];
    Pessoa **m80_100a = malloc(size * sizeof(Pessoa));
    estadoMaiorLetalidadeIntervalo[0] = &pais->estados[0];
    for (int i = 0; i < pais->numEstados; ++i) {
        estado = &pais->estados[i];

        printf("%s:\n", estado->nome);

        cidadeMaiorInfeccao[0] = &estado->cidades[0];
        cidadeMaiorLetalidadeIntervalo[0] = &estado->cidades[0];

        for (int j = 0; j < estado->numCidades; ++j) {
            cidade = &estado->cidades[j];

            if (cidadeMaiorInfeccao[0]->infeccaoProporcional < cidade->infeccaoProporcional) {
                cidadeMaiorInfeccao[0] = cidade;
                cidadeMaiorInfeccao[1] = cidade;
            } else if (cidadeMaiorInfeccao[0]->infeccaoProporcional == cidade->infeccaoProporcional) {
                cidadeMaiorInfeccao[1] = cidade;
            }

            printf("\n\t%s\n", cidade->nome);
            for (int k = 0; k < cidade->numInfectados; ++k) {
                infectado = &cidade->infectados[k];
                if (infectado->sexo == 'M' && infectado->idade >= 80 && infectado->idade <= 100 &&
                    infectado->coronaVirus.grauDeLetalidade == 3) {
                    m80_100a[cont] = infectado;
                    cont++;
                    if (cont == size) {
                        size += 50;
                        m80_100a = realloc(m80_100a, size * sizeof(Pessoa));
                    }
                }
                printf("\t\t");
                printPessoa(infectado);
            }
        }
    }
}

int main() {
    FILE *file = fopen("cidades.txt", "r");
    Pais pais = montaEstados(file);
    estatisticas(&pais);
    return 0;
}

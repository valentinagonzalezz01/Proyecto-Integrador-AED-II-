#ifndef TDA_PLANESTUDIOS_H
#define TDA_PLANESTUDIOS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "TDA_MATERIA.h"

#define N 30

/* DECLARACION DE TIPOS DE DATOS PERSONALIZADOS */
typedef int tVertice;

typedef struct {
    tVertice origen;
    tVertice destino;
} tArista;

typedef bool tConjuntoVertices[N];

typedef bool tConjuntoAristas[N][N];

typedef struct {
    tConjuntoVertices vertices;
    tConjuntoAristas aristas;
    tString nombres[N]; // Nombre de la materia asociada a cada vértice
} tGrafoNoPonderado;

/* DECLARACION DE PROTOTIPOS DE FUNCIONES */
void inicializarGrafo(tGrafoNoPonderado*);
void agregarVertice(tGrafoNoPonderado*, tVertice);
void agregarArista(tGrafoNoPonderado*, tArista);
void borrarVertice(tGrafoNoPonderado*, tVertice);
void borrarArista(tGrafoNoPonderado*, tVertice, tArista);
int maximoVertice(tGrafoNoPonderado);
void visualizarMatrizAdyacencia(tGrafoNoPonderado);
bool existeVertice(tGrafoNoPonderado, tVertice);
bool existeArista(tGrafoNoPonderado, tArista);

/* FUNCIONES PROPIAS DEL PLAN DE ESTUDIO */
void visualizarPlanEstudios(tGrafoNoPonderado);
bool puedeInscribirse(tGrafoNoPonderado, tVertice, tListaMaterias*);
void cargarPlanDesdeTexto(tGrafoNoPonderado*);

/* IMPLEMENTACION DE FUNCIONES */
void inicializarGrafo(tGrafoNoPonderado* pGrafoNP) {
    int x, y;
    
    for (x = 0; x < N; x++) {
        pGrafoNP->vertices[x] = false;
        strcpy(pGrafoNP->nombres[x], "");
        for (y = 0; y < N; y++) {
            pGrafoNP->aristas[x][y] = false;
        }
    }
    printf("Grafo no ponderado inicializado ...\n");
}

void agregarVertice(tGrafoNoPonderado* pGrafoNP, tVertice pVertice) {
    pGrafoNP->vertices[pVertice] = true;
    printf("Vertice %d insertado\n", pVertice);
}

void agregarArista(tGrafoNoPonderado* pGrafoNP, tArista pArista) {
    bool aristaValida = existeVertice(*pGrafoNP, pArista.origen) && 
                        existeVertice(*pGrafoNP, pArista.destino);
    if (aristaValida) {
        pGrafoNP->aristas[pArista.origen][pArista.destino] = true;
        printf("Arista activada!\n");
    } else {
        printf("Arista no valida | alguno de los nodos no esta activo\n");
    }
}

void borrarVertice(tGrafoNoPonderado* pGrafoNP, tVertice pVertice) {
    pGrafoNP->vertices[pVertice] = false;
    printf("Vertice desactivado: %d\n", pVertice);
}

void borrarArista(tGrafoNoPonderado* pGrafoNP, tVertice pVertice, tArista pArista) {
    if (existeArista(*pGrafoNP, pArista)) {
        pGrafoNP->aristas[pArista.origen][pArista.destino] = false;
    } else {
        printf("No existe la arista que se desea borrar!");
    }
}

int maximoVertice(tGrafoNoPonderado pGrafoNP) {
    int x;
    int max = 0;
    for (x = 1; x < N; x++) {
        if (pGrafoNP.vertices[x] == true) {
            max = x;
        }
    }
    return max;
}

void visualizarMatrizAdyacencia(tGrafoNoPonderado pGrafoNP) {
    int x, y;
    int maxVertice = maximoVertice(pGrafoNP);
    
    printf("     ");
    for (y = 1; y <= maxVertice; y++) {
        printf("%3d", y);
    }
    printf("\n    +");
    for (y = 1; y <= maxVertice; y++) {
        printf("---");
    }
    printf("\n");

    for (x = 1; x <= maxVertice; x++) {
        printf("[%2d]|", x);
        for (y = 1; y <= maxVertice; y++) {
            tArista aristaActual = { .origen = x, .destino = y };
            if (existeArista(pGrafoNP, aristaActual)) {
                printf("  1");
            } else {
                printf("  0");
            }
        }
        printf("\n");
    }
}

bool existeVertice(tGrafoNoPonderado pGrafoNP, tVertice pVertice) {
    return pGrafoNP.vertices[pVertice] == true;
}

bool existeArista(tGrafoNoPonderado pGrafoNP, tArista pArista) {
    return (pGrafoNP.aristas[pArista.origen][pArista.destino] == true)
            && existeVertice(pGrafoNP, pArista.origen)
            && existeVertice(pGrafoNP, pArista.destino);
}

/* Muestra el plan de estudios alineado en formato tabla */
void visualizarPlanEstudios(tGrafoNoPonderado pGrafoNP) {
    int x, y;
    int maxVertice = maximoVertice(pGrafoNP);
    int cantPrevias;
    
    printf("\n====================================================================================================\n");
    printf("                             PLAN DE ESTUDIOS Y REGIMEN DE CORRELATIVAS                             \n");
    printf("====================================================================================================\n");
    printf(" COD  | MATERIA                                      | CORRELATIVAS REQUERIDAS (PARA CURSAR)        \n");
    printf("------+----------------------------------------------+----------------------------------------------\n");
    
    for (y = 1; y <= maxVertice; y++) {
        if (existeVertice(pGrafoNP, y)) {
            printf(" [%2d] | %-44s | ", y, pGrafoNP.nombres[y]);
            cantPrevias = 0;
            
            for (x = 1; x <= maxVertice; x++) {
                tArista aristaActual = { .origen = x, .destino = y };
                if (existeArista(pGrafoNP, aristaActual)) {
                    if (cantPrevias == 0) {
                        printf("[%2d] %s\n", x, pGrafoNP.nombres[x]);
                    } else {
                        printf("      |                                              | [%2d] %s\n", x, pGrafoNP.nombres[x]);
                    }
                    cantPrevias++;
                }
            }
            
            if (cantPrevias == 0) {
                printf("Sin correlativas\n");
            }
        }
    }
    printf("====================================================================================================\n");
}

/* Verifica si el alumno tiene APROBADAS (nota >= 6) todas las materias previas requeridas */
bool puedeInscribirse(tGrafoNoPonderado pGrafoNP, tVertice pMateriaDestino, tListaMaterias* pHistorialAlumno) {
    int x;
    int maxVertice = maximoVertice(pGrafoNP);
    bool habilitado = true;
    int notaDesaprobada;
    
    if (!existeVertice(pGrafoNP, pMateriaDestino)) {
        printf("  [!] La materia solicitada no existe en el plan de estudios.\n");
        return false;
    }
    
    if (existeMateriaEnLista(pHistorialAlumno, pMateriaDestino)) {
        printf("  [!] El alumno ya tiene APROBADA la materia [%d] %s.\n", 
               pMateriaDestino, pGrafoNP.nombres[pMateriaDestino]);
        return false;
    }
    
    for (x = 1; x <= maxVertice; x++) {
        tArista aristaActual = { .origen = x, .destino = pMateriaDestino };
        if (existeArista(pGrafoNP, aristaActual)) {
            if (!existeMateriaEnLista(pHistorialAlumno, x)) {
                notaDesaprobada = obtenerNotaDesaprobada(pHistorialAlumno, x);
                if (notaDesaprobada != -1) {
                    printf("  [X] Correlativa DESAPROBADA: [%d] %s (Nota obtenida: %d - No aprobo)\n", 
                           x, pGrafoNP.nombres[x], notaDesaprobada);
                } else {
                    printf("  [X] Correlativa PENDIENTE: [%d] %s (No registra aprobacion)\n", 
                           x, pGrafoNP.nombres[x]);
                }
                habilitado = false;
            }
        }
    }
    
    return habilitado;
}

/* Lee materias.txt y correlativas.txt usando agregarVertice y agregarArista de clase */
void cargarPlanDesdeTexto(tGrafoNoPonderado* pGrafoNP) {
    FILE* archMaterias = fopen("materias.txt", "r");
    FILE* archCorrelativas;
    tVertice cod;
    tString nombre;
    tArista arista;
    char linea[150];
    char* inicio;
    int cantMaterias = 0;

    if (archMaterias != NULL) {
        while (fgets(linea, sizeof(linea), archMaterias) != NULL) {
            inicio = linea;
            if ((unsigned char)inicio[0] == 0xEF &&
                (unsigned char)inicio[1] == 0xBB &&
                (unsigned char)inicio[2] == 0xBF) {
                inicio += 3;
            }
            if (sscanf(inicio, "%d;%[^\r\n]", &cod, nombre) == 2) {
                agregarVertice(pGrafoNP, cod);
                strcpy(pGrafoNP->nombres[cod], nombre);
                cantMaterias++;
            }
        }
        fclose(archMaterias);
    }

    if (cantMaterias == 0) {
        archMaterias = fopen("materias.txt", "w");
        if (archMaterias != NULL) {
            fprintf(archMaterias, "1;Algoritmos y Estructuras de Datos I\n");
            fprintf(archMaterias, "2;Algoritmos y Estructuras de Datos II\n");
            fprintf(archMaterias, "3;Algebra\n");
            fprintf(archMaterias, "4;Logica y Matematica Computacional\n");
            fprintf(archMaterias, "5;Sistemas y Organizaciones\n");
            fprintf(archMaterias, "6;Calculo\n");
            fprintf(archMaterias, "7;Paradigmas\n");
            fprintf(archMaterias, "8;Arquitectura y Organizacion de Computadoras\n");
            fprintf(archMaterias, "9;Base de Datos\n");
            fprintf(archMaterias, "10;Sistemas Operativos\n");
            fprintf(archMaterias, "11;Programacion Orientada a Objetos\n");
            fprintf(archMaterias, "12;Ingles Tecnico\n");
            fclose(archMaterias);

            agregarVertice(pGrafoNP, 1);  strcpy(pGrafoNP->nombres[1],  "Algoritmos y Estructuras de Datos I");
            agregarVertice(pGrafoNP, 2);  strcpy(pGrafoNP->nombres[2],  "Algoritmos y Estructuras de Datos II");
            agregarVertice(pGrafoNP, 3);  strcpy(pGrafoNP->nombres[3],  "Algebra");
            agregarVertice(pGrafoNP, 4);  strcpy(pGrafoNP->nombres[4],  "Logica y Matematica Computacional");
            agregarVertice(pGrafoNP, 5);  strcpy(pGrafoNP->nombres[5],  "Sistemas y Organizaciones");
            agregarVertice(pGrafoNP, 6);  strcpy(pGrafoNP->nombres[6],  "Calculo");
            agregarVertice(pGrafoNP, 7);  strcpy(pGrafoNP->nombres[7],  "Paradigmas");
            agregarVertice(pGrafoNP, 8);  strcpy(pGrafoNP->nombres[8],  "Arquitectura y Organizacion de Computadoras");
            agregarVertice(pGrafoNP, 9);  strcpy(pGrafoNP->nombres[9],  "Base de Datos");
            agregarVertice(pGrafoNP, 10); strcpy(pGrafoNP->nombres[10], "Sistemas Operativos");
            agregarVertice(pGrafoNP, 11); strcpy(pGrafoNP->nombres[11], "Programacion Orientada a Objetos");
            agregarVertice(pGrafoNP, 12); strcpy(pGrafoNP->nombres[12], "Ingles Tecnico");
        }
    }

    archCorrelativas = fopen("correlativas.txt", "r");
    if (archCorrelativas != NULL) {
        while (fgets(linea, sizeof(linea), archCorrelativas) != NULL) {
            inicio = linea;
            if ((unsigned char)inicio[0] == 0xEF &&
                (unsigned char)inicio[1] == 0xBB &&
                (unsigned char)inicio[2] == 0xBF) {
                inicio += 3;
            }
            if (sscanf(inicio, "%d;%d", &arista.origen, &arista.destino) == 2) {
                agregarArista(pGrafoNP, arista);
            }
        }
        fclose(archCorrelativas);
    } else {
        printf("Aviso: No se encontro correlativas.txt\n");
    }
}

#endif

#ifndef TDA_ALUMNO_H
#define TDA_ALUMNO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "TDA_MATERIA.h"

#define MAX_CAD 50

/* DECLARACION DE TIPOS DE DATOS PERSONALIZADOS */
typedef struct {
    int legajo;
    long dni;
    tString nombre;
    int anioIngreso;
} tDatosAlumno;

typedef struct nodo {
    tDatosAlumno info;
    tListaMaterias* historial; // Puntero a la lista enlazada de materias
    struct nodo* hijoIzq;
    struct nodo* hijoDer;
} tArbol;

/* DECLARACION DE PROTOTIPOS DE DATOS */
void crearArbol(tArbol**);
bool arbolVacio(tArbol*);
void insertarNodo(tDatosAlumno, tArbol**);
void visualizarPreOrden(tArbol*);
void visualizarInOrden(tArbol*);
void visualizarPostOrden(tArbol*);

bool existeElemento(tArbol*, int);
void existeElemento2(tArbol*, int);
tArbol* buscarNodo(tArbol*, int);

tArbol* minimoElemento(tArbol*);
tArbol* maximoElemento(tArbol*);

void eliminarNodo(tArbol**, int);

void guardarArbolPreOrden(tArbol*, FILE*);
void guardarArchivoAlumnos(tArbol*);
void cargarArchivoAlumnos(tArbol**);

/* IMPLEMENTACION DE FUNCIONES */
void crearArbol(tArbol** pArbol) {
    *pArbol = NULL;
    printf("Arbol de alumnos creado ...\n");
}

bool arbolVacio(tArbol* pArbol) {
    return pArbol == NULL;
}

void insertarNodo(tDatosAlumno pInfo, tArbol** pArbol) {
    if (arbolVacio(*pArbol)) {
        *pArbol = (tArbol*) malloc(sizeof(tArbol));
        
        if (*pArbol != NULL) {
            (*pArbol)->info = pInfo;
            inicializarLista(&((*pArbol)->historial));
            (*pArbol)->hijoIzq = NULL;
            (*pArbol)->hijoDer = NULL;
        } else {
            printf("NO queda memoria disponible\n");
        }
    } else {
        if (pInfo.legajo < (*pArbol)->info.legajo) {
            insertarNodo(pInfo, &(*pArbol)->hijoIzq);
        } else {
            if (pInfo.legajo > (*pArbol)->info.legajo) {
                insertarNodo(pInfo, &(*pArbol)->hijoDer);
            } else {
                printf("Error! Legajo duplicado\n");
            }
        }
    }
}

void visualizarPreOrden(tArbol* pArbol) {
    if (!arbolVacio(pArbol)) {
        printf("Legajo: %d - %s (DNI: %ld, Ingreso: %d)\n", 
               pArbol->info.legajo, pArbol->info.nombre, 
               pArbol->info.dni, pArbol->info.anioIngreso);
        visualizarPreOrden(pArbol->hijoIzq);
        visualizarPreOrden(pArbol->hijoDer);
    }
}

void visualizarInOrden(tArbol* pArbol) {
    if (!arbolVacio(pArbol)) {
        visualizarInOrden(pArbol->hijoIzq);
        printf("Legajo: %d - %s (DNI: %ld, Ingreso: %d)\n", 
               pArbol->info.legajo, pArbol->info.nombre, 
               pArbol->info.dni, pArbol->info.anioIngreso);
        visualizarInOrden(pArbol->hijoDer);
    }
}

void visualizarPostOrden(tArbol* pArbol) {
    if (!arbolVacio(pArbol)) {
        visualizarPostOrden(pArbol->hijoIzq);
        visualizarPostOrden(pArbol->hijoDer);
        printf("Legajo: %d - %s (DNI: %ld, Ingreso: %d)\n", 
               pArbol->info.legajo, pArbol->info.nombre, 
               pArbol->info.dni, pArbol->info.anioIngreso);
    }
}

bool existeElemento(tArbol* pArbol, int pLegajo) {
    if (arbolVacio(pArbol)) {
        return false;
    } else {
        if (pLegajo < pArbol->info.legajo) {
            return existeElemento(pArbol->hijoIzq, pLegajo);
        } else {
            if (pLegajo > pArbol->info.legajo) {
                return existeElemento(pArbol->hijoDer, pLegajo);
            } else {
                return true;
            }
        }
    }
}

void existeElemento2(tArbol* pArbol, int pLegajo) {
    if (arbolVacio(pArbol)) {
        printf("\nNo existe un alumno con legajo: %d\n", pLegajo);
    } else {
        if (pLegajo < pArbol->info.legajo) {
            existeElemento2(pArbol->hijoIzq, pLegajo);
        } else {
            if (pLegajo > pArbol->info.legajo) {
                existeElemento2(pArbol->hijoDer, pLegajo);
            } else {
                printf("\nExiste un alumno con legajo: %d (%s)\n", 
                       pLegajo, pArbol->info.nombre);
            }
        }
    }
}

tArbol* buscarNodo(tArbol* pArbol, int pLegajo) {
    if (arbolVacio(pArbol)) {
        return NULL;
    } else {
        if (pLegajo < pArbol->info.legajo) {
            return buscarNodo(pArbol->hijoIzq, pLegajo);
        } else {
            if (pLegajo > pArbol->info.legajo) {
                return buscarNodo(pArbol->hijoDer, pLegajo);
            } else {
                return pArbol;
            }
        }
    }
}

tArbol* minimoElemento(tArbol* pArbol) {
    if (arbolVacio(pArbol)) {
        return NULL;
    } else {
        if (pArbol->hijoIzq != NULL) {
            return minimoElemento(pArbol->hijoIzq);
        } else {
            return pArbol;
        }
    }
}

tArbol* maximoElemento(tArbol* pArbol) {
    if (arbolVacio(pArbol)) {
        return NULL;
    } else {
        if (pArbol->hijoDer != NULL) {
            return maximoElemento(pArbol->hijoDer);
        } else {
            return pArbol;
        }
    }
}

void eliminarNodo(tArbol** pArbol, int pLegajo) {
    tArbol* aux;
    tArbol* padreAux;
    
    if (arbolVacio(*pArbol)) {
        printf("\nNo existe un alumno con legajo: %d para quitar.\n", pLegajo);
    } else {
        if (pLegajo < (*pArbol)->info.legajo) {
            eliminarNodo(&((*pArbol)->hijoIzq), pLegajo);
        } else {
            if (pLegajo > (*pArbol)->info.legajo) {
                eliminarNodo(&((*pArbol)->hijoDer), pLegajo);
            } else {
                /* Mostrar los datos del alumno que se va a eliminar */
                printf("\n--- ALUMNO ELIMINADO DEL PADRON ---\n");
                printf("  Legajo  : %d\n", (*pArbol)->info.legajo);
                printf("  Nombre  : %s\n", (*pArbol)->info.nombre);
                printf("  DNI     : %ld\n", (*pArbol)->info.dni);
                printf("  Ingreso : %d\n", (*pArbol)->info.anioIngreso);
                printf("-----------------------------------\n");

                if ((*pArbol)->hijoIzq == NULL) {
                    aux = *pArbol;
                    vaciarLista(&(aux->historial));
                    *pArbol = (*pArbol)->hijoDer;
                    free(aux);
                } else {
                    if ((*pArbol)->hijoDer == NULL) {
                        aux = *pArbol;
                        vaciarLista(&(aux->historial));
                        *pArbol = (*pArbol)->hijoIzq;
                        free(aux);
                    } else {
                        padreAux = *pArbol;
                        aux = (*pArbol)->hijoDer;
                        while (aux->hijoIzq != NULL) {
                            padreAux = aux;
                            aux = aux->hijoIzq;
                        }
                        
                        vaciarLista(&((*pArbol)->historial));
                        (*pArbol)->info = aux->info;
                        (*pArbol)->historial = aux->historial;
                        
                        if (padreAux == *pArbol) {
                            padreAux->hijoDer = aux->hijoDer;
                        } else {
                            padreAux->hijoIzq = aux->hijoDer;
                        }
                        free(aux);
                    }
                }
            }
        }
    }
}

void guardarArbolPreOrden(tArbol* pArbol, FILE* pArchivo) {
    if (!arbolVacio(pArbol)) {
        fwrite(&(pArbol->info), sizeof(tDatosAlumno), 1, pArchivo);
        guardarArbolPreOrden(pArbol->hijoIzq, pArchivo);
        guardarArbolPreOrden(pArbol->hijoDer, pArchivo);
    }
}

void guardarArchivoAlumnos(tArbol* pArbol) {
    FILE* archivo = fopen("alumnos.dat", "wb");
    if (archivo != NULL) {
        guardarArbolPreOrden(pArbol, archivo);
        fclose(archivo);
        printf("\nAlumnos guardados en alumnos.dat\n");
    } else {
        printf("\nError al abrir alumnos.dat\n");
    }
}

void cargarArchivoAlumnos(tArbol** pArbol) {
    FILE* archivo = fopen("alumnos.dat", "rb");
    tDatosAlumno reg;
    
    if (archivo != NULL) {
        fread(&reg, sizeof(tDatosAlumno), 1, archivo);
        while (!feof(archivo)) {
            insertarNodo(reg, pArbol);
            fread(&reg, sizeof(tDatosAlumno), 1, archivo);
        }
        fclose(archivo);
    }
}

#endif               //cierra el bloque (equivale a la llave '}' de un if común)

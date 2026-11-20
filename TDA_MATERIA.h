#ifndef TDA_MATERIA_H
#define TDA_MATERIA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXCAR 50

/* DECLARACION DE TIPOS DE DATOS PERSONALIZADOS */
typedef char tString[MAXCAR];

typedef struct {
    int codigo;
    tString nombre;
    int nota;
    tString fecha;
} tMateria;

typedef struct {
    int legajo;
    tMateria materia;
} tRegHistorial;

typedef struct nodoMateria {
    tMateria materia;
    struct nodoMateria* siguienteMateria;
} tListaMaterias;

/* DECLARACION DE PROTOTIPOS DE DATOS */
void inicializarLista(tListaMaterias**);
bool listaVacia(tListaMaterias*);

void insertarMateria(tListaMaterias**, tMateria);
void insertarPrimeraMateria(tListaMaterias**, tMateria);
void insertarMateriaAdelante(tListaMaterias**, tMateria);

void eliminarPrimeraMateria(tListaMaterias**);
void mostrarMaterias(tListaMaterias*);
int contarMaterias(tListaMaterias*);
float calcularPromedio(tListaMaterias*);
float calcularPromedioSinAplazos(tListaMaterias*);
bool existeMateriaEnLista(tListaMaterias*, int);
int obtenerNotaDesaprobada(tListaMaterias*, int);
void vaciarLista(tListaMaterias**);
void insertaMateriaEnPos(tListaMaterias**, tMateria, int);
void eliminarMateriaDePos(tListaMaterias**, int);

/* IMPLEMENTACION DE FUNCIONES */
void inicializarLista(tListaMaterias** pListaMaterias) {
    *pListaMaterias = NULL;
}

bool listaVacia(tListaMaterias* pListaMaterias) {
    return (pListaMaterias == NULL);
}

void insertarMateria(tListaMaterias** pListaMaterias, tMateria pMateria) {
    if (listaVacia(*pListaMaterias)) {
        insertarPrimeraMateria(&(*pListaMaterias), pMateria);
    } else {
        insertarMateriaAdelante(&(*pListaMaterias), pMateria);
    }
}

void insertarPrimeraMateria(tListaMaterias** pListaMaterias, tMateria pMateria) {
    tListaMaterias* nuevaMateria;
    nuevaMateria = (tListaMaterias*) malloc(sizeof(tListaMaterias));
    nuevaMateria->materia = pMateria;
    nuevaMateria->siguienteMateria = NULL;
    *pListaMaterias = nuevaMateria;
}

void insertarMateriaAdelante(tListaMaterias** pListaMaterias, tMateria pMateria) {
    tListaMaterias* nuevaMateria;
    nuevaMateria = (tListaMaterias*) malloc(sizeof(tListaMaterias));
    nuevaMateria->materia = pMateria;
    nuevaMateria->siguienteMateria = *pListaMaterias;
    *pListaMaterias = nuevaMateria;
}

void eliminarPrimeraMateria(tListaMaterias** pListaMaterias) {
    if (listaVacia(*pListaMaterias)) {
        printf("No hay materias para quitar de la lista!\n");
    } else {
        tListaMaterias* nodoSuprimir = *pListaMaterias;
        *pListaMaterias = nodoSuprimir->siguienteMateria;
        
        printf("Primera materia eliminada: %d - %s\n", 
               nodoSuprimir->materia.codigo, 
               nodoSuprimir->materia.nombre);
        free(nodoSuprimir);
        nodoSuprimir = NULL;
    }
}

void mostrarMaterias(tListaMaterias* pListaMaterias) {
    printf("\n*** HISTORIAL ACADEMICO DEL ALUMNO ***\n");
    printf("--------------------------------------------------------------------------------\n");
    if (listaVacia(pListaMaterias)) {
        printf("No hay materias registradas en el historial.\n");
    } else {
        tListaMaterias* aux = pListaMaterias;
        while (aux != NULL) {
            printf("  [%2d] %-42s | Nota: %2d (%-11s) | %s\n", 
                   aux->materia.codigo, 
                   aux->materia.nombre,
                   aux->materia.nota,
                   (aux->materia.nota >= 6) ? "APROBADA" : "DESAPROBADA",
                   aux->materia.fecha);
            aux = aux->siguienteMateria;
        }
    }
    printf("--------------------------------------------------------------------------------\n");
}

/* Cuenta UNICAMENTE las materias aprobadas (nota >= 6) */
int contarMaterias(tListaMaterias* pListaMaterias) {
    int cantidad = 0;
    tListaMaterias* aux = pListaMaterias;
    
    while (aux != NULL) {
        if (aux->materia.nota >= 6) {
            cantidad++;
        }
        aux = aux->siguienteMateria;
    }
    
    return cantidad;
}

/* Calcula el promedio general incluyendo aplazos (todas las notas registradas) */
float calcularPromedio(tListaMaterias* pListaMaterias) {
    int suma = 0;
    int cantidad = 0;
    float promedio = 0.0;
    tListaMaterias* aux = pListaMaterias;
    
    while (aux != NULL) {
        suma = suma + aux->materia.nota;
        cantidad++;
        aux = aux->siguienteMateria;
    }
    
    if (cantidad > 0) {
        promedio = (float) suma / cantidad;
    }
    
    return promedio;
}

/* Calcula el promedio sin aplazos (solo materias aprobadas con nota >= 6) */
float calcularPromedioSinAplazos(tListaMaterias* pListaMaterias) {
    int suma = 0;
    int cantidad = 0;
    float promedio = 0.0;
    tListaMaterias* aux = pListaMaterias;
    
    while (aux != NULL) {
        if (aux->materia.nota >= 6) {
            suma = suma + aux->materia.nota;
            cantidad++;
        }
        aux = aux->siguienteMateria;
    }
    
    if (cantidad > 0) {
        promedio = (float) suma / cantidad;
    }
    
    return promedio;
}

/* Devuelve true SOLO si la materia figura con nota aprobada (>= 6) */
bool existeMateriaEnLista(tListaMaterias* pListaMaterias, int pCodigo) {
    tListaMaterias* aux = pListaMaterias;
    bool aprobada = false;
    
    while (aux != NULL && !aprobada) {
        if (aux->materia.codigo == pCodigo && aux->materia.nota >= 6) {
            aprobada = true;
        } else {
            aux = aux->siguienteMateria;
        }
    }
    
    return aprobada;
}

/* Devuelve la nota desaprobada (< 6) si la rindio mal y aun no la aprobo, o -1 si nunca la rindio */
int obtenerNotaDesaprobada(tListaMaterias* pListaMaterias, int pCodigo) {
    tListaMaterias* aux = pListaMaterias;
    int notaDesaprobada = -1;
    
    while (aux != NULL && notaDesaprobada == -1) {
        if (aux->materia.codigo == pCodigo && aux->materia.nota < 6) {
            notaDesaprobada = aux->materia.nota;
        } else {
            aux = aux->siguienteMateria;
        }
    }
    
    return notaDesaprobada;
}

void vaciarLista(tListaMaterias** pListaMaterias) {
    tListaMaterias* nodoSuprimir;
    
    while (!listaVacia(*pListaMaterias)) {
        nodoSuprimir = *pListaMaterias;
        *pListaMaterias = nodoSuprimir->siguienteMateria;
        free(nodoSuprimir);
    }
}

void insertaMateriaEnPos(tListaMaterias** pListaMaterias, tMateria pMateria, int posicion) {
    tListaMaterias* listaAux = *pListaMaterias;
    int i;
    for (i = 1; i < posicion - 1; i++) {
        listaAux = listaAux->siguienteMateria;
    }
    tListaMaterias* nuevoNodo = (tListaMaterias*) malloc(sizeof(tListaMaterias));
    nuevoNodo->materia = pMateria;
    nuevoNodo->siguienteMateria = listaAux->siguienteMateria;
    listaAux->siguienteMateria = nuevoNodo;
}

void eliminarMateriaDePos(tListaMaterias** pListaMaterias, int posicion) {
    tListaMaterias* listaAux = *pListaMaterias;
    int i;
    for (i = 1; i < posicion - 1; i++) {
        listaAux = listaAux->siguienteMateria;
    }
    tListaMaterias* materiaSuprimir = listaAux->siguienteMateria;
    listaAux->siguienteMateria = materiaSuprimir->siguienteMateria;
    free(materiaSuprimir);
    materiaSuprimir = NULL;
}

#endif

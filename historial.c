#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "TDA_MATERIA.h"
#include "TDA_Alumno.h"
#include "TDA_PLANESTUDIOS.h"

/* PROTOTIPOS DE FUNCIONES DEL PROGRAMA PRINCIPAL */
void cargarDatosAlumno(tDatosAlumno*);
void guardarHistorialInOrden(tArbol*, FILE*);
void guardarArchivoHistorial(tArbol*);
void cargarArchivoHistorial(tArbol*);
void exportarAnaliticoTxt(tArbol*, int);

int main() {
    tArbol* arbolAlumnos;
    tGrafoNoPonderado planEstudios;
    
    tDatosAlumno infoAlumno;
    tMateria infoMateria;
    tArista nuevaCorrelativa;
    tArbol* alumnoEncontrado;
    
    int opcion, legajo, codMateria;
    tString nombreMat;

    /*Inicialización de estructuras */
    crearArbol(&arbolAlumnos);
    inicializarGrafo(&planEstudios);

    cargarPlanDesdeTexto(&planEstudios);
    cargarArchivoAlumnos(&arbolAlumnos);
    cargarArchivoHistorial(arbolAlumnos);

    do {
        printf("\n====================================================\n");
        printf("     SISTEMA DE GESTION ACADEMICA Y CORRELATIVAS (SIGA))  \n");
        printf("====================================================\n");
        printf("1. Registrar nuevo alumno\n");
        printf("2. Listar padron de alumnos ordenado (In-Orden)\n");
        printf("3. Consultar ficha e historial de un alumno\n");
        printf("4. Eliminar alumno por legajo\n");
        printf("5. Registrar calificacion de materia a un alumno\n");
        printf("6. Validar inscripcion a materia (Correlatividades)\n");
        printf("7. Ver Plan de Estudios y correlatividades\n");
        printf("8. Ver Matriz de Adyacencia del Plan de Estudios\n");
        printf("9. Agregar materia o correlatividad al Plan\n");
        printf("10. Emitir certificado analitico (Pantalla y .txt)\n");
        printf("0. Guardar todo y salir\n");
        printf("----------------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: {
                cargarDatosAlumno(&infoAlumno);
                insertarNodo(infoAlumno, &arbolAlumnos);
                break;
            }
            case 2: {
                if (arbolVacio(arbolAlumnos)) {
                    printf("\nNo hay alumnos registrados en el padron.\n");
                } else {
                    printf("\n--- PADRON GENERAL DE ALUMNOS (ORDENADO POR LEGAJO) ---\n");
                    visualizarInOrden(arbolAlumnos);
                }
                break;
            }
            case 3: {
                printf("\nIngrese el legajo del alumno: ");
                scanf("%d", &legajo);
                alumnoEncontrado = buscarNodo(arbolAlumnos, legajo);
                
                if (alumnoEncontrado != NULL) {
                    printf("\n--- FICHA DEL ESTUDIANTE ---\n");
                    printf("Legajo: %d | Nombre: %s | DNI: %ld | Ingreso: %d\n",
                           alumnoEncontrado->info.legajo,
                           alumnoEncontrado->info.nombre,
                           alumnoEncontrado->info.dni,
                           alumnoEncontrado->info.anioIngreso);
                    mostrarMaterias(alumnoEncontrado->historial);
                    printf("Total materias aprobadas      : %d\n", contarMaterias(alumnoEncontrado->historial));
                    printf("Promedio general (c/ aplazos) : %.2f\n", calcularPromedio(alumnoEncontrado->historial));
                    printf("Promedio sin aplazos          : %.2f\n", calcularPromedioSinAplazos(alumnoEncontrado->historial));
                } else {
                    printf("\nNo se encontro un alumno con el legajo %d.\n", legajo);
                }
                break;
            }
            case 4: {
                printf("\nIngrese el legajo del alumno a eliminar: ");
                scanf("%d", &legajo);
                eliminarNodo(&arbolAlumnos, legajo);
                break;
            }
            case 5: {
                printf("\nIngrese el legajo del alumno: ");
                scanf("%d", &legajo);
                alumnoEncontrado = buscarNodo(arbolAlumnos, legajo);
                
                if (alumnoEncontrado != NULL) {
                    printf("Ingrese codigo de materia: ");
                    scanf("%d", &codMateria);
                    
                    if (!existeVertice(planEstudios, codMateria)) {
                        printf("Error: El codigo %d no existe en el Plan de Estudios.\n", codMateria);
                    } else if (existeMateriaEnLista(alumnoEncontrado->historial, codMateria)) {
                        printf("El alumno ya tiene APROBADA esa materia.\n");
                    } else {
                        infoMateria.codigo = codMateria;
                        strcpy(infoMateria.nombre, planEstudios.nombres[codMateria]);
                        
                        printf("Materia: %s\n", infoMateria.nombre);
                        printf("Ingrese nota obtenida (1 a 10, aprueba con 6): ");
                        scanf("%d", &infoMateria.nota);
                        
                        if (infoMateria.nota >= 1 && infoMateria.nota <= 10) {
                            printf("Ingrese fecha (dd/mm/aaaa): ");
                            scanf(" %[^\n]", infoMateria.fecha);
                            
                            insertarMateria(&(alumnoEncontrado->historial), infoMateria);
                            if (infoMateria.nota >= 6) {
                                printf(">> Materia APROBADA registrada en el historial de %s.\n", alumnoEncontrado->info.nombre);
                            } else {
                                printf(">> Materia registrada como DESAPROBADA (nota %d). No habilita correlativas.\n", infoMateria.nota);
                            }
                        } else {
                            printf("Error: La nota debe estar entre 1 y 10.\n");
                        }
                    }
                } else {
                    printf("\nNo existe un alumno con legajo %d.\n", legajo);
                }
                break;
            }
            case 6: {
                printf("\nIngrese el legajo del alumno: ");
                scanf("%d", &legajo);
                alumnoEncontrado = buscarNodo(arbolAlumnos, legajo);
                
                if (alumnoEncontrado != NULL) {
                    printf("Ingrese codigo de materia a la que desea inscribirse: ");
                    scanf("%d", &codMateria);
                    
                    printf("\nVerificando requisitos para [%d] %s...\n",
                           codMateria,
                           existeVertice(planEstudios, codMateria) ? planEstudios.nombres[codMateria] : "Desconocida");
                           
                    if (puedeInscribirse(planEstudios, codMateria, alumnoEncontrado->historial)) {
                        printf(">> INSCRIPCION HABILITADA: Cumple con todas las correlatividades aprobadas.\n");
                    } else {
                        printf(">> INSCRIPCION RECHAZADA: No cumple con las correlatividades requeridas.\n");
                    }
                } else {
                    printf("\nNo existe un alumno con legajo %d.\n", legajo);
                }
                break;
            }
            case 7: {
                visualizarPlanEstudios(planEstudios);
                break;
            }
            case 8: {
                printf("\n--- MATRIZ DE ADYACENCIA ---\n");
                visualizarMatrizAdyacencia(planEstudios);
                break;
            }
            case 9: {
                int subOp;
                printf("\n1. Agregar materia al plan\n2. Agregar correlatividad\nOpcion: ");
                scanf("%d", &subOp);
                
                if (subOp == 1) {
                    printf("Codigo de materia (1 a %d): ", N - 1);
                    scanf("%d", &codMateria);
                    printf("Nombre de la materia: ");
                    scanf(" %[^\n]", nombreMat);
                    
                    agregarVertice(&planEstudios, codMateria);
                    strcpy(planEstudios.nombres[codMateria], nombreMat);
                } else if (subOp == 2) {
                    printf("Codigo materia previa (origen): ");
                    scanf("%d", &nuevaCorrelativa.origen);
                    printf("Codigo materia siguiente (destino): ");
                    scanf("%d", &nuevaCorrelativa.destino);
                    
                    agregarArista(&planEstudios, nuevaCorrelativa);
                }
                break;
            }
            case 10: {
                printf("\nIngrese el legajo para emitir su analitico: ");
                scanf("%d", &legajo);
                exportarAnaliticoTxt(arbolAlumnos, legajo);
                break;
            }
            case 0: {
                guardarArchivoAlumnos(arbolAlumnos);
                guardarArchivoHistorial(arbolAlumnos);
                printf("\nDatos guardados correctamente. Saliendo del sistema...\n");
                break;
            }
            default: {
                printf("\nOpcion invalida.\n");
                break;
            }
        }
    } while (opcion != 0);

    return 0;
}

/* Carga por teclado de los datos de un alumno */
void cargarDatosAlumno(tDatosAlumno* pAux) {
    printf("\nIngrese numero de legajo: ");
    scanf("%d", &pAux->legajo);
    
    printf("Ingrese DNI: ");
    scanf("%ld", &pAux->dni);
    
    printf("Ingrese Nombre y Apellido: ");
    scanf(" %[^\n]", pAux->nombre);
    
    printf("Ingrese anio de ingreso: ");
    scanf("%d", &pAux->anioIngreso);
}

/* Recorre el arbol en In-Orden para guardar historial.dat ordenado por legajo */
void guardarHistorialInOrden(tArbol* pArbol, FILE* pArchivo) {
    if (!arbolVacio(pArbol)) {
        guardarHistorialInOrden(pArbol->hijoIzq, pArchivo);
        
        tListaMaterias* aux = pArbol->historial;
        tRegHistorial reg;
        while (aux != NULL) {
            reg.legajo = pArbol->info.legajo;
            reg.materia = aux->materia;
            fwrite(&reg, sizeof(tRegHistorial), 1, pArchivo);
            aux = aux->siguienteMateria;
        }
        
        guardarHistorialInOrden(pArbol->hijoDer, pArchivo);
    }
}

void guardarArchivoHistorial(tArbol* pArbol) {
    FILE* archivo = fopen("historial.dat", "wb");
    if (archivo != NULL) {
        guardarHistorialInOrden(pArbol, archivo);
        fclose(archivo);
        printf("Historiales guardados en historial.dat\n");
    } else {
        printf("Error al abrir historial.dat\n");
    }
}

/* Carga historial.dat aplicando CORTE DE CONTROL por numero de legajo */
void cargarArchivoHistorial(tArbol* pArbol) {
    FILE* archivo = fopen("historial.dat", "rb");
    tRegHistorial reg;
    int legajoAnterior;
    tArbol* alumnoActual;
    
    if (archivo != NULL) {
        fread(&reg, sizeof(tRegHistorial), 1, archivo);
        while (!feof(archivo)) {
            legajoAnterior = reg.legajo;
            alumnoActual = buscarNodo(pArbol, legajoAnterior);
            
            while (!feof(archivo) && reg.legajo == legajoAnterior) {
                if (alumnoActual != NULL) {
                    insertarMateria(&(alumnoActual->historial), reg.materia);
                }
                fread(&reg, sizeof(tRegHistorial), 1, archivo);
            }
        }
        fclose(archivo);
    }
}

/* Muestra el certificado analitico completo en pantalla y lo exporta a archivo .txt */
void exportarAnaliticoTxt(tArbol* pArbol, int pLegajo) {
    tArbol* alu = buscarNodo(pArbol, pLegajo);
    if (alu == NULL) {
        printf("\nNo existe un alumno con legajo %d.\n", pLegajo);
        return;
    }
    
    tListaMaterias* aux = alu->historial;
    int totalAprobadas = contarMaterias(alu->historial);
    float promedioGeneral = calcularPromedio(alu->historial);
    float promedioSinAplazos = calcularPromedioSinAplazos(alu->historial);
    
    /*Muestra todo el analitico en pantalla */
    printf("\n================================================================================\n");
    printf("                        CERTIFICADO ANALITICO DE ESTUDIOS                       \n");
    printf("================================================================================\n");
    printf("  Alumno  : %s\n", alu->info.nombre);
    printf("  Legajo  : %d\n", alu->info.legajo);
    printf("  DNI     : %ld\n", alu->info.dni);
    printf("  Ingreso : %d\n", alu->info.anioIngreso);
    printf("--------------------------------------------------------------------------------\n");
    printf("  COD  | MATERIA                                      | NOTA | ESTADO      | FECHA\n");
    printf("-------+----------------------------------------------+------+-------------+-----------\n");
    
    if (aux == NULL) {
        printf("  (El alumno aun no registra materias cursadas)\n");
    } else {
        while (aux != NULL) {
            printf("  [%2d] | %-44s |  %2d  | %-11s | %s\n",
                   aux->materia.codigo,
                   aux->materia.nombre,
                   aux->materia.nota,
                   (aux->materia.nota >= 6) ? "APROBADA" : "DESAPROBADA",
                   aux->materia.fecha);
            aux = aux->siguienteMateria;
        }
    }
    
    printf("--------------------------------------------------------------------------------\n");
    printf("  Total materias aprobadas      : %d\n", totalAprobadas);
    printf("  Promedio general (c/ aplazos) : %.2f\n", promedioGeneral);
    printf("  Promedio sin aplazos          : %.2f\n", promedioSinAplazos);
    printf("================================================================================\n");
    
    /* Genera el archivo de texto (.txt) con el mismo formato */
    char nombreArchivo[60];
    sprintf(nombreArchivo, "analitico_%d.txt", pLegajo);
    
    FILE* txt = fopen(nombreArchivo, "w");
    if (txt != NULL) {
        fprintf(txt, "================================================================================\n");
        fprintf(txt, "                        CERTIFICADO ANALITICO DE ESTUDIOS                       \n");
        fprintf(txt, "================================================================================\n");
        fprintf(txt, "  Alumno  : %s\n", alu->info.nombre);
        fprintf(txt, "  Legajo  : %d\n", alu->info.legajo);
        fprintf(txt, "  DNI     : %ld\n", alu->info.dni);
        fprintf(txt, "  Ingreso : %d\n", alu->info.anioIngreso);
        fprintf(txt, "--------------------------------------------------------------------------------\n");
        fprintf(txt, "  COD  | MATERIA                                      | NOTA | ESTADO      | FECHA\n");
        fprintf(txt, "-------+----------------------------------------------+------+-------------+-----------\n");
        
        aux = alu->historial;
        if (aux == NULL) {
            fprintf(txt, "  (El alumno aun no registra materias cursadas)\n");
        } else {
            while (aux != NULL) {
                fprintf(txt, "  [%2d] | %-44s |  %2d  | %-11s | %s\n",
                        aux->materia.codigo,
                        aux->materia.nombre,
                        aux->materia.nota,
                        (aux->materia.nota >= 6) ? "APROBADA" : "DESAPROBADA",
                        aux->materia.fecha);
                aux = aux->siguienteMateria;
            }
        }
        
        fprintf(txt, "--------------------------------------------------------------------------------\n");
        fprintf(txt, "  Total materias aprobadas      : %d\n", totalAprobadas);
        fprintf(txt, "  Promedio general (c/ aplazos) : %.2f\n", promedioGeneral);
        fprintf(txt, "  Promedio sin aplazos          : %.2f\n", promedioSinAplazos);
        fprintf(txt, "================================================================================\n");
        fclose(txt);
        
        printf("\n>> Certificado guardado tambien en archivo de texto: '%s'\n", nombreArchivo);
    } else {
        printf("\nError al crear el archivo de texto.\n");
    }
}

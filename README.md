# Sistema Integral de Gestión Académica ("SIGA") en C

<p align="center">

<img src="https://img.shields.io/badge/-LENGUAJE-2D2D2D?style=for-the-badge&logo=c&logoColor=white&labelColor=E53935">
<img src="https://img.shields.io/badge/Estructuras%20de%20Datos-ABB%20%7C%20Listas%20%7C%20Grafos-6C63FF?style=for-the-badge">
<img src="https://img.shields.io/badge/Acad%C3%A9mico-Proyecto%20Integrador-2F6BFF?style=for-the-badge">
<img src="https://img.shields.io/badge/Estado-Completado-2EA44F?style=for-the-badge">

</p>

## Acerca del Proyecto

**SIGA (Sistema Integral de Gestión Académica y Control de Correlatividades)** es una aplicación modular de consola desarrollada en **lenguaje C**.

El sistema permite administrar el padrón de estudiantes de una carrera, registrar sus historiales de materias rendidas y validar automáticamente si cumplen con las correlatividades requeridas antes de inscribirse a nuevas asignaturas.

Este proyecto fue desarrollado para integrar estructuras de datos dinámicas y no lineales junto con técnicas de persistencia híbrida, respetando la programación estructurada y el encapsulamiento mediante Tipos de Datos Abstractos (TDAs).

---

## Funcionalidades
- Registrar nuevo alumno (con control de legajos duplicados)
- Listar padrón de alumnos ordenado (recorrido In-Orden)
- Consultar ficha e historial académico de un alumno
- Eliminar alumno por legajo (visualizando sus datos y liberando su lista)
- Registrar calificación de materia a un alumno (escala de 1 a 10)
- Validar inscripción a materia según el régimen de correlatividades
- Visualizar Plan de Estudios y correlatividades en formato tabla
- Visualizar Matriz de Adyacencia del Plan de Estudios
- Agregar nueva materia o correlatividad al Plan de Estudios
- Emitir certificado analítico (en pantalla y exportación a archivo `.txt`)
- Guardar todo y salir (persistencia automática en archivos binarios)

---

## Estructuras de Datos

La aplicación utiliza diferentes estructuras de datos dinámicas y no lineales:

### Árbol Binario de Búsqueda (ABB)

Administra el padrón general de estudiantes ordenado por número de legajo, permitiendo búsquedas, altas sin duplicados y bajas eficientes sin recorrer secuencialmente todo el padrón.

### Listas Enlazadas Simples

Almacenan el historial dinámico de calificaciones dentro de cada nodo del árbol, evitando el desperdicio de espacio frente a arreglos de tamaño fijo y soportando una cantidad variable de materias por estudiante.

### Grafos Dirigidos No Ponderados

Modelan las asignaturas como vértices y las correlatividades como aristas dirigidas sobre una Matriz de Adyacencia (`materia previa → materia destino`) sin peso numérico.

### Tipos de Datos Abstractos (TDAs)

El proyecto separa cada estructura de datos y sus operaciones públicas en archivos de cabecera independientes (`.h`) reutilizables.

---

## Conceptos Principales

Este proyecto aplica:
- Programación en C
- Funciones y programación estructurada
- Estructuras y punteros
- Memoria dinámica y liberación de memoria
- Tipos de Datos Abstractos (TDAs)
- Árboles Binarios de Búsqueda (recorridos Pre-Orden, In-Orden y Post-Orden)
- Listas enlazadas simples
- Grafos dirigidos y Matriz de Adyacencia
- Manejo de archivos binarios (`.dat`) y de texto (`.txt`)
- Algoritmo de Corte de Control
- Validaciones cruzadas y de entrada

---

## Estructura del Proyecto
sistema-gestion-academica-siga/
├── historial.c
├── TDA_Alumno.h
├── TDA_MATERIA.h
├── TDA_PLANESTUDIOS.h
├── materias.txt
├── correlativas.txt
├── alumnos.dat
├── historial.dat
└── README.md

 ## ARCHIVOS PRINCIPALES: 
## historial.c:
Programa principal, menú interactivo, validaciones cruzadas y Corte de Control.
## TDA_Alumno.h: 
Implementación del Árbol Binario de Búsqueda (ABB) para el padrón de alumnos.
## TDA_MATERIA.h:
Implementación de Lista Enlazada Simple para el historial de calificaciones.
## TDA_PLANESTUDIOS.h:
Implementación de Grafo Dirigido No Ponderado mediante Matriz de Adyacencia.
## materias.txt:
Archivo de texto de entrada con el catálogo inicial de asignaturas.
## correlativas.txt:
Archivo de texto de entrada con las relaciones de precedencia (origen;destino).
## alumnos.dat:
Archivo binario para persistir el padrón de estudiantes.
## historial.dat:
Archivo binario para persistir los historiales académicos.


## COMO FUNCIONA:
Cada estudiante cuenta con su legajo y datos personales en el ABB, junto con una lista enlazada de materias rendidas. Las calificaciones se evalúan en escala de 1 a 10:
Nota >= 6  ->  APROBADA (habilita correlativas)
Nota < 6   ->  DESAPROBADA / APLAZO (bloquea correlativas)
Al solicitar inscripción a una materia, el sistema consulta la Matriz de Adyacencia del grafo y verifica en la lista del alumno que todas las materias previas estén aprobadas.
Además, calcula dos indicadores académicos independientes: Promedio General (con aplazos) y Promedio sin aplazos.

## COMPILACION Y EJECUCION: 
Mantener en una misma carpeta los archivos historial.c, TDA_Alumno.h, TDA_MATERIA.h, TDA_PLANESTUDIOS.h, materias.txt y correlativas.txt.
## Compilar el proyecto usando GCC:
gcc historial.c -o sistema_academico
## Ejecutar en Linux o macOS:
./sistema_academico
## Ejecutar en Windows:
sistema_academico.exe
(Finalizar siempre mediante la opción 0. Guardar todo y salir para conservar los datos en disco).


## PERSISTENCIA DE DATOS: 
El sistema implementa un esquema híbrido que combina archivos de texto y archivos binarios según la función de cada conjunto de datos:

Configuración del Plan (.txt): materias.txt y correlativas.txt cargan automáticamente los vértices y aristas del grafo al iniciar.

Padrón e Historiales (.dat): alumnos.dat guarda el ABB en Pre-Orden para conservar su topología, mientras que historial.dat graba las notas en In-Orden y las reconstruye al reiniciar aplicando Corte de Control por legajo.

Certificado Analítico (.txt): Exporta un informe formal legible en analitico_<legajo>.txt.

## MENU DEL SISTEMA:
## ========== SISTEMA DE GESTIÓN ACADÉMICA (SIGA) ==========
1. Registrar nuevo alumno
2. Listar padrón de alumnos ordenado
3. Consultar ficha e historial académico
4. Eliminar alumno por legajo
5. Registrar calificación de materia
6. Validar inscripción a materia (correlatividades)
7. Visualizar Plan de Estudios
8. Visualizar Matriz de Adyacencia
9. Agregar nueva materia o correlatividad
10. Emitir certificado analítico
0. Guardar todo y salir.

## Decisiones de DiseñoABB vs. Vectores o Listas:
Se eligió un ABB para evitar límites de capacidad estática y búsquedas secuenciales, obteniendo el listado ordenado mediante recorrido In-Orden.
## Lista Enlazada vs. Vector fijo: 
Se asigna memoria dinámica exacta según el avance real de cada estudiante.
## Grafo Dirigido vs. Condicionales if:
Evita recompilar el programa ante cambios curriculares, cargando las correlatividades dinámicamente desde archivos.txt].
## Persistencia Híbrida: 
Separa nodos y listas en dos archivos binarios con Corte de Control porque los punteros en memoria pierden validez entre ejecuciones.

## ESTADO DEL PROYECTO:
[x] Gestión de padrón de alumnos (ABB)

[x] Historial dinámico de calificaciones (Listas Enlazadas)

[x] Control automático de correlatividades (Grafo Dirigido)

[x] Cálculo de promedios con y sin aplazos

[x] Persistencia híbrida en .dat y .txt con Corte de Control

[x] Exportación de certificado analítico

[x] Implementación modular con TDAs

## CONTEXTO ACADEMICO:
Este repositorio contiene un proyecto integrador desarrollado por el Grupo 6 para la asignatura
Algoritmos y Estructuras de Datos II de la Licenciatura en Sistemas de Información — Universidad Nacional del Nordeste (UNNE) (2026).

## INTEGRANTES:
Flores, Clara Isabel
Flores, Dana Ivone
Godoy, Luana Vera Astrid
González, Valentina Guadalupe
Barrios, Yaquelin
Denis, Facundo Jose

```text

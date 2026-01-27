# Documentación: Transpilador de Pseudo-lenguaje a Ensamblador

## Estructura de Datos y Variables

El programa utiliza las siguientes estructuras para gestionar la traducción:

* **`vector<string> codigo`**: Almacena secuencialmente las instrucciones generadas.
* **`set<string> variables`**: Repositorio de nombres de variables para evitar duplicados y declararlas al final del archivo.
* **`labelCount`**: Contador entero para la generación de etiquetas únicas (útil para futuras implementaciones de saltos).

---

## Lógica de Procesamiento

La función central `procesarLinea(string linea)` analiza cada instrucción basándose en patrones de texto:

### 1. Entrada de Datos (`LEER`)
Busca la palabra clave al inicio de la línea.
- **Entrada:** `LEER X`
- **Salida ASM:** - `INP` (Solicita valor)
  - `STA X` (Almacena en memoria)

### 2. Salida de Datos (`IMPRIMIR`)
Identifica la solicitud de visualización.
- **Entrada:** `IMPRIMIR X`
- **Salida ASM:**
  - `LDA X` (Carga el valor en el acumulador)
  - `OUT` (Muestra el valor)

### 3. Asignación y Sustracción (`=`, `-`)
Extrae los operandos y el destino mediante subcadenas.
- **Entrada:** `C = A - B`
- **Salida ASM:**
  - `LDA A` (Carga primer operando)
  - `SUB B` (Resta el segundo)
  - `STA C` (Guarda el resultado)

---

## Flujo de Ejecución del Programa

1.  **Lectura:** Abre `entrada.txt` y procesa línea por línea.
2.  **Traducción:** Ignora líneas vacías y traduce las válidas al vector de código.
3.  **Finalización de Programa:** Inserta la instrucción `HLT` (Halt) para detener la ejecución.
4.  **Declaración de Memoria:** Recorre el conjunto de variables y reserva espacio para cada una con `DAT 0`.
5.  **Escritura:** Vuelca todo el contenido en `salida.asm`.

---

## Ejemplo de Transpilación

### Código Fuente (`entrada.txt`)
```text
LEER VAL1
LEER VAL2
RES = VAL1 - VAL2
IMPRIMIR RES
```
### Codigo de salida
```INP
STA VAL1
INP
STA VAL2
LDA VAL1
SUB VAL2
STA RES
LDA RES
OUT
HLT
RES DAT 0
VAL1 DAT 0
VAL2 DAT 0

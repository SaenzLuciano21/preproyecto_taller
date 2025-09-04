# Preproyecto Taller de Diseño de Software

## Integrantes
- Luciano Saenz ([@SaenzLuciano21](https://github.com/SaenzLuciano21))

## Descripción
Este proyecto implementa un compilador básico en C utilizando **Lex** y **Bison**.  
Incluye:
- Analizador léxico
- Parser
- Árbol Sintáctico Abstracto (AST)
- Intérprete de expresiones
- Generador de pseudo-assembly

## Requisitos
Instalar las siguientes dependencias:
- sudo apt install flex bison gcc make

## Compilación
Ejecutar:
flex calc-lexico.l
bison -d calc-sintaxis.y
gcc -o calc lex.yy.c calc-sintaxis.tab.c ast.c symtab.c eval.c codegen.c -lfl

## Ejecución
Ejemplo de uso:
- ./calc entrada

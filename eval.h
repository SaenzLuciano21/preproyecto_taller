#ifndef EVAL_H
#define EVAL_H

#include "ast.h"

typedef enum { VAL_VOID, VAL_INT, VAL_BOOL } ValueType;

typedef struct {
    ValueType type;
    int i; // valor int
    int b; // valor bool (0/1)
} Value;

Value eval_program(ASTNode* root); // ejecuta main; retorna valor (o VOID)

#endif


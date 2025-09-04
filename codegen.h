#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <stdio.h>

void gen_program(ASTNode* root, FILE* out);

#endif


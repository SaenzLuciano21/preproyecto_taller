#include "codegen.h"
#include <stdio.h>
#include <string.h>

static void gen_expr(ASTNode* n, FILE* out) {
    switch (n->type) {
        case NODE_INT:
            fprintf(out, "  PUSH %d\n", n->int_val);
            break;
        case NODE_BOOL:
            fprintf(out, n->bool_val ? "  PUSH_TRUE\n" : "  PUSH_FALSE\n");
            break;
        case NODE_ID:
            fprintf(out, "  LOAD %s\n", n->id);
            break;
        case NODE_BINOP:
            gen_expr(n->binop.left, out);
            gen_expr(n->binop.right, out);
            switch (n->binop.op) {
                case OP_ADD: fprintf(out, "  ADD\n"); break;
                case OP_SUB: fprintf(out, "  SUB\n"); break;
                case OP_MUL: fprintf(out, "  MUL\n"); break;
            }
            break;
        default:
            // padres o nodos que no son expr
            break;
    }
}

static void gen_stmt(ASTNode* s, FILE* out) {
    switch (s->type) {
        case NODE_ASSIGN:
            gen_expr(s->assign.expr, out);
            fprintf(out, "  STORE %s\n", s->assign.id);
            break;
        case NODE_RETURN:
            if (s->ret.expr) {
                gen_expr(s->ret.expr, out);
                fprintf(out, "  RET\n");
            } else {
                fprintf(out, "  RET_VOID\n");
            }
            break;
        default:
            break;
    }
}

static int gen_list(ASTNode* list, FILE* out) {
    if (!list || list->type != NODE_STATEMENTS) return 0;
    int found_ret = 0;
    for (int i=0; i<list->statements.count; ++i) {
        ASTNode* s = list->statements.stmts[i];
        gen_stmt(s, out);
        if (s->type == NODE_RETURN)
            found_ret = 1;
    }
    return found_ret;
}

void gen_program(ASTNode* root, FILE* out) {
    if (!root || root->type != NODE_PROGRAM) return;

    fprintf(out, "MAIN: ; return type = %s\n", root->program.tipo_retorno);

    // Declaraciones: (no hace nada salvo comentar)
    if (root->program.declaraciones && root->program.declaraciones->type == NODE_STATEMENTS) {
        for (int i=0; i<root->program.declaraciones->statements.count; ++i) {
            ASTNode* d = root->program.declaraciones->statements.stmts[i];
            if (d->type == NODE_DECLARATION)
                fprintf(out, "  ; DECL %s %s\n", d->decl.tipo, d->decl.id);
        }
    }

    // Sentencias:
    int has_ret = gen_list(root->program.sentencias, out);

    if (!has_ret) {
        // Si no hubo return explícito, forzamos según tipo
        if (strcmp(root->program.tipo_retorno, "void")==0) {
            fprintf(out, "  RET_VOID\n");
        } else {
            // para int/bool sin return explícito, no deberíamos llegar semánticamente;
            // pero por si acaso:
            fprintf(out, "  ; WARNING: missing return, pushing default 0/false\n");
            fprintf(out, "  PUSH 0\n  RET\n");
        }
    }
    fprintf(out, "HALT\n");
}


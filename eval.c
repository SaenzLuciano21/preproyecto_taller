#include "eval.h"
#include "symtab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// helpers
static void type_error(const char* msg) {
    fprintf(stderr, "Type error: %s\n", msg);
    exit(1);
}
static void undeclared_error(const char* name) {
    fprintf(stderr, "Semantic error: variable '%s' no declarada\n", name);
    exit(1);
}
static void uninit_error(const char* name) {
    fprintf(stderr, "Semantic error: variable '%s' sin inicializar\n", name);
    exit(1);
}

static Value make_int(int x)  { Value v; v.type=VAL_INT;  v.i=x; v.b=0; return v; }
static Value make_bool(int x) { Value v; v.type=VAL_BOOL; v.b=!!x; v.i=0; return v; }
static Value make_void()      { Value v; v.type=VAL_VOID; v.i=v.b=0; return v; }

static Value eval_expr(ASTNode* n) {
    switch (n->type) {
        case NODE_INT:  return make_int(n->int_val);
        case NODE_BOOL: return make_bool(n->bool_val);
        case NODE_ID: {
            VarEntry* e = sym_lookup(n->id);
            if (!e) undeclared_error(n->id);
            if (!e->initialized) uninit_error(n->id);
            if (e->type == VT_INT)  return make_int(e->ival);
            else                    return make_bool(e->bval);
        }
        case NODE_BINOP: {
            Value L = eval_expr(n->binop.left);
            Value R = eval_expr(n->binop.right);
            // Solo +, -, * sobre INT
            if (L.type != VAL_INT || R.type != VAL_INT)
                type_error("operación aritmética sobre no enteros");
            int r = 0;
            switch (n->binop.op) {
                case OP_ADD: r = L.i + R.i; break;
                case OP_SUB: r = L.i - R.i; break;
                case OP_MUL: r = L.i * R.i; break;
            }
            return make_int(r);
        }
        default:
            type_error("expresión inválida");
            return make_void();
    }
}

// ejecuta una sentencia; si encuentra return, devuelve (flag=1, valor)
// si no hay return en esta sentencia, flag=0
typedef struct { int has_ret; Value val; } Ret;

static Ret exec_stmt(ASTNode* s) {
    Ret r = {0, make_void()};
    switch (s->type) {
        case NODE_ASSIGN: {
            VarEntry* e = sym_lookup(s->assign.id);
            if (!e) undeclared_error(s->assign.id);
            Value v = eval_expr(s->assign.expr);
            if (e->type == VT_INT) {
                if (v.type != VAL_INT) type_error("asignación int := no-int");
                sym_set_int(e->name, v.i);
            } else {
                if (v.type != VAL_BOOL) type_error("asignación bool := no-bool");
                sym_set_bool(e->name, v.b);
            }
        } break;

        case NODE_RETURN: {
            if (s->ret.expr) {
                Value v = eval_expr(s->ret.expr);
                r.has_ret = 1;
                r.val = v;
            } else {
                r.has_ret = 1;
                r.val = make_void();
            }
        } break;

        default:
            // declaraciones y otros no llegan acá
            break;
    }
    return r;
}

static Ret exec_stmts(ASTNode* list) {
    Ret r = {0, make_void()};
    if (!list || list->type != NODE_STATEMENTS) return r;
    for (int i=0; i<list->statements.count; ++i) {
        ASTNode* s = list->statements.stmts[i];
        r = exec_stmt(s);
        if (r.has_ret) return r;
    }
    return r;
}

static void declare_list(ASTNode* list) {
    if (!list || list->type != NODE_STATEMENTS) return;
    for (int i=0; i<list->statements.count; ++i) {
        ASTNode* d = list->statements.stmts[i];
        if (d->type != NODE_DECLARATION) continue;
        VarType t = str_to_vartype(d->decl.tipo);
        if (sym_declare(d->decl.id, t) != 0) {
            fprintf(stderr, "Semantic error: redeclaración de '%s'\n", d->decl.id);
            exit(1);
        }
    }
}

Value eval_program(ASTNode* root) {
    if (!root || root->type != NODE_PROGRAM) {
        fprintf(stderr, "No hay programa válido.\n");
        exit(1);
    }
    sym_init();

    // 1) declarar variables
    declare_list(root->program.declaraciones);

    // 2) ejecutar sentencias
    Ret r = exec_stmts(root->program.sentencias);

    // 3) verificar consistencia return vs tipo de main
    const char* tret = root->program.tipo_retorno;
    if (strcmp(tret, "void") == 0) {
        // si hubo return con expresión -> error
        if (r.has_ret && r.val.type != VAL_VOID)
            type_error("main declarado void pero retorna un valor");
        sym_free();
        return make_void();
    } else if (strcmp(tret, "int") == 0) {
        if (!r.has_ret) type_error("main int sin return");
        if (r.val.type != VAL_INT) type_error("main int retorna no-int");
        sym_free();
        return r.val;
    } else { // bool
        if (!r.has_ret) type_error("main bool sin return");
        if (r.val.type != VAL_BOOL) type_error("main bool retorna no-bool");
        sym_free();
        return r.val;
    }
}


#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ASTNode* root = NULL;

ASTNode* create_int_node(int val) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_INT;
    node->int_val = val;
    return node;
}

ASTNode* create_bool_node(int val) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BOOL;
    node->bool_val = val;
    return node;
}

ASTNode* create_id_node(const char* name) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ID;
    node->id = strdup(name);
    return node;
}

ASTNode* create_binop_node(BinOpType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_BINOP;
    node->binop.op = op;
    node->binop.left = left;
    node->binop.right = right;
    return node;
}

ASTNode* create_assign_node(const char* id, ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGN;
    node->assign.id = strdup(id);
    node->assign.expr = expr;
    return node;
}

ASTNode* create_decl_node(const char* tipo, const char* id) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_DECLARATION;
    node->decl.tipo = strdup(tipo);
    node->decl.id = strdup(id);
    return node;
}

ASTNode* create_return_node(ASTNode* expr) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_RETURN;
    node->ret.expr = expr;
    return node;
}

ASTNode* create_statements_node() {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_STATEMENTS;
    node->statements.stmts = NULL;
    node->statements.count = 0;
    return node;
}

void append_statement(ASTNode* stmts, ASTNode* stmt) {
    stmts->statements.stmts = realloc(stmts->statements.stmts, sizeof(ASTNode*) * (stmts->statements.count + 1));
    stmts->statements.stmts[stmts->statements.count++] = stmt;
}

ASTNode* create_program_node(const char* tipo, ASTNode* decls, ASTNode* stmts) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->program.tipo_retorno = strdup(tipo);
    node->program.declaraciones = decls;
    node->program.sentencias = stmts;
    return node;
}

ASTNode* create_empty_node() {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_EMPTY;
    return node;
}

void print_indent(int indent) {
    for (int i = 0; i < indent; ++i)
        printf("  ");
}

void print_ast(ASTNode* node, int indent) {
    if (!node) return;

    print_indent(indent);

    switch (node->type) {
        case NODE_INT:
            printf("INT: %d\n", node->int_val);
            break;
        case NODE_BOOL:
            printf("BOOL: %s\n", node->bool_val ? "true" : "false");
            break;
        case NODE_ID:
            printf("ID: %s\n", node->id);
            break;
        case NODE_BINOP:
            printf("BINOP: ");
            switch (node->binop.op) {
                case OP_ADD: printf("+\n"); break;
                case OP_SUB: printf("-\n"); break;
                case OP_MUL: printf("*\n"); break;
            }
            print_ast(node->binop.left, indent + 1);
            print_ast(node->binop.right, indent + 1);
            break;
        case NODE_ASSIGN:
            printf("ASSIGN: %s = \n", node->assign.id);
            print_ast(node->assign.expr, indent + 1);
            break;
        case NODE_DECLARATION:
            printf("DECL: %s %s\n", node->decl.tipo, node->decl.id);
            break;
        case NODE_RETURN:
            printf("RETURN:\n");
            if (node->ret.expr)
                print_ast(node->ret.expr, indent + 1);
            break;
        case NODE_STATEMENTS:
            printf("STATEMENTS:\n");
            for (int i = 0; i < node->statements.count; ++i)
                print_ast(node->statements.stmts[i], indent + 1);
            break;
        case NODE_PROGRAM:
            printf("PROGRAM: return type = %s\n", node->program.tipo_retorno);
            print_ast(node->program.declaraciones, indent + 1);
            print_ast(node->program.sentencias, indent + 1);
            break;
        case NODE_EMPTY:
            printf("EMPTY\n");
            break;
    }
}


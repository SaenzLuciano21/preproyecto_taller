#ifndef AST_H
#define AST_H

typedef enum {
    NODE_INT,
    NODE_BOOL,
    NODE_ID,
    NODE_BINOP,
    NODE_ASSIGN,
    NODE_DECLARATION,
    NODE_RETURN,
    NODE_STATEMENTS,
    NODE_PROGRAM,
    NODE_EMPTY
} NodeType;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL
} BinOpType;

typedef struct ASTNode {
    NodeType type;

    union {
        int int_val;           // Para enteros
        int bool_val;          // Para true (1) / false (0)
        char* id;              // Para variables (ID)

        struct {
            BinOpType op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binop;

        struct {
            char* id;
            struct ASTNode* expr;
        } assign;

        struct {
            char* tipo;
            char* id;
        } decl;

        struct {
            struct ASTNode* expr; // puede ser NULL para "return;"
        } ret;

        struct {
            struct ASTNode** stmts;
            int count;
        } statements;

        struct {
            char* tipo_retorno;
            struct ASTNode* declaraciones;
            struct ASTNode* sentencias;
        } program;
    };
} ASTNode;

extern ASTNode* root;

// Funciones de creación de nodos
ASTNode* create_int_node(int val);
ASTNode* create_bool_node(int val);
ASTNode* create_id_node(const char* name);
ASTNode* create_binop_node(BinOpType op, ASTNode* left, ASTNode* right);
ASTNode* create_assign_node(const char* id, ASTNode* expr);
ASTNode* create_decl_node(const char* tipo, const char* id);
ASTNode* create_return_node(ASTNode* expr);
ASTNode* create_statements_node();
void      append_statement(ASTNode* stmts, ASTNode* stmt);
ASTNode* create_program_node(const char* tipo, ASTNode* decls, ASTNode* stmts);
ASTNode* create_empty_node();

void print_ast(ASTNode* node, int indent);

#endif


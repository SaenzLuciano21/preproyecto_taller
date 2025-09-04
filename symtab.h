#ifndef SYMTAB_H
#define SYMTAB_H

typedef enum { VT_INT, VT_BOOL } VarType;

typedef struct VarEntry {
    char* name;
    VarType type;
    int initialized;
    int ival;   // valor int cuando type == VT_INT
    int bval;   // valor bool cuando type == VT_BOOL (0/1)
    struct VarEntry* next;
} VarEntry;

void  sym_init(void);
void  sym_free(void);

int   sym_declare(const char* name, VarType type); // 0 ok, -1 redeclaración
VarEntry* sym_lookup(const char* name);            // NULL si no existe

// set/get con chequeos de tipo/decl
int   sym_set_int (const char* name, int v);       // 0 ok, -1 error
int   sym_set_bool(const char* name, int v);       // 0 ok, -1 error
int   sym_get_int (const char* name, int* out);    // 0 ok, -1 error
int   sym_get_bool(const char* name, int* out);    // 0 ok, -1 error

// helpers
static inline VarType str_to_vartype(const char* s) {
    return (s && s[0]=='i') ? VT_INT : VT_BOOL; // "int"->VT_INT, "bool"->VT_BOOL
}

#endif


#include "symtab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static VarEntry* head = NULL;

void sym_init(void) { head = NULL; }

void sym_free(void) {
    VarEntry* it = head;
    while (it) {
        VarEntry* nx = it->next;
        free(it->name);
        free(it);
        it = nx;
    }
    head = NULL;
}

static VarEntry* create_entry(const char* name, VarType type) {
    VarEntry* e = (VarEntry*)malloc(sizeof(VarEntry));
    e->name = strdup(name);
    e->type = type;
    e->initialized = 0;
    e->ival = 0;
    e->bval = 0;
    e->next = NULL;
    return e;
}

VarEntry* sym_lookup(const char* name) {
    for (VarEntry* it = head; it; it = it->next)
        if (strcmp(it->name, name) == 0) return it;
    return NULL;
}

int sym_declare(const char* name, VarType type) {
    if (sym_lookup(name)) return -1; // redeclaración
    VarEntry* e = create_entry(name, type);
    e->next = head;
    head = e;
    return 0;
}

int sym_set_int(const char* name, int v) {
    VarEntry* e = sym_lookup(name);
    if (!e || e->type != VT_INT) return -1;
    e->ival = v;
    e->initialized = 1;
    return 0;
}

int sym_set_bool(const char* name, int v) {
    VarEntry* e = sym_lookup(name);
    if (!e || e->type != VT_BOOL) return -1;
    e->bval = !!v;
    e->initialized = 1;
    return 0;
}

int sym_get_int(const char* name, int* out) {
    VarEntry* e = sym_lookup(name);
    if (!e || e->type != VT_INT || !e->initialized) return -1;
    *out = e->ival;
    return 0;
}

int sym_get_bool(const char* name, int* out) {
    VarEntry* e = sym_lookup(name);
    if (!e || e->type != VT_BOOL || !e->initialized) return -1;
    *out = e->bval;
    return 0;
}


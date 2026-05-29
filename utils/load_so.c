#include "load_so.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef const char* (*FPTR)(char*, void*, void*, void*);
FPTR fptr = NULL;

typedef const char* (*FPTRV2)(int, void*, void*, void*);
FPTRV2 fptrv2 = NULL;

int has_dlopen = 0;

void setup(char* dl_path) {

    void* handle;
    char full_path[256];

    snprintf(full_path, sizeof(full_path), "%s/%s", dl_path, "libstackplz.so");
    handle = dlopen(full_path, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "[eDBG] dlopen failed: %s (path: %s)\n", dlerror(), full_path);
        return;
    }
    fptr = (FPTR)dlsym(handle, "StackPlz");
    fptrv2 = (FPTRV2)dlsym(handle, "StackPlzV2");

    if (!fptr && !fptrv2) {
        fprintf(stderr, "[eDBG] dlsym failed: %s\n", dlerror());
        return;
    }

    if (has_dlopen == 0) {
        has_dlopen = 1;
    }
}

const char* get_stack(char* dl_path, char* map_buffer, void* opt, void* regs_buf, void* stack_buf)
{
    if (has_dlopen == 0) {
        setup(dl_path);
    }

    if (!fptr) {
        return "[eDBG] error: StackPlz symbol not loaded";
    }

    return (*fptr)(map_buffer, opt, regs_buf, stack_buf);
}

const char* get_stackv2(char* dl_path, int pid, void* opt, void* regs_buf, void* stack_buf)
{
    if (has_dlopen == 0) {
        setup(dl_path);
    }

    if (!fptrv2) {
        return "[eDBG] error: StackPlzV2 symbol not loaded";
    }

    return (*fptrv2)(pid, opt, regs_buf, stack_buf);
}
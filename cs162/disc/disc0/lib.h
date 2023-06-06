typedef struct helper_args {
    #ifdef ABC
        char* aux;
    #endif
    char* string;
    char target;
} helper_args_t;
char* helper_func(helper_args_t* args);

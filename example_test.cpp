#include <stdio.h>
#include <stdlib.h>
#include "co_routine.h"

void* f(void* args) {
    while (1) {
        printf("f\n");
        co_yield_ct();
    }
    return NULL;
}
void* g(void* args) {
    while (1) {
        printf("g\n");
        co_yield_ct();
    }
    return NULL;
}
int main() {
    stCoRoutine_t* co_f;
    stCoRoutine_t* co_g;
	co_create(&co_f, NULL, f, NULL); 
	co_create(&co_g, NULL, g, NULL); 
    for (int i = 0; i < 10; ++ i)
    {
        co_resume(co_f);
        co_resume(co_g);
    }
    return 0;
}

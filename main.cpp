#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "libreria.h"


int main()
{
    FILE *f= fopen("network_structure.dat", "rb");
    if( f == NULL)
    {
        printf("Error al abrir archivo\n");
        return -1;
    }
int C = cant_disp(f);  
struct registro *R= new struct registro[C];


secuencia(f, R);
equipos(f, R);
delete []R;
fclose(f);


return 0;

}





 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "libreria.h"

/**
 * @file Recuperatorio 2024
 * @author Sosa Schmechel Janet
 * @brief Recuperatorio 2024
 * @version 0.1
 * @date 2024-06-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */


/**
 * @brief esta funcion cuenta cuantos dispositivos hay en el archivo
 * 
 *
 * Restablece el puntero del archivo al inicio una vez leidos los 16 bits
 * 
 * @param f  manipulador del archivo
 * @return  El número de dispositivos en el archivo.
 */
int cant_disp(FILE *f)
{
    int c=0;
    uint16_t header;
    uint8_t LLDC;
    uint16_t aux;
    uint8_t aux1;
    while (fread(&header, sizeof(uint16_t), 1, f))
    {
        aux = header << 10;
        aux1= aux >> 10;
        LLDC = aux1 & 0xFFFF;
        fseek(f, LLDC * sizeof(uint8_t), SEEK_CUR);
        c=c+1;
    }
    rewind(f);
    return c;
}

/**
 * @brief extrae la porcion de bits que se indique
 * 
 * @param data es el "lugar" de donde se quieren extraer los bits
 * @param startBit indica DESDE donde quiero extraer bits
 * @param endBit indica HASTA donde quiero extraer bits
 */
uint8_t extract_segment(uint8_t data, uint8_t startBit, uint8_t endBit) 
{
    if (startBit > endBit || startBit >= 32 || endBit >= 32) {
        printf("Rango de bits invalido\n");
        return 0;
    }
    uint8_t longi = endBit - startBit + 1; //longitud el segmento a extraer
    uint8_t mask = (1 << longi) - 1; //muevo el 1 longi veces y resto 1 para que todos los bits antes del 1 (Lsb) se conviertan en 1                          
    data = data >> startBit; //shifteo data hasta start que es donde quiero empezar a extraer
    uint8_t segment = data & mask; //segmento extraído      
    return segment;
}

/**
 * @brief Lee los IDs y la cantidad de dispositivos.
 * 
 * 
 * utilizo la funcion extract_segment para extraer los bits con la informacion necesaria.
 * 
 * @param f  manipulador del archivo
 * @param R estructura registro que almacena los datos
 */
void lectura(FILE *f, struct registro *R)
{ 
    int cont=0;
    uint16_t header1;
    uint16_t ID;
    uint8_t LLDC;
    uint16_t header2;
    uint8_t NH;
    uint8_t NL;
    uint8_t info;
    uint16_t header3;
    uint16_t ULD_ID;
   
    
    while (fread(&header1, sizeof(uint16_t), 1,f))
    {
        ID = extract_segment(header1 , 15, 6);
        R[cont].ID = ID;
        LLDC = extract_segment(header1 , 5, 0);
        R[cont].LowerLD_C = LLDC;
    }
    while (fread(&header2, sizeof(uint16_t), 1,f))
    {
        NH = extract_segment(header2, 12, 12);
        R[cont].NH = NH;
        NL = extract_segment(header2, 3, 3);
        R[cont].NL = NL;
        info = extract_segment(header2, 11, 4);
        R[cont].info = info;
    }
    while (fread(&header3, sizeof(uint16_t), 1,f))
    {
        ULD_ID = extract_segment(header3, 12, 3);
        R[cont].ULD_ID = ULD_ID;
    }
    cont++;
}
/**
 * @brief busca un ID 
 * 
 * Esta función busca un id que se pasa como parametro 
 * Si no se encuentra el id, retorno 0
 * 
 * @param R estructura en la que hay que buscar el id
 * @param size tamaño de R
 * @param ID id a buscar en R
 * @return  posicion i en donde se encuentra el id
 */
int buscar(struct registro *R, int size, uint64_t ID) 
{ 
    for (int i = 0; i < size; ++i) 
    {
        if (R[i].ID == ID) 
        {
            return i;  
        }
    }
    return 0; // si no se encuentra el id retorno 0;
}
/**
 * @brief se verifica si el id ingresado existe en los registros
 * 
 * @param f manipulador del archivo
 * @param R puntero a los registros cargados en la estructura registro
 * @return uint16_t el id existente en R
 */
uint16_t buscar_id(FILE *f, struct registro *R) 
{  
    int size = cant_disp(f);
    uint16_t ID;
    int aux;
    do {
        printf("ingrese id:\n ");
        scanf("%d", &ID);
        aux = buscar(R, size, ID);
        if(aux==-1)
        {
            printf("ingrese id valido\n");
        }
    } while (aux == -1);
    return ID;
}
/**
 * @brief procedimiento que encuentra la secuencia de conexiones de un ID ingresado por el usuario (exceptuando el id 1023)
 * 
 * @param f manipulador del archivo
 * @param R puntero a los registros cargados en la estructura registro
 */
void secuencia(FILE *f, struct registro *R)
 {
    int size = cant_disp(f); 
    int posicion = buscar(R, size, ID); 
    int v = 0; 

    while (R[posicion].ULD_ID != 1023) 
    {  
        posicion = buscar(R, size, R[posicion].ULD_ID);
        if (posicion == -1) 
        {
            return 0;
        }
        v++;
    }
  
    int *v1 = new int[v + 1]; 
    posicion = buscar(R, size, ID);
    for (int i = 0; i <= v; i++) {
        v1[i] = R[posicion].ID;
        posicion = buscar(R, size, R[posicion].ULD_ID);
    }
    printf("\n\n");
    for (int i = v; i >= 0; i--) {
        printf("ID %d", v1[i]);
        if (i > 0) 
        {
            printf(" -> ");
        }
    }
    printf("\n\n");
}


/**
 * @brief este procedimiento incrementa los contadores de cada equipo, dependiendo los valores de los nibbles
 * 
 * @param f manipulador de archivo
 * @param R Puntero a los registros cargados en la estructura registro
 */
void equipos(FILE *f, struct registro *R)
{
    int cant= cant_disp(f)
    uint8_t NL;
    uint8_t NH;
    uint8_t type
    int count_cpu , count_tempsensor , count_valve , count_concentrator;


for(int i=0; i<cant; i++)
{
 NL= R[cant].NL;
 NH= R[cant].NH;
 type=NH + NL;

 if (NH == 1 && NL==0)
 {
    count_cpu = count_cpu + 1;
 }
 if (NH==0 && NL==1)
 {   
    count_tempsensor = count_tempsensor + 1;
 }
 if (NH == 1 && NL==0)
 {    
     count_valve = count_valve + 1;
 }
 if (NH==1 && NL==1)
 {
     count_concentrator = count_concentrator + 1;
 }

}

    printf("en la red hay: \n");
    printf("CPU = %d\n",count_cpu);
    printf("CONCENTRATOR = %d\n",count_concentrator);
    printf("TEMP SENSOR = %d\n",count_tempsensor);
    printf("\n VALVE = %d\n",count_valve);


}


       
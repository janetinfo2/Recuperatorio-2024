#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct Sensor
{
    uint8_t tipo_sensor;     
};

struct Actuador
{
    uint8_t tipo_actuador;    
};

struct DeviceType
{
    uint8_t cpu;  
    uint8_t concentrador;    
    struct Sensor sensor;
    struct Actuator actuador;
};

struct registro
{
    uint16_t ID;  
    uint8_t LLDC;
    uint8_t NH;
    uint8_t NL;
    uint8_t info;
    uint16_t ULD_ID;
};

int cant_disp(FILE *f);
uint8_t extract_segment(uint8_t data, uint8_t startBit, uint8_t endBit);
void lectura(FILE *f, struct registro *R);
int buscar(struct registro *R, int size, uint64_t ID);
uint16_t buscar_id(FILE *f, struct registro *R);
void secuencia(FILE *f, struct registro *R);
void equipos(FILE *f, struct registro *R);

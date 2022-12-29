#ifndef _PAQUETE_H__
#define _PAQUETE_H__ 1

struct Conexion{ // type 0
    int id;
    char name[256];
};

struct Movimiento{
    float x;
    float y;
};

struct Accion{ // type 1
    int id;
    Movimiento movimiento;
    bool bomba;
};

struct Paquete{
    int type;
    union{
        struct Conexion con; // 0
        struct Accion action; // 1
    };
};

#endif

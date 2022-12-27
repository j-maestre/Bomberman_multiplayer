

#ifndef _SCENARIO_H__
#define _SCENARIO_H__ 1

#include "casilla.h"

class Scenario{
    public:
    Scenario();
    Scenario(const Scenario&);
    ~Scenario();
    void Render();

    private:
    Casilla casillas[50];
    Casilla prueba;


};

#endif
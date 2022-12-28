

#ifndef _SCENARIO_H__
#define _SCENARIO_H__ 1

#include "oxml/Vec4.h"
#include "casilla.h"


class Scenario{
    public:
    Scenario();
    Scenario(const Scenario&);
    ~Scenario();
    void Render();
    
    //Width y heich de cada casilla
    float width_;
    float height_;
    static const int filas_ = 18;
    static const int columnas_ = 31;
    Casilla casillas_[filas_][columnas_];

    private:
    Casilla prueba;


};

#endif
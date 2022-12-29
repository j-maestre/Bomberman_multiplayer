#ifndef _CLIENTE_H__
#define _CLIENTE_H__ 1

#include <winsock.h>
#include "paquete.h"


class Cliente{
    public:

    ~Cliente();
    static Cliente& Instance(){
        if(Cliente::instance_ == nullptr){
            Cliente::instance_ = new Cliente();
        }
        return *Cliente::instance_;
    }


    void Conect();
    Movimiento Move(float x, float y, bool bomb);
    void UpdateStats(Accion action);
    char my_name_[256];
    int my_id_;

    private:
    
    Cliente();
    Cliente(const Cliente&){};
    static Cliente *instance_;

    WSADATA wsa_;
    SOCKET sock_;
    sockaddr_in ip_,ips_[4];
    int ip_size_;
    char nombre_[256];
    Paquete paq_;


    void ResetPaq();


};


#endif
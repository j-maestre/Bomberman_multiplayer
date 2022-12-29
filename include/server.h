#ifndef _SERVER_H__
#define _SERVER_H__ 1

#include <winsock.h>
#include "paquete.h"


class Server{
    public:

    ~Server();
    static Server& Instance(){
        if(Server::instance_ == nullptr){
            Server::instance_ = new Server();
        }
        return *Server::instance_;
    }


    void Deploy();
    void Listen();
    static const int max_clients_ = 4;
    char clientes_names_[max_clients_][256];

    private:
    Server();
    Server(const Server&){};
    static Server *instance_;

    void ReadPaquete(Paquete paq, sockaddr_in ip);

    WSADATA wsa_;
    SOCKET sock_;
    sockaddr_in ip_,ips_[4];
    
    int total_clients_;

    int ip_size_;
    timeval time_;
    fd_set readfs_;

    Paquete paq_;


};


#endif
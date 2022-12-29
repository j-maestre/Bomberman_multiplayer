#include "server.h"
#include <winsock.h>
#include <stdio.h>
#include "gameManager.h"
//#include "paquete.h"


Server* Server::instance_ = nullptr;
//recuerda cuando hagas una variable estatica en una classe tyenes que ponerla en el .cc
//sintaxi: tipo NombreDeLaClase::NombreDeLaVariable = valor;

Server::Server(){
    ip_size_ = sizeof(ip_);
    time_.tv_sec = 0;
    time_.tv_usec = 100000;
    total_clients_ = 0;
    memset(&paq_,0,sizeof(paq_));

}

Server::~Server(){}

void Server::Deploy(){
    
    if(WSAStartup(MAKEWORD(2,2),&wsa_)){
        printf("Error iniciando servidor\n");
    }else{
        int nobloqueado = 1;
        ip_.sin_family = AF_INET;
        ip_.sin_addr.s_addr = inet_addr("0.0.0.0"); //Localhost
        ip_.sin_port = htons(8888);
        sock_ = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        bind(sock_,(SOCKADDR*)&ip_,ip_size_);

        printf("\n*** Servidor iniciado correctamente ***\n");
    }
}

void Server::ReadPaquete(Paquete paq, sockaddr_in ip){
    switch(paq.type){
        case 0:
            //Paquete tipo conexion
            printf("Conectado %s:%d -> %s",inet_ntoa(ip.sin_addr),ntohs(ip.sin_port),paq.con.name);
            ips_[total_clients_] = ip; //Guardamos la ip del cliente que se acaba de conectar
            strcpy(clientes_names_[total_clients_],paq.con.name); //Guardamos el nombre del cliente
            paq.con.id = total_clients_;

            //Creamos un nuevo jugador con la id actual
            GameManager::Instance().AddPlayer(total_clients_); //Añadimos nuevo jugador

            //Reenviamos la id del cliente asignada
            sendto(sock_,(char*)&paq,sizeof(paq),0,(SOCKADDR*)&ip,ip_size_);


            //Llega el primer cliente(nada)
            //Llega el segundo:
            /*
                - Le envio a todo el mundo todas las ids que tengo
                - Los clientes comprueban todas las ids y añaden a los jugadores que no sean de su misma id
            
            */

            //Avisamos a los demas jugadores de que hay un nuevo jugador y tienen que pintarlo
            for(int i = 0; i <= total_clients_; i++){
                printf("*** AVISO A NAVEGANTES *** ");
                printf("Avisando a %s:%d\n",clientes_names_[i],i);
                sendto(sock_,(char*)&paq,sizeof(paq),0,(SOCKADDR*)&ips_[i],ip_size_);
            } 

            total_clients_++;
        break;
        case 1:
            //Paquete tipo accion
            //printf("Paquete de accion\n");
            printf("El jugador con id %d:%s quiere moverse en X[%f] Y[%f]\n",paq.action.id,clientes_names_[paq.action.id],paq.action.movimiento.x,paq.action.movimiento.y);
            Movimiento new_pos = GameManager::Instance().Move(paq.action.id,paq.action.movimiento);
            paq.action.movimiento = new_pos;

            //Comprobamos si planta bomba
            if(paq.action.bomba){
                GameManager::Instance().PlantBomb(new_pos.x,new_pos.y);
            }
            //Devuelvo la nueva posicion cambiada
            sendto(sock_,(char*)&paq,sizeof(paq),0,(SOCKADDR*)&ip,ip_size_);
        break;
    }

}

void Server::Listen(){
    //printf("Escuchando..\n");
    sockaddr_in ip_tmp;
    FD_ZERO(&readfs_);
    FD_SET(sock_,&readfs_);
    //char buffer[256];

    int respuesta = select(0,&readfs_,NULL,NULL,&time_);
    if(respuesta == 0){
        //printf("Timeout...\n");
    }else{
        //Hay algo
        memset(&paq_,0,sizeof(paq_));
        if(FD_ISSET(sock_,&readfs_)){
            int bytes = recvfrom(sock_,(char*)&paq_,sizeof(paq_),0,(SOCKADDR*)&ip_tmp,&ip_size_);
            if(bytes > 0){
                ReadPaquete(paq_,ip_tmp);
            }
            //printf("escuchao\n");
        }
    }
    
}

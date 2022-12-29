

#include "cliente.h"
#include <winsock.h>
#include <stdio.h>
#include "gameManager.h"


Cliente* Cliente::instance_ = nullptr;

Cliente::Cliente(){
    ip_size_ = sizeof(ip_);
    memset(&paq_,0,sizeof(paq_));
    memset(my_name_,'\0',sizeof(my_name_));
    my_id_ = -1;

}

Cliente::~Cliente(){}

void Cliente::ResetPaq(){
    memset(&paq_,0,sizeof(paq_));
}

void Cliente::Conect(){
    
    if(WSAStartup(MAKEWORD(2,2),&wsa_)){
        printf("Error iniciando servidor\n");
    }else{
        int activo = 1;
        ip_.sin_family = AF_INET;
        ip_.sin_addr.s_addr = inet_addr("255.255.255.255"); // Broadcast
        ip_.sin_port = htons(8888);
        sock_ = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        if(setsockopt(sock_,SOL_SOCKET,SO_BROADCAST,(char*)&activo,sizeof(activo)) != SOCKET_ERROR){
          printf("*** Cliente Broadcast configurado con exito ***\n");
        }

        //Pedir nombre y conectar al servidor
        ResetPaq();
        printf("Introduzca su nombre: ");
        fgets(paq_.con.name,sizeof(paq_.con.name),stdin);
        paq_.con.name[strlen(paq_.con.name)-1] = '\0';
        strcpy(my_name_,paq_.con.name);
        paq_.type = 0;
        sendto(sock_,(char*)&paq_,sizeof(paq_),0,(SOCKADDR*)&ip_,ip_size_);

        //Recibimos la ID de cliente
        int bytes = recvfrom(sock_,(char*)&paq_,sizeof(paq_),0,(SOCKADDR*)&ip_,&ip_size_);
        if(bytes > 0){
            //Id recibida
            printf("ID asignada [%d]\n",paq_.con.id);
            my_id_ = paq_.con.id;
            GameManager::Instance().AddPlayer(my_id_);
        }


    }
}

void Cliente::UpdateStats(Accion action){

}

Movimiento Cliente::Move(float x, float y, bool bomb){
    ResetPaq();
    Movimiento mov = {x,y};
    paq_.type = 1;
    paq_.action.id = my_id_;
    paq_.action.movimiento = mov;
    paq_.action.bomba = bomb;

    printf("C mueve! %f %f\n",paq_.action.movimiento.x,paq_.action.movimiento.y);

    //Enviamos al servidor a donde me quiero mover
    sendto(sock_,(char*)&paq_,sizeof(paq_),0,(SOCKADDR*)&ip_,ip_size_);

    //El servidor me devuelve mi posicion actualizada
    ResetPaq();
    recvfrom(sock_,(char*)&paq_,sizeof(paq_),0,(SOCKADDR*)&ip_,&ip_size_);

    //Devuelvo la posicion actualizada
    return paq_.action.movimiento;
}

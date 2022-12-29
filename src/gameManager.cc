#include "gameManager.h"
#include "scenario.h"
#include <stdio.h>
#include "oxml/Vec4.h"
#include "server.h"
#include "cliente.h"
#include <winsock.h>
//#include "player.h"

GameManager* GameManager::instance_ = nullptr;

GameManager::GameManager(){
    currentScenario_ = 0;
    total_players_ = 0;
    is_server_ = false;
    InitScenarios();
    ip_size_ = sizeof(ip_);
    time_.tv_sec = 0;
    time_.tv_usec = 100000;
    printf("Constructor GameManager\n");
}

GameManager::~GameManager(){
  
}


void GameManager::InitScenario0(){

    //10 filas y 15 coumnas
    
    for (int i = 0; i < 18; i++){
        for (int j = 0; j < 31; j++){
            scenarios_[0].casillas_[i][j].color = oxml::Vec4(200.0f,200.0f,200.0f,255.0f);

        }
        
    }
    
    printf("*** Scenario 0 iniciado correctamente ***\n");

}
void GameManager::InitScenarios(){
    printf("Iniciando Scenarios...\n");
    InitScenario0();
    printf("Todos los scenarios inciados correctamente\n");
}

void GameManager::DrawScenario(){
    scenarios_[currentScenario_].Render();

}

void GameManager::DrawPlayer(){
    for (int i = 0; i < total_players_; i++){
        //Pasar el name que tiene que dibujar
        //El game manager deberia tener guardado todos los nombres de los jugadores
        //Y tiene que pasarlos todos independientemente si es servidor o cliente

        if(is_server_){
            players_[i].DrawPlayer("hola");
        }

        //Me pinto a mi mismo
        if(players_[i].id_ == Cliente::Instance().my_id_){
            players_[i].DrawPlayer("hola");
        }else{
            //Pintar los demas jugadores
            players_[i].DrawOtherPlayers(players_[i].transform_.x,players_[i].transform_.y);
        }
        /*if(is_server_){
            players_[i].DrawPlayer(Server::Instance().clientes_names_[i]);
        }else{
            players_[i].DrawPlayer(Cliente::Instance().my_name_);
        }*/
    }
    
}

bool GameManager::MovePlayer(){
    bool moved = false;

    for (int i = 0; i < total_players_; i++){

        //Si estoy activo y soy yo, me muevo
        if(players_[i].active_ && players_[i].id_ == Cliente::Instance().my_id_){
           //printf("Active ");
            players_[i].MovePlayer();
        }
    }
    //printf("\n");

    return moved;
}

Movimiento GameManager::Move(int id, Movimiento mov){
    players_[id].transform_.x += mov.x;
    players_[id].transform_.y += mov.y;
    Movimiento new_pos = {players_[id].transform_.x, players_[id].transform_.y};
    return new_pos;
}

void GameManager::PlantBomb(float x, float y){
    printf("Bomba en %f:%f -> %d:%d\n",x,y,(int) (x/45.0f),(int) (y/45.0f));
    scenarios_[0].casillas_[(int) (y/45.0f)][(int) (x/45.0f)].color = oxml::Vec4(255,0,0,255);
}

void GameManager::AddPlayer(int id){
    players_[total_players_].active_ = true;
    players_[total_players_].id_ = id;
    players_[total_players_].transform_.x = 0.0f;
    players_[total_players_].transform_.y = 0.0f;
    total_players_++;
    printf("--- Player Added [%d]---\n",total_players_);
}


void GameManager::RecivePlayer(){
    sockaddr_in ip_tmp;
    Paquete paq;
    FD_ZERO(&readfs_);
    FD_SET(sock_,&readfs_);

    printf("Mirando a ver si entra alguien mas...\n");
    int respuesta = select(0,&readfs_,NULL,NULL,&time_);
    if(respuesta == 0){
        //printf("Timeout...\n");
    }else{
        //Hay algo
        memset(&paq,0,sizeof(paq));
        if(FD_ISSET(sock_,&readfs_)){
            int bytes = recvfrom(sock_,(char*)&paq,sizeof(paq),0,(SOCKADDR*)&ip_tmp,&ip_size_);
            if(bytes > 0){
                //ReadPaquete(paq,ip_tmp);
                printf("Llega paquete\n");
                if(paq.type == 2){
                    printf("Paquete tipo nuevo player\n");
                    //Soy cliente y el server me dice que hay un nuevo player
                    if(paq.newP.id != Cliente::Instance().my_id_){
                        //eeee nuevo player
                        printf("yeeeee nuevo player con id->%d\n",paq.newP.id);
                        AddPlayer(paq.newP.id);
                    }
                }
            }
            //printf("escuchao\n");
        }
    }

}


//Author1 Sahil Sangani
//Author2 Vishwarajsinh Vala  
//server
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

static int gameId=1;
void servicePlayers(int, int, int);

int main(int argc, char *argv[]){

    srand(time(NULL));
    int sock,port,remoteConn,remoteConn1, remoteConn2;
    int status;
   
    struct sockaddr_in servConfig;
    char message[255];  
    
    if(argc != 2){
        printf("Command signature: %s Port Number\n", argv[0]);
    exit(0);
    }
    
    //create a soccket for connection
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Socket can not be created\n");
    exit(1);
     }
     sock = socket(AF_INET, SOCK_STREAM, 0);
     servConfig.sin_family = AF_INET;
     servConfig.sin_addr.s_addr = htonl(INADDR_ANY);
     sscanf(argv[1],"%d",&port);
     servConfig.sin_port = htons((uint16_t)port);
     
     //bind the socket
    bind(sock,(struct sockaddr *)&servConfig,sizeof(servConfig));   
     
     //listen to new connections
    listen(sock,10);

    int playerNo=0;
     while(1)
     {  
        
        printf("Waiting for clients..\n");
         
         //accept connection
        remoteConn = accept(sock,NULL,NULL); //returns a file descriptor 
        // sleep(1);
        // remoteConn = accept(sock,struct sockaddr *addr,socklen_t *addrlen);
        // remoteConn = accept(sock,(struct sockaddr *)NULL,NULL);
        // printf("%d\n", remoteConn);
        // keep track of number of clients
        playerNo++;

        if(playerNo == 2){
            remoteConn2 = remoteConn;
            // printf("remoteConn2: %d\n", remoteConn2);

            if(!fork()){
             servicePlayers(remoteConn1, remoteConn2, gameId);
            }
            gameId++;
            playerNo = 0;
            // close(remoteConn1);
            // close(remoteConn2);
         }
         else if(playerNo == 1){
            // write(remoteConn1,"Waiting for player2\n",sizeof("Waiting for player2\n"));
            remoteConn1 = remoteConn;

            // printf("remoteConn1: %d\n", remoteConn1);
            printf("Waiting for 2nd player..!!!\n");
            // write(remoteConn1,"Waiting for 2nd player..!!!\n",sizeof("Waiting for 2nd player..!!!\n"));
         }
         else{
            printf("Something went wrong...!!\n");
         }

        // printf("========================\n");
        // close(remoteConn);
        waitpid(0,&status,WNOHANG); //returnd child process status immediately - 0 checks for group id
    }    
}    

void servicePlayers(int remoteConn1, int remoteConn2, int gameId){
    

    int dice=0,n=10;
    int status;
    char str[6];
    int player1 = 1, player2 = 2;

    printf("========================\n");
    printf("GameID: %d\n", gameId);
    printf("Got Players: %d  %d \n", player1, player2);
    printf("========================\n");
    //Fields in data array
    //0 - First element tracks the player1 dice value
    //1 - Second element tracks the player2 dice value
    //2 - Third element tracks the player1 score
    //3 - Fourth element tracks the player2 score
    //4 - Fifth element specifies the number of turns for player1
    //5 - Sixth element specifies the number of turns for player2 // status
    
    int data[6]={0,0,0,0,1,1};

    //send directive message to client
    write(remoteConn1,"Hello Player 1 (TITI)\n",sizeof("Hello Player 1 (TITI)\n"));
    
    // sleep(1); 
    write(remoteConn2,"Hello Player 2 (TOTO)\n",sizeof("Hello Player 2 (TOTO)\n"));

    write(remoteConn1,&player1,sizeof(player1));
    sleep(1);
    write(remoteConn2,&player2,sizeof(player2));

    sleep(1);
    while(1){

        // sleep(1);

        if(data[2] + data[3] < 100){

            // For player 1
            if(data[4]==1){
                // sleep(1);
                write(remoteConn1,"You can now play.",sizeof("You can now play."));
                sleep(1);
                write(remoteConn1,&data,sizeof(data));
                sleep(1);
                read(remoteConn1, &data, sizeof(data));
                // printf("Player 1 score: %d\n", data[2]);
              
            } 

            if(data[4]==0){
                // printf("data[4]= 0 condition\n");
                // sleep(1);
                write(remoteConn2,"Player 1 Exited.\nYou won the game.",sizeof("Player 1 Exited.\nYou won the game."));
                printf("========================\n");
                printf("GameID: %d\n", gameId);
                printf("Game over: Player 1 Quited...!!!\nPlayer 2 Won....\n");
                printf("========================\n");
                // printf("Write done\n");
                sleep(1);
                close(remoteConn2);
                close(remoteConn1);
                exit(3);
            }

            // sleep(1);
            // For player 2
            if(data[5]==1){
                // sleep(1);

                write(remoteConn2,"You can now play.",sizeof("You can now play."));
                sleep(1);
                write(remoteConn2,&data,sizeof(data));
                sleep(1);
                read(remoteConn2, &data, sizeof(data));
                printf("========================\n");
                printf("GameID: %d\n", gameId);
                printf("Player 1 score: %d\n", data[2]);
                printf("Player 2 score: %d\n", data[3]);
                printf("========================\n");

            } 
            if(data[5]==0){
                write(remoteConn1,"Player 2 Exited.\nYou won the game.",sizeof("Player 2 Exited.\nYou won the game."));
                printf("========================\n");
                printf("GameID: %d\n", gameId);
                printf("Game over: Player 2 Quited...!!!\nPlayer 1 Won...");
                printf("========================\n");
                sleep(1);
                close(remoteConn1);
                close(remoteConn2);
                exit(2);
            }
        }
        else{
            // sleep(1);
            // For Player 1
            if(data[2] > data[3]){
                if(data[4]==1){
                    write(remoteConn1,"Game over: you won the game.",sizeof("Game over: you won the game."));
                    // sleep(1);
                    write(remoteConn2,"Game over: you lost the game.",sizeof("Game over: you lost the game."));
                    printf("========================\n");
                    printf("GameID: %d\n", gameId);
                    printf("Game over: Player 1 wins...\n");
                    printf("========================\n");
                    close(remoteConn1);
                    close(remoteConn2);
                    exit(0);
                }
                // else{
                //     printf("Game over...!!! \n");
                //     exit(0);
                // }
            }
            else{
                // For player 2
                if(data[5]==1){
                    write(remoteConn2,"Game over: you won the game.",sizeof("Game over: you won the game."));
                    // sleep(1);
                    write(remoteConn1,"Game over: you lost the game.",sizeof("Game over: you lost the game."));
                    printf("========================\n");
                    printf("GameID: %d\n", gameId);
                    printf("Game over: Player 2 wins...\n");
                    printf("========================\n");
                    close(remoteConn1);
                    close(remoteConn2);
                    exit(1);
                }
                // else{
                //     printf("Game over...!!! \n");
                // }
            } 
            // waitpid(0,&status,WNOHANG);
            // exit(0);
        } 
       
    }  
}

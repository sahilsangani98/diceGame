//client
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

int getRandomDiceNumber(int n){
  return(rand()%n + 1);
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    // int coverted_dice, converted_pl_status ,coverted_data[6];
    // int pl_status = 1;
    int player_id=2;
    int sock, port,dice=0;
    char message[255];
    //str array to record string inputs and messages
    char str[6];
     //Fields in data array
    //Fields in data array
    //0 - First element tracks the player1 dice value
    //1 - Second element tracks the player2 dice value
    //2 - Third element tracks the player1 score   
    //3 - Fourth element tracks the player2 score  
    //4 - Fifth element specifies the number of turns for player1 +3  +3
    //5 - Sixth element specifies the number of turns for player2 +3  +5
    // int data[6]={0,0,0,0,3,3};
    int data[6],n=10;
    struct sockaddr_in clientConfig;
    // printf("In client\n");
    
    if(argc != 3){
        printf("Command Signature: %s IPAddress PortNumber\n", argv[0]);
    exit(0);
    }
    //create a soccket for connection
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Socket cannot be created\n");
    exit(1);
     }
     clientConfig.sin_family = AF_INET;
     sscanf(argv[2],"%d",&port);
     clientConfig.sin_port = htons((uint16_t)port);
     
     if(inet_pton(AF_INET,argv[1],&clientConfig.sin_addr)<0){
           fprintf(stderr, " Conversion of internet address has failed\n");
            exit(2);
     } 
     
    //connect to server
    if(connect(sock, (struct sockaddr *) &clientConfig, sizeof(clientConfig))<0){
        fprintf(stderr, "connect() has failed, exiting\n");
        exit(3);
    }
        
    printf("Connected to server\n");
 
    //read message from server
    if((read(sock,&message,sizeof(message)))<0){
        printf("No directive message from server\n");
        // exit(0);
    }
    printf("%s\n",message);
    // sleep(2);

    read(sock,&player_id,sizeof(player_id));
    printf("player_id: %d\n", player_id);
    // sleep(1);

    while(1){

        
        if((read(sock,&message,sizeof(message)))>0){
            printf("====================\n");
            printf("Referee: %s\n",message);
            printf("====================\n");
        }

        
        //check if client has sent data
        if(strcmp(message,"You can now play.")==0){
            // sleep(1);
            // read(sock,&data,sizeof(data));
            if((read(sock,&data,sizeof(data)))>0){

                //if statement (for specific condition)
                    if(player_id ==1){

                        if(data[5] == 0){
                            printf("Opponent left...\n");
                            printf("You won the game...\n");
                            write(sock,&data,sizeof(data));
                            sleep(1);
                            exit(5);
                        }
                        else if(data[4]==1){
                            printf("Do you want to continue? (y/n)\n");
                            gets(str);
                            // sleep(1);
                            if(strcmp(str,"n")==0){
                                printf("You quit the game.\n");
                                data[4] = 0;
                                write(sock,&data,sizeof(data)); 
                                sleep(1);
                                exit(3);
                            }
                     
                            dice = getRandomDiceNumber(n);
                            printf("You got number: %d\n", dice);
                            data[2] = data[2] + dice;
                            write(sock,&data,sizeof(data));
                            sleep(1);
                        }
                        else{
                            printf("Game over...!!\n");
                            exit(6);
                        }
                    }
                else if(player_id ==2){

                    if(data[4] == 0){
                        write(sock,&data,sizeof(data));
                        printf("Opponent left...\n");
                        printf("You won the game...\n");
                        sleep(1); 
                        exit(5);
                    }

                    if(data[5]==1){
                        printf("Do you want to continue? (y/n)\n");
                        gets(str);
                        // sleep(1);
                        if(strcmp(str,"n")==0){
                            printf("You quit the game.\n");
                            data[5] = 0;
                            write(sock,&data,sizeof(data)); 
                            sleep(1); 
                            exit(3);
                        } 
                        dice = getRandomDiceNumber(n);
                        printf("You got number: %d\n", dice);
                        data[3] = data[3] + dice;
                        write(sock,&data,sizeof(data));
                        sleep(1); 
                    } 
                }
            }
            // printf("data read\n");
            // sleep(1);
            
        }
        else if(strcmp(message,"Game over: you won the game.")==0){
            printf("I won the game\n");
            exit(0);
        }
        else if(strcmp(message,"Game over: you lost the game.")==0){
            printf("I lost the game\n");
            exit(1);
        }
        else{
            printf("Game over...!!!!!!\n");
            exit(2);
        }

    }
}



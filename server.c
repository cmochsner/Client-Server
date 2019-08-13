#include <fstream>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT

using namespace std;

int main(){
	
	//read file
	int acct[20];
	double balance[20];
	char user[20][20];
	string name;
	
  	printf("What is your file name?\n");
  	getline(cin, name);
	cout<<endl;
  
	ifstream file (name.c_str());
	
	if (file.is_open())
 	{
		int x=0;
		while ( !file.eof()) {		
		file>>acct[x]>>balance[x]>>user[x]; 
		if( file.eof() ) break;
		
		x++;
		}
		file.close();	
	}
  	else 
  	{
		cout << "Unable to open file"<<endl; 
		return 0;
	}	
	int portNum;	
	
	cout<<"Enter your port number: ";
	cin>>portNum;
	cout<<endl;
	
	//socket
	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char b[100];
	char a[100];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Connection Error\n");
		exit(1);
	}
	printf("***Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("Error in binding.\n");
		exit(1);
	}
	printf("***Bind to port %d\n", portNum, "...");

	if(listen(sockfd, 10) == 0)
	{
		printf("***Listening\n");
	}
	else
	{
		printf("Error in binding.\n");
	}
		
		//server options
		while(1)
		{
			newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
			if(newSocket < 0)
			{
				exit(1);
			}
			printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
			char choice;
			if((childpid = fork()) == 0){
				close(sockfd);
					
				while(1)
				{
					recv(newSocket, &b, sizeof(b), 0);
					if(strcmp(b, ":exit") == 0)
					{
						printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					}
					else
					{
						
						stringstream converter;
						converter<<b<<b<<b;
						
						int choice;
						int account;
						double amount;;
						double newbal;
						char c[10];
						char d[10];
						int pos;
						bool login;
						
						converter>>choice>>account>>amount;
						for(int i=0;i<20;i++)
						{
							if(account==acct[i])
							{
								login=true;
								pos=i;
								break;
							}
							i++;
						}
						if(login==false)
						{
							login=false;
							string s="Login failed.";
							strcpy(a, s.c_str());
							send(newSocket, a, strlen(a), 0);
							bzero(b, sizeof(b));
							bzero(a, sizeof(a));
							close(newSocket);
							break;
						}
						
						switch(choice)
						{
							case 0:
							{
								strcat(a,"Welcome ");
								strcat(a,user[pos]);
								strcat(a,"!");
								
								send(newSocket, a, strlen(a), 0);
								bzero(b, sizeof(b));
								bzero(a, sizeof(a));
							}
							break;
							case 1:
							{
								gcvt (balance[pos],6,c);
								strcat(a,"Your current balance is ");
								strcat(a, c);
								
								send(newSocket, a, strlen(a), 0);
								bzero(b, sizeof(b));
								bzero(a, sizeof(a));
							}
							break;
							case 2:
							{
								newbal=balance[pos]+amount;
								
								
								gcvt (balance[pos],6,c);
								gcvt (newbal,6,d);
								
								balance[pos]=newbal;
								
								strcat(a,"Your balance went from ");
								strcat(a, c);
								strcat(a," to ");
								strcat(a,d);
								
								send(newSocket, a, strlen(a), 0);
								bzero(b, sizeof(b));
								bzero(a, sizeof(a));
							}
							break;
							case 3:
							{
								newbal=balance[pos]-amount;
								
								gcvt (balance[pos],6,c);
								gcvt (newbal,6,d);
								
								balance[pos]=newbal;
								
								strcat(a,"Your balance went from ");
								strcat(a, c);
								strcat(a," to ");
								strcat(a,d);
								
								send(newSocket, a, strlen(a), 0);
								bzero(b, sizeof(b));
								bzero(a, sizeof(a));
							}
							break;
							case 4:
							{
								close(newSocket);
								return 0;
							}
							break;
							default:
							{
								bzero(b, sizeof(b));
								bzero(a, sizeof(a));
							}
							break;
						}
					}

				}	
			}
		}
		
	close(newSocket);
	return 0;
}
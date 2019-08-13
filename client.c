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

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char b[100];
	char a[100];

	int portNum;
	char servAdd[15];
	
	cout<<"Enter your port number: ";
	cin>>portNum;
	cout<<endl;
	cout<<"Enter your host name: ";
	cin>>servAdd;
	cout<<endl;
	
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("Connection error\n");
		exit(1);
	}
	printf("***Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("Connection error\n");
		exit(1);
	}
	printf("***Connected to Server.\n\n");
	
	int choice;
	double amount;
	char account[20];
	bool login=false;
	char c[10];
	char d[3];
	
	cout<<"Enter your account number: ";
	cin>>account;
	cout<<endl;
	
	while(choice!='4')
	{
		cout<<"Choose an option by inputting the associated number."<<endl;
		cout<<"0: login"<<endl;
		cout<<"1: balance"<<endl;
		cout<<"2: deposit"<<endl;
		cout<<"3. withdraw"<<endl;
		cout<<"4: close connection"<<endl;
		cin>>choice;
		cout<<endl;
		switch(choice)
		{
			case 0:
			{
				login=true;
			}
			break;
			case 1:
			{
				if(login==true)
				{
				}
				else
				{
					cout<<"Login first please."<<endl<<endl;
				}
			}
			break;
			case 2:
			{
				if(login==true)
				{
					cout<<"Enter the amount to be deposited: ";
					cin>>amount;
					cout<<endl;
					if(amount<=0)
					{
						while(amount<=0)
						{
							cout<<"Amount is too small. Try again."<<endl;
							cout<<"Enter the amount to be deposited: ";
							cin>>amount;
							cout<<endl;
							if(amount>0)
							{
								break;
							}
						}
					}
					gcvt (amount,6,c);
				}
				else
				{
					cout<<"Login first please."<<endl<<endl;
				}
				
			}
			break;
			case 3:
			{
				if(login==true)
				{
					cout<<"Enter the amount to be withdrawn: ";
					cin>>amount;
					cout<<endl;
					if(amount<=0)
					{
						while(amount<=0)
						{
							cout<<"Amount is too small. Try again."<<endl;
							cout<<"Enter the amount to be withdrawn: ";
							cin>>amount;
							cout<<endl;
							if(amount>0)
							{
								break;
							}
						}
					}
					gcvt (amount,6,c);
				}
				else
				{
					cout<<"Login first please."<<endl<<endl;
				}
					
			}
			break;
			case 4:
			{
				close(clientSocket);
				cout<<"Disconnected."<<endl;
				return 0;
			}
			break;
			default:
			{
				cout<<"Invalid value."<<endl;
			}
			break;
		}
		if(login==true)
		{
			sprintf(d,"%d",choice);
			
			strcat(b, d);
			strcat(b, " ");
			strcat(b, account);
			strcat(b, " ");
			strcat(b, c);
			strcat(b, " ");

			send(clientSocket, &b, sizeof(b), 0);
			
		if(recv(clientSocket, a, 100, 0) < 0)
			{
				printf("[-]Error in receiving data.\n");
			}
			else
			{
				cout<<a<<endl<<endl;
				if(strcmp(a,"Login failed.")==0)
				{
					bzero(a, sizeof(a));
					bzero(b, sizeof(b));
					close(clientSocket);
					return 0;
				}
				bzero(a, sizeof(a));
				bzero(b, sizeof(b));
			}
		}
	
	}
	return 0;
}
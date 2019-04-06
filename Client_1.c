#include<stdio.h>	//C library
#include<stdlib.h>	//memory allocation
#include<sys/types.h>	//size_t
#include<sys/socket.h>	//socket
#include<netinet/in.h>	//socketaddr_in
#include<arpa/inet.h>	//socket
#include<unistd.h>	//data type
#include<sys/stat.h>    //mkdir
#include<dirent.h>
#include<stdlib.h>
#include<memory.h>

int main(){
	char choice;
	char ans;
	struct sockaddr_in sock_var;
	int clientFileDescriptor = socket(AF_INET,SOCK_STREAM,0);
	char str_client[30];
	char *str_server = (char*)malloc(30);
	char *foldername = (char*)malloc(30);

	sock_var.sin_addr.s_addr = inet_addr("127.0.0.1");
	sock_var.sin_port = 3000;
	sock_var.sin_family = AF_INET;


	if(connect(clientFileDescriptor, (struct sockaddr*)&sock_var, sizeof(sock_var))>=0){
		int client_num;
		printf("Enter your user number:");
		scanf("%d",&client_num);
		str_client[0] = client_num;
		write(clientFileDescriptor, str_client, 30);


		//printf("\nConnected to Server!");
		read(clientFileDescriptor, str_server, 30);
		printf("\n%s", str_server);

		printf("\nA.Login \nB.SignUp ");
		printf("\nEnter choice :");

		scanf("%s",str_client);
		write(clientFileDescriptor,str_client,30);  	//choice is given to server

		if(str_client[0] == 'A'){
			read(clientFileDescriptor,str_server,30);	//pass or folder query from server
		    printf("\n%s",str_server);

		    scanf("%s",str_client);						//password
			write(clientFileDescriptor,str_client,30);

			read(clientFileDescriptor, str_server, 30);
			printf("\n%s", str_server);


			printf("A).Show all docs \n B).Edit a document \n C).Create new file \n D).Delete a file");
			printf("\nEnter Choice:");

			scanf("%s",str_client);
			write(clientFileDescriptor, str_client, 30);

		}else{
			read(clientFileDescriptor,str_server,30);	//folder query from server
		    printf("\n%s",str_server);

			scanf("%s",str_client);						//foldername
			write(clientFileDescriptor,str_client,30);

			strcpy(foldername, str_client);

			read(clientFileDescriptor,str_server,30);	//password query from server
		    printf("\n%s",str_server);

			scanf("%s",str_client);						//password
			write(clientFileDescriptor,str_client,30);

			///client folder created here
			DIR *d;
	  		  struct dirent *dir;

			  int check;
			  char *dirname = (char*)malloc(300);
			  char *path = (char*)malloc(100);
			  path = "/home/himangi/OS/Server/";


			  d = opendir("/home/himangi/OS/Server/");

			  strcpy(dirname, path);
			  strcat(dirname, foldername);

			  //dirname = "/home/himangi/folder";

			  if(d){
			    while((dir = readdir(d))!=NULL){
			      printf("%s\n",dir->d_name);
			      if(strcmp(dir->d_name, foldername)==0){
			         printf("\nDirectory exists!\n");
			         break;
			    }else{
			        check = mkdir(dirname,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

			        if (!check){
			          printf("Directory created\n");
			          break;
			        }else{
			         printf("Unable to create directory\n");
			         break;
			        }

			      }
			    }
			  }
			  //end



			printf("Login to Access Your Folder");
			close(clientFileDescriptor);
			exit(1);
		}

		close(clientFileDescriptor);
	}else{
		printf("\nConnection failed!");
	}
	return 0;
}

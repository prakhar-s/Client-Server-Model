#include<stdio.h>	//C library
#include<stdlib.h>	//memory allocation
#include<sys/types.h>	//size_t
#include<sys/socket.h>	//socket
#include<netinet/in.h>	//socketaddr_in
#include<arpa/inet.h>	//socket
#include<unistd.h>	//data type
#include<pthread.h>	//thread
#include<string.h>
#include<sys/stat.h>    //mkdir
#include<dirent.h>
#include<stdlib.h>
#include<memory.h>
#include<inttypes.h>

void *ServerEcho(void *args){
	char *p[20];
	char folderpath[100];
	DIR *dir;
	struct dirent *ent;
	char s[100];
	int i=0,fileno;
	FILE *out;
	static int count=10;
	p[0] = NULL;
	p[1] = NULL;
	p[2] = NULL;
	p[3] = "Three";
	p[4] = "Four";
	p[5] = "Five";
	p[6] = "Six";
	p[7] = "Seven";
	p[8] = NULL;
	p[9] = NULL;


	int clientFileDescriptor = (int)args;
	char *str = (char*)malloc(30);
	char *msg = (char*)malloc(30);
	char id = (int)clientFileDescriptor;

	read(clientFileDescriptor, str, 30);
	clientFileDescriptor = str[0];

	str = "server connected";
	write(clientFileDescriptor, str, 30);

	read(clientFileDescriptor,str,30);				//server reads the choice
	printf("\nReading from client:%d %s",clientFileDescriptor, str);

	if(str[0] == 'A'){
		msg = "Password:";
		write(clientFileDescriptor, msg, 30);			//password query to client

		read(clientFileDescriptor, str, 30);			//reads password from client

		if(strcmp(p[clientFileDescriptor], str)==0){
			msg = "Access Granted!";
			write(clientFileDescriptor, msg, 30);

			//open directory
			snprintf(folderpath, sizeof(folderpath), "/home/himangi/OS/Server/client%d",clientFileDescriptor );
			if ((dir = opendir (folderpath)) != NULL) {
		  		/* print all the files and directories within directory */
		  	while ((ent = readdir (dir)) != NULL) {
				printf("%d .",i);
				i++;
				//printf("% ." PRIuMAX, (uintmax_t)ent->d_ino);
		    	printf ("%s\n", ent->d_name);
		  	}
		  	//closedir (dir);
			} else {
		  		/* could not open directory */
		  		printf ("could not open");
		  		return ;
			}


			read(clientFileDescriptor, str, 30);
			printf("%c",str[0]);
		}else{
			printf("\nWrong Password!%d",clientFileDescriptor);
		}
	}else{
		//printf("This here: %c",str[0]);
		msg = "Foldername:";
		write(clientFileDescriptor,msg,30);					//send foldername query to client

		read(clientFileDescriptor, str, 30);				//reads foldername from client
		printf("\nFoldername: %s",str);

		msg = "Password:";
		write(clientFileDescriptor, msg, 30);				//send password query to client

		read(clientFileDescriptor, str, 30);				//reads password from client
		printf("\nPassword: %s",str);

		p[count]=str;
		++count;

	}

	printf("\nEchoing back to client");

	close(clientFileDescriptor);
}

int main(){
	struct sockaddr_in sock_var;	//sockaddr_in,basic structure for all syscalls, function of internet
	int serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
	int clientFileDescriptor;
	int i;
	pthread_t t[20];	//20 clients to be handled

	//Server folder created from here
   	  DIR *d;
	  struct dirent *dir;

	  int check;
	  char *dirname = (char*)malloc(300);
	  char *foldername = (char*)malloc(10);
	  char *path = (char*)malloc(100);
	  path = "/home/himangi/OS/";

	  foldername = "Server";

	  d = opendir("/home/himangi/OS/");

	  strcpy(dirname, path);
	  strcat(dirname, foldername);

	  //dirname = "/home/himangi/folder";

	  if(d){
	    while((dir = readdir(d))!=NULL){
	      //printf("%s\n",dir->d_name);
	     if(strcmp(dir->d_name, foldername)==0){
	       printf("\nServer already created!\n");
	       break;
	      }else{
	        check = mkdir(dirname,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	        if (!check){
	          printf("New Server created\n");
	          break;
	        }else{
	         printf("Unable to create server\n");
	         break;
	        }

	      }
	    }
	  }
	  //end

	//defining variables in structure sock_var
	sock_var.sin_addr.s_addr = inet_addr("127.0.0.1");	//sock_var contains a structutre in_addr
	sock_var.sin_port = 3000;
	sock_var.sin_family = AF_INET;
	//bind function need pointer to socket address structure
	if(bind(serverFileDescriptor,(struct sockaddr*)&sock_var,sizeof(sock_var))>=0){
		printf("\nSocket created");			//creating server
		listen(serverFileDescriptor,0);
		while(1){
			for(int i=0;i<20;++i){
				clientFileDescriptor = accept(serverFileDescriptor,NULL,NULL);	//stores client number
				printf("\nConnected to client:%d\n", clientFileDescriptor);
				pthread_create(&t,NULL,ServerEcho,(void *)clientFileDescriptor);
			}
		}

		close(serverFileDescriptor);
	}else{
		printf("\nConnection failed!");
	}
	return 0;

}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include<sqlite3.h>
#include"database.h"
#include"person.h"
#include <vector>

#define bufsize 256
#define PORT 2000                  

typedef struct thData{
	int idThread;                     
	int cl; 
  int ret;                         
}thData;

static void *treat(void *);        
int raspunde(void *);

int main()
{
  struct sockaddr_in server;	      
  struct sockaddr_in from;	
  int nr;		                        
  int sd;		                        
  int pid;
  pthread_t th[100];                
	int i=0;
  std::vector<int> filedescriptors;
  int* res =(int*)malloc(sizeof(int));
                                                  
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)     
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
 
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  

  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  

    server.sin_family = AF_INET;	
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);
  

  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  if (listen (sd, 10) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  while (1)
    {
      int client;
      thData * td;                                          
     socklen_t length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);


      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}
	filedescriptors.push_back(client);


	td=(struct thData*)malloc(sizeof(struct thData));	
	td->idThread=i++;
	td->cl=client;

	pthread_create(&th[i], NULL, &treat, td);	
  // for(int j=i;j>=0;j--)
  // pthread_join(th[i],(void**)&res);
  // cout<<"hehe";

				
	}//while    
};				
void *treat(void * arg)
{		

  
  int login=0;
  while(1)
  {
  struct thData *tdL=(struct thData *)arg; 
    if(login==0)
    {
    char nameperson[1000];
    char mailperson[1000];
    bzero(nameperson,sizeof(nameperson));
    if (read (tdL->cl, &nameperson,sizeof(nameperson)) < 0)
     {
       perror ("[client]Eroare la read() de la client.\n");
     }

     if (read (tdL->cl, &mailperson,sizeof(mailperson)) < 0)
     {
       perror ("[client]Eroare la read() de la client.\n");
     }

    database hope;
    hope.open();
    person user(hope.executeid("SELECT MAX(participant_id) FROM participant;"),nameperson, mailperson);    //folosim pentru a crea un id unic pentru fiecare utilizator
    hope.execute(user.dbperson()); 
    login=1;
    } 

	printf ("[thread]- %d - Trimitem intrebare...\n", tdL->idThread);
	fflush (stdout);		 
	pthread_detach(pthread_self());	
	int nr, i=0;
  char buf[bufsize];
  strcpy(buf,"intrebare test");

if (write (tdL->cl, &buf, sizeof(buf)) <= 0)
		{
		 printf("[Thread %d] ",tdL->idThread);
		 perror ("[Thread]Eroare la write() catre client.\n");
		}
    
  bzero(buf,sizeof(buf));
	if (read (tdL->cl, &buf,sizeof(buf)) <= 0)
			{
			  printf("[Thread %d]\n",tdL->idThread);
			  perror ("Eroare la read() de la client.\n");
			
			}
	printf ("[Thread %d]Raspunsul a fost receptionat...%s\n",tdL->idThread, buf);
		      
    if(strcmp(buf,"exit")==0){
      close((intptr_t)arg);
      return(&(tdL->cl));
     }
  }
  		
};


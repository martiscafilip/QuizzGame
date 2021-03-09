#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include "database.h"
#include "person.h"
#include <sqlite3.h>
#include <string>
#define bufsize 256
int port;

int main (int argc, char *argv[])
{
  int sd;			                                         
  struct sockaddr_in server;	                          
  int nr=0;
  char buf[bufsize];
  int exitt=0;
  
  if (argc != 3)    
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }
  
  port = atoi (argv[2]);     
   



        //char* nameperson=(char*)malloc(1000);
        char nameperson[1000];
        char mailperson[1000];
        cout<<"Please login"<<endl;
        cout<<"Username: ";
        cin>>nameperson;
       cout<<"Mail: ";
        cin >>mailperson;

   
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }


  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

    if (write (sd,&nameperson,sizeof(nameperson)) <= 0)
     {
       perror ("[client]Eroare la write() spre server.\n");
       return errno;
     }

if (write (sd,&mailperson,sizeof(mailperson)) <= 0)
     {
       perror ("[client]Eroare la write() spre server.\n");
       return errno;
     }

  while(1)
  {

 bzero(buf, sizeof(buf));
 if (read (sd, &buf,sizeof(buf)) < 0) //citim si afisam intrebarea
     {
       perror ("[client]Eroare la read() de la server.\n");
       return errno;
     }

    
     cout<<buf<<endl;

   printf ("[client]Introduceti un raspuns: "); //raspundem la intrebare
   fflush (stdout);
   bzero(buf, sizeof(buf));
   scanf("%s",buf);

 
   /* trimiterea mesajului la server */
   if (write (sd,&buf,sizeof(buf)) <= 0)
     {
       perror ("[client]Eroare la write() spre server.\n");
       return errno;
     }
    if(strcmp(buf,"exit")==0){
     close(sd);
     exit(0);
   }
  }
}
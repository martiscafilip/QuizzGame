#pragma once 

#include <stdio.h>
#include<sqlite3.h>
#include<string>
#include<iostream>
#include<vector>
using namespace std;



class database
{
    public:
    sqlite3* DB;

    public:
    database(){

    };
    void open( )
    {
        
        int open=0;
        open = sqlite3_open("questions.db",&DB);
        if(open)
        {
            std::cerr<<"Can't open DataBase"<<sqlite3_errmsg(DB)<<std::endl;
        }
        else 
        {
            std::cout<<"Opened DataBase Succesfully!"<<std::endl;
        }
    }

    static int function(void* data, int argc, char**argv, char**azColName){
         int i;
         fprintf(stderr, "%s: ", (const char*)data);

         for(i=0;i<argc;i++){
             printf("%s = %s\n",azColName[i],argv[i]? argv[i]:"NULL");
         }
         std::cout<<std::endl;
         return 0;
     };

    //  static int retid(void* data, int argc, char**argv, char**azColName){
    //      int i;
    //      fprintf(stderr, "%s: ", (const char*)data);
    //     // id_t *id=static_cast<id_t*>(data);
    //     std::vector<int> *id=static_cast<std::vector<int>*>data;
    //      *id. = atoi(argv[0]);
    //      return 0;
    //  };

     

    void execute(string command){

        int rval;
        char* error;
        rval = sqlite3_exec(DB, command.c_str(), NULL, 0, &error);
        if(rval!=SQLITE_OK){
            std::cerr<<"Error at Login Database. "<<std::endl;
            sqlite3_free(error);
        }
        else {
            std::cout<<"Login DatabaseID Succes!"<<endl;
        };
    }

   int executeid(string command){
        int sent;
        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(DB, command.c_str(), command.length(),&stmt, nullptr);
        if(rc!=SQLITE_OK)
        {   
            std::cerr<<"Error at Login Databaseid. "<<std::endl;
        }
         else 
         {
             std::cout<<"Login Database Succes!"<<endl;
         }
        while((rc=sqlite3_step(stmt))==SQLITE_ROW)
        {
            sent= sqlite3_column_int(stmt,0);
        }
        sqlite3_finalize(stmt);
        return sent;
    }


};
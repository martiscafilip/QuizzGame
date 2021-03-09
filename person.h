#pragma once
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;
class person
{
    private:

    int id;
    const char* name;
    const char* mail;

    public:
    person(int idd, char* nameperson, char* mailperson)
    {  
        name=nameperson;
        mail=mailperson;
        id=idd+1;
    }

    string dbperson()
    {
        string retunrstring="INSERT INTO participant VALUES(";
       
        char* nr=(char*)malloc(sizeof(id));
        sprintf(nr, "%d", id);
        retunrstring.append(nr);
        retunrstring.append(",'");
        retunrstring.append((const char*)name);
        retunrstring.append("','");
        retunrstring.append(mail);
        retunrstring.append("');");
        char* rr=(char*)retunrstring.c_str();
        return retunrstring;
    }

    
};

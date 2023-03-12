#pragma once

#include <cstddef>
#include <stdlib.h>
#include <cstring>


class  SocketV2
{
public:
    void *data;
    size_t size;
     SocketV2(size_t taille) : size(taille){}
    size_t getSize(){
        return size;
    }
     void* read(){
        return data;
    }
};

class Input : public SocketV2 {
    public : 

    Input(size_t taille) : SocketV2(taille){}

    void set_data(void* data){
        this->data = data;
    }
   
};



class Output : public SocketV2 {
    public : 

    Output(size_t taille) : SocketV2(taille){}
    ~ Output(){free(data);}

    void* write_data(void* to_write){ //Les output sockets 
        data = malloc(sizeof(data)*size*4);
        memcpy(data,to_write,size*4);
        return data;
    }
};

class InOut : public SocketV2{

    public : 
    
    InOut(size_t taille) : SocketV2(taille){}

      void set_data(void* data){
        this->data = data;
    }

};
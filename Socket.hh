#pragma once

#include <cstddef>
#include <stdlib.h>
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;

class  Socket
{
	protected:
	void *data;
	size_t size;
	public:
	Socket(size_t taille) : size(taille){}
	virtual ~Socket(){};
	size_t get_size(){
		return size;
	}

	void set_data(void* data){
		this->data = data;
	}

	void* get_data(){
		return data;
	}

	virtual void* write(){
		return nullptr;
	};
	virtual void* read(){
		return nullptr;
	};

	void display(){
		cout << "size :" << size << endl;
		int * int_array = (int*)data;
		for(size_t i=0; i < size; i++){
			cout << int_array[i] << endl;
		}
	}
};

class Input : public Socket {
	public : 
	Input(size_t taille) : Socket(taille){}

	void* read(){
		return data;
	}
};



class Output : public Socket {
	public : 

	Output(size_t taille) : Socket(taille){}
	~Output(){free(data);}

	void* write(){ //Les output sockets 
		data = malloc(size*sizeof(int)); // On suppose travailler avec des entiers pour cette version 
		return data;
	}
};

class InOut : public Socket{
	public : 
	InOut(size_t taille) : Socket(taille){}

	void* write(){
		return data;
	};
	void* read(){
		return data;
	}
};
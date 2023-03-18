#pragma once

#include <cstddef>
#include <stdlib.h>
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;

class  SocketV2
{
	protected:
	void *data;
	size_t size;
	public:
	SocketV2(size_t taille) : size(taille){}
	~SocketV2() {free(data);}

	size_t get_size(){
		return size;
	}

	void set_data(void* data){
		this->data = data;
	}

	virtual void* write(){
		return nullptr;
	};
	virtual void* read(){
		return data;
	};
	void display(){
		cout << "size :" << size << endl;
		int * int_array = (int*)data;
		for(int i=0; i < size; i++){
			cout << int_array[i] << endl;
		}
	}
};

class Input : public SocketV2 {
	public : 
	Input(size_t taille) : SocketV2(taille){}

	void* read(){
		return data;
	}
};



class Output : public SocketV2 {
	public : 

	Output(size_t taille) : SocketV2(taille){}
	~Output(){free(data);}

	void* write(){ //Les output sockets 
		data = malloc(size*4);
		return data;
	}
};

class InOut : public SocketV2{
	public : 
	InOut(size_t taille) : SocketV2(taille){}
	~InOut(){free(data);}

	void* write(){
		return data;
	};
	void* read(){
		return data;
	}
};
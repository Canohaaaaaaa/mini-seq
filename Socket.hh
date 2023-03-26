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
	Socket() {}
	Socket(size_t size) : size(size){} 
	virtual ~Socket(){};

	size_t get_size(){
		return size;
	}

	void set_data(void *data){
		this->data = data;
	}

	void *get_data(){
		return data;
	}

	void **get_socket_data_addr(){
		return &data;
	}

	void display(){
		cout << "size :" << size << endl;
		int *int_array = (int*)data;
		for(size_t i=0; i < size; i++){
			cout << int_array[i] << endl;
		}
	}
};

class Input : public Socket {
	public : 
	Input(size_t size) : Socket(size){}
	Input(size_t size, void *data){
		this->size = size;
		this->data = data;
	} 
};



class Output : public Socket {
	public : 
	Output(size_t size) : Socket(size){
		data = malloc(size*sizeof(int));
	}
	~Output(){
		free(data);
	}
};

class InOut : public Socket{
	bool alloced = false;
	public : 
	InOut(size_t size) : Socket(size){}
	~InOut(){
		if(alloced)
			free(data);
	}

	void alloc(){ //Les output sockets
		data = malloc(size*sizeof(int)); // On suppose travailler avec des entiers pour cette version 
	}
};
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
	Socket(const size_t size) : size(size){} 
	virtual ~Socket() {};

	size_t get_size() const {
		return size;
	}

	void set_data(void *data) {
		this->data = data;
	}

	void *get_data() const {
		return data;
	}

	void **get_socket_data_addr() {
		return &data;
	}

	void display() {
		cout << "size :" << size/sizeof(int) << endl;
		int *int_array = (int*)data;
		for(size_t i=0; i < size/sizeof(int); i++){
			cout << int_array[i] << endl;
		}
	}
};

class Input : public Socket {
	public : 
	Input(const size_t size) : Socket(size) {}
	Input(const size_t size, void *data) {
		this->size = size;
		this->data = data;
	} 
};



class Output : public Socket {
	public : 
	Output(const size_t size) : Socket(size) {
		data = malloc(size);
	}
	~Output() {
		free(data);
	}
};

class InOut : public Socket{
	bool alloced = false;
	public : 
	InOut(const size_t size) : Socket(size) {}
	~InOut() {
		if(alloced)
			free(data);
	}

	// Si un socket IO est le premier de la sequence, il doit allouer
	void alloc() {
		data = malloc(size);
		alloced = true;
	}
};
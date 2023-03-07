#pragma once
#include <cstddef>

class Socket{
	public:
	void *data;
	int size_in, size_out;
	Socket(size_t size_in, size_t size_out) : size_in(size_in), size_out(size_in) {}
	~Socket() {free(data);}
	void* read(){
		return data;
	}
	void* write(){
		data = malloc(sizeof(size_out) * size_out);
		return data;
	}
};
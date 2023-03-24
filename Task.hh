#pragma once
#include <functional>
#include "Socket.hh"

using std::function;
class Task{
	function<void(size_t, void*, size_t, void*)> func;
	Socket *socket_in;
	Socket *socket_out;
	void* data;

	public:
	Task(function<void(size_t, void*, size_t, void*)> func, Socket* socket_in ,Socket *socket_out) : func(func), socket_in(socket_in), socket_out(socket_out) {}
	void exec(){
		func(socket_in->get_size(), socket_in->read(), socket_out->get_size(), data); // On fait l'hypothèse que la taille des entrée est la même que les sorties 
	}

	Socket* get_input_socket(){
		return socket_in;
	}

	Socket* get_output_socket(){
		return socket_out;
	}

	void* get_data(){
		return data;
	}
	void set_data(void* data){
		this->data = data;
	}
};
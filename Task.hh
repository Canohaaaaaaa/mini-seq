#pragma once
#include <functional>
#include "Socket.hh"

class Task{
	std::function<void(size_t, void*, size_t, void*)> func;
	SocketV2 &socket_in;
	SocketV2 &socket_out;

	public:
	Task(std::function<void(size_t, void*, size_t, void*)> func, SocketV2 &socket_in ,SocketV2 &socket_out) : func(func), socket_in(socket_in), socket_out(socket_out) {}
	void exec(){
		func(socket_in.get_size(), socket_in.read(), socket_out.get_size(), socket_out.write()); // On fait l'hypothèse que la taille des entrée est la même que les sorties 
	}
};
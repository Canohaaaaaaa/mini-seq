#pragma once
#include <functional>
#include "SocketV2.h"

class Task{
	std::function<void(size_t, Input*,Output*)> func;
	Input *socket_in;
	Output *socket_out;

	public:
	Task(std::function<void(size_t,Input*,Output*)> func, Input *socket_in = nullptr ,Output* socket_out = nullptr) : func(func), socket_in(socket_in), socket_out(socket_out) {}
	void exec(){
		func(socket_in->getSize(), socket_in,socket_out); // On fait l'hypothèse que la taille des entrée est la même que les sorties 
	}
};

class TaskIo{

	std::function<void(size_t, InOut*)> func;
	InOut *socket;

	public : 
	 
	TaskIo(std::function<void(size_t,InOut*)> func,InOut* socket = nullptr) : func(func), socket(socket) {}
	void exec(){
		func(socket->getSize(), socket); // On fait l'hypothèse que la taille des entrée est la même que les sorties 
	}

};
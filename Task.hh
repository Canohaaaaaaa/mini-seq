#pragma once
#include <functional>
#include "Socket.hh"

class Task{
	std::function<void(size_t, void *, size_t, void *)> func;
	Socket &socket_in, &socket_out;
	public:
	Task(std::function<void(size_t, void *, size_t, void *)> func, Socket &socket_in ,Socket &socket_out) : func(func), socket_in(socket_in), socket_out(socket_out) {}
	void exec(){
		func(socket_in.size_in, socket_in.read(), socket_out.size_in, socket_out.write());
	}
};
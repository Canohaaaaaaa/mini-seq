#pragma once
#include <functional>
#include "Socket.hh"

using std::function;
class Task{
	function<void(const size_t, const void*, const size_t, void*)> func;
	protected:
	Socket *input_socket;
	Socket *output_socket;
	size_t input_size, output_size; // Nombre d'octets de input_data et output_data respectivement
	// Indirection sur les donnees des sockets et pas directement les espaces alloues
	void **input_data, **output_data;

	public:
	Task() {}
	Task(function<void(const size_t, const void*, const size_t, void*)> func, Socket *input_socket, Socket *output_socket) : func(func), input_socket(input_socket), output_socket(output_socket) {
		input_size = input_socket->get_size();
		input_data = input_socket->get_socket_data_addr();
		output_size = output_socket->get_size();
		output_data = output_socket->get_socket_data_addr();
	}

	virtual void exec() {
		func(input_size, *input_data, output_size, *output_data);
	}

	Socket *get_input_socket() const {
		return input_socket;
	}

	Socket *get_output_socket() const {
		return output_socket;
	}

	void *get_output_data() const {
		return *output_data;
	}

	void *get_input_data() const {
		return *input_data;
	}
};

class Task_IO : public Task{
	function<void(size_t, void*)> func_io;
	public:
	Task_IO(function<void(const size_t, void*)> func, Socket *socket) {
		func_io = func;
		input_socket = socket;
		output_socket = socket;
		input_size = input_socket->get_size();
		input_data = socket->get_socket_data_addr();
		output_size = output_socket->get_size();
		output_data = socket->get_socket_data_addr();
	}

	virtual void exec() {
		func_io(input_size, *input_data);
	}
};
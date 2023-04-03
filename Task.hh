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
	virtual ~Task(){}

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

void increment(const size_t size_in, const void *in_data, const size_t size_out, void *out_data) {
	const int *in_int_array = (int*)in_data;
	// Ici les size_in/out sont des nombres d'octets, a moins d'ecrire des chars on ne veut idealement pas iterer dessus
	size_t in_array_size = size_in / sizeof(int);
	int *out_int_array = (int*)out_data;
	for(size_t i=0; i < in_array_size; i++){
		out_int_array[i] = in_int_array[i] + 1;
	}
}

void increment_io(const size_t size_in, void *data) {
	int * in_int_array = (int*)data;
	int in_array_size = size_in / sizeof(int);
	for(int i=0; i < in_array_size; i++){
		in_int_array[i] = in_int_array[i] + 1;
	}
}

void increment_uint8(const size_t size_in, const void *in_data, const size_t size_out, void *out_data) {
	const uint8_t *in_int_array = (uint8_t*)in_data;
	size_t in_array_size = size_in / sizeof(uint8_t);
	uint8_t *out_int_array = (uint8_t*)out_data;
	for(size_t i=0; i < in_array_size; i++){
		out_int_array[i] = in_int_array[i] + 1;
	}
}

void increment_uint8_io(const size_t size_in, void *data) {
	uint8_t * in_int_array = (uint8_t*)data;
	uint8_t in_array_size = size_in / sizeof(uint8_t);
	for(uint8_t i=0; i < in_array_size; i++){
		in_int_array[i] = in_int_array[i] + 1;
	}
}
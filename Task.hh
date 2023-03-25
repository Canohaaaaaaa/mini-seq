#pragma once
#include <functional>
#include "Socket.hh"

using std::function;
class Task{
	function<void(size_t, void*, size_t, void*)> func;
	Socket *input_socket;
	Socket *ouput_socket;
	size_t input_size, output_size;
	void *input_data, *output_data;

	public:
	Task(function<void(size_t, void*, size_t, void*)> func, Socket *input_socket, Socket *ouput_socket) : func(func), input_socket(input_socket), ouput_socket(ouput_socket) {
		input_size = input_socket->get_size();
		input_data = input_socket->get_data();
		output_size = ouput_socket->get_size();
		output_data = ouput_socket->get_data();
	}

	void exec(){
		func(input_size, input_socket->get_data(), output_size, ouput_socket->get_data());
	}

	Socket *get_input_socket(){
		return input_socket;
	}

	Socket *get_output_socket(){
		return ouput_socket;
	}

	void *get_output_data(){
		return output_data;
	}

	void *get_intput_data(){
		return input_data;
	}
};
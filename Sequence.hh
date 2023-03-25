#pragma once
#include <cstddef>
#include <vector>
#include <utility>
#include <chrono>
#include "Task.hh"
#include "Socket.hh"

using namespace std::chrono;
using std::vector, std::function;

class Sequence{
	bool inOutShouldAlloc = true;
	vector<Task> tasks;
	vector<std::pair<Socket*,Socket*>> sockets;
	public:
	vector<steady_clock::time_point> timestamps;

	~Sequence(){
		for(size_t i=0;i<sockets.size();++i){
			delete sockets[i].second;
		}
	}

	void addTask(size_t size_in, size_t size_out, function<void(size_t, void*, size_t, void*)> func){
		Socket* socket_in = new Input(size_in);
		Socket* socket_out = new Output(size_out);
		socket_out->write();
		if(sockets.size()){
			socket_in->set_data(sockets.back().second->get_data());
		}
		sockets.push_back(std::make_pair(socket_in, socket_out));
		Task newTask(func, socket_in, socket_out);
		newTask.set_data(socket_out->get_data());
		tasks.push_back(newTask);
		inOutShouldAlloc = true;
	}

	void addTask(size_t size, function<void(size_t, void*, size_t, void*)> func){
		Socket* socketInOut = new InOut(size);
		if(inOutShouldAlloc) {
			socketInOut->write();
			inOutShouldAlloc = false;
		}
		else{
			socketInOut->set_data(sockets.back().second->get_data());
		}
		if(sockets.size()){
		}
		sockets.push_back(std::make_pair(socketInOut, socketInOut));
		Task newTask(func, socketInOut, socketInOut);
		newTask.set_data(socketInOut->get_data());
		tasks.push_back(newTask);
	}

	void exec(void * first_input){
		sockets[0].first->set_data(first_input);// On initialise la première socket d'entrée !
		#ifdef BENCH
		timestamps.push_back(steady_clock::now());
		#endif
		for(size_t i=0; i < tasks.size(); i++){
			tasks[i].exec();
			#ifdef BENCH
			timestamps.push_back(steady_clock::now());
			#endif
		}
		
	}
};
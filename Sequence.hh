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
	vector<Task*> tasks;
	vector<std::pair<Socket*,Socket*>> sockets;
	public:
	vector<steady_clock::time_point> timestamps;

	/*
	~Sequence(){
		for(size_t i=0;i<sockets.size();++i){
			delete sockets[i].second;
		}
	}
	*/

	void add_task(size_t size_in, size_t size_out, function<void(size_t, void*, size_t, void*)> func){
		Input *socket_in = new Input(size_in);
		Output *socket_out = new Output(size_out);
		if(sockets.size()){
			socket_in->set_data(sockets.back().second->get_data());
		}
		sockets.push_back(std::make_pair(socket_in, socket_out));
		Task *new_task = new Task(func, socket_in, socket_out);
		tasks.push_back(new_task);
	}

	void add_task(size_t size, function<void(size_t, void*)> func){
		InOut *socket_in_out;
		if(!sockets.size()) {
			socket_in_out = new InOut(size);
			socket_in_out->alloc();
		}
		else if(!dynamic_cast<InOut*>(sockets.back().second)){
			socket_in_out = new InOut(size);
			socket_in_out->set_data(sockets.back().second->get_data());
		}
		else{
			socket_in_out = dynamic_cast<InOut*>(sockets.back().second);
		}
		sockets.push_back(std::make_pair(socket_in_out, socket_in_out));
		Task_IO *new_task = new Task_IO(func, socket_in_out);
		tasks.push_back(new_task);
	}

	// Phase rebinding pour la premiere entree
	void set_input(void *input){
		sockets[0].first->set_data(input);
		if(!dynamic_cast<InOut*>(sockets.front().first)){
			return;
		}
		for(int i=1; i < sockets.size(); i++){
			if(!dynamic_cast<InOut*>(sockets[i].first)){
				sockets[i].first->set_data(sockets[0].first->get_data());
				return;
			}
		}
	}

	void exec(){
		/* // Code debug
		for(size_t i=0; i < sockets.size(); i++){
			cout << "socket " << i << " InputDataAddr " <<  sockets[i].first->get_socket_data_addr() <<endl;
			cout << "socket " << i << " OutputDataAddr " <<  sockets[i].second->get_socket_data_addr() <<endl;
			cout << "socket " << i << " Input " <<  sockets[i].first->get_data() <<endl;
			cout << "socket " << i << " Output " <<  sockets[i].second->get_data() <<endl;
			cout << "Task" << i << " Input " << tasks[i]->get_input_data() << endl;
			cout << "Task" << i << " output " << tasks[i]->get_output_data() << endl;
			sockets[i].second->display();
		}
		*/
		#ifdef BENCH
		timestamps.push_back(steady_clock::now());
		#endif
		for(size_t i=0; i < tasks.size(); i++){
			tasks[i]->exec();
			#ifdef BENCH
			timestamps.push_back(steady_clock::now());
			#endif
		}
	}
};
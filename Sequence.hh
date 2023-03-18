#pragma once
#include <cstddef>
#include <vector>
#include "Task.hh"
#include "Socket.hh"

using namespace std::chrono;
using std::vector, std::function;

struct Triplet{ 
	public:
	size_t size_in, size_out;
	function<void(size_t, void*, size_t, void*)> func;
	Triplet(size_t size_in, size_t size_out, function<void(size_t, void*, size_t, void*)> func) : size_in(size_in), size_out(size_out), func(func){}
};

class Sequence{
	int current_step, max_step;
	vector<Task> tasks;
	vector<Socket*> sockets;
	public:
	vector<steady_clock::time_point> timestamps;
	Sequence(vector<Triplet> ordre, bool io = false) : current_step(0){
		if(!io){
			for(Triplet t : ordre){
				Socket* socket_in = new Input(t.size_in);
				Socket* socket_out = new Output(t.size_out);
				tasks.push_back(Task(t.func, *socket_in, *socket_out));
				sockets.push_back(socket_in);
				sockets.push_back(socket_out);
			}
		}
		else{
			cout << "AYAYAYA" << endl;
			Socket* socket_in_out = new InOut(ordre[0].size_in);
			sockets.push_back(socket_in_out);
			for(Triplet t : ordre){
				tasks.push_back(Task(t.func, *socket_in_out, *socket_in_out));
			}
		}
		max_step = tasks.size();
	}
	~Sequence(){
		for(Socket *s: sockets){
			delete s;
		}
	}

	void exec(void * first_input, bool bench = false){
		void* data = first_input;
		if(bench){
			timestamps.push_back(steady_clock::now());
			for(size_t i=0; i < tasks.size(); i++){
				Socket& socket_in = tasks[i].get_input_socket();
				Socket& socket_out = tasks[i].get_output_socket();
				socket_in.set_data(data);
				tasks[i].exec();
				timestamps.push_back(steady_clock::now());
				data = socket_out.get_data();
				//socket_out.display();
			}
		}
		else{
			for(size_t i=0; i < tasks.size(); i++){
				Socket& socket_in = tasks[i].get_input_socket();
				Socket& socket_out = tasks[i].get_output_socket();
				socket_in.set_data(data);
				tasks[i].exec();
				data = socket_out.get_data();
			}
		}
	}
};
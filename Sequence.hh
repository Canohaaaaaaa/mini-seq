#pragma once
#include <cstddef>
#include <vector>
#include<utility>
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
	vector<std::pair<Socket*,Socket*>> sockets;
	public:
	vector<steady_clock::time_point> timestamps;
	Sequence(vector<Triplet> ordre, bool io = false) : current_step(0){
		if(!io){
			for(size_t i=0;i<ordre.size();++i){
				Socket* socket_in = new Input(ordre[i].size_in);
				Socket* socket_out = new Output(ordre[i].size_out);
				socket_out->write(); // La socket de sortie alloue son propre espace mémoire !
				sockets.push_back(std::make_pair(socket_in,socket_out)); // Création de la pair de sockets pour chaque task
			}
			for (size_t i=0;i<ordre.size();++i){
				if (i!=ordre.size()-1)
					sockets[i+1].first->set_data(sockets[i].second->get_data()); // Réalisation de binding !
				Task newTache(ordre[i].func,sockets[i].first,sockets[i].second);
				newTache.set_data(sockets[i].second->get_data()); // On bind aussi le pointeur relié à la tache vers la bonne sortie
				tasks.push_back(newTache); // Création de la tache avec les bonne sockets
			}
		}
		max_step = tasks.size();
	}
	~Sequence(){
		for(size_t i=0;i<sockets.size();++i){
			delete sockets[i].second;
		}
	}

	void exec(void * first_input, bool bench = false){
		sockets[0].first->set_data(first_input);// On initialise la première socket d'entrée !
		
		/*if(bench){
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
		}*/
	
			
			for(size_t i=0; i < tasks.size(); i++){
				std::cout << "Rentre dans boucle execution" << std::endl;
				tasks[i].exec();
				sockets[i].second->display();
			}
		
	}
};
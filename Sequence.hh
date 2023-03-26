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
	void *result;
	public:
	vector<steady_clock::time_point> timestamps;
	~Sequence(){
		for(std::pair<Socket*,Socket*> pair : sockets){
			if(!dynamic_cast<InOut*>(pair.first)){
				delete pair.first;
			}
			delete pair.second;
		}
		for(Task *task : tasks){
			delete task ;
		}
	}

	// Ajout d'une tache classique
	void add_task(const size_t size_in, const size_t size_out, const function<void(const size_t, const void*, const size_t, void*)> func) {
		Input *socket_in = new Input(size_in);
		Output *socket_out = new Output(size_out);
		//Si ce n'est pas la premiere paire de sockets, on bind le socket d'entree au dernier socket de sortie de la seq
		if(sockets.size()){
			socket_in->set_data(sockets.back().second->get_data());
		}
		sockets.push_back(std::make_pair(socket_in, socket_out));
		Task *new_task = new Task(func, socket_in, socket_out);
		tasks.push_back(new_task);
	}

	/*
	 * Ajout d'une tache IO, une suites de taches IO pointent toutes vers le meme socket en memoire
	 * Pour simplifier, on suppose que si une tache est IO alors ses deux sockets sont necessairement IO aussi
	 */
	void add_task(const size_t size, const function<void(const size_t, void*)> func) {
		InOut *socket_in_out;
		if(!sockets.size()) { // Est-ce le premier socket ? Si oui on doit instancier le socket
			socket_in_out = new InOut(size);
			sockets.push_back(std::make_pair(socket_in_out, socket_in_out));
			socket_in_out->alloc();
		}
		else if(!dynamic_cast<InOut*>(sockets.back().second)){ // Le dernier socket est-il IO ? si non, on doit instancier le socket
			socket_in_out = new InOut(size);
			socket_in_out->set_data(sockets.back().second->get_data());
			sockets.push_back(std::make_pair(socket_in_out, socket_in_out));
		}
		else{ // Sinon, le dernier socket est aussi IO, on ne fait que continuer la suite et on donne a la tache le meme socket
			socket_in_out = dynamic_cast<InOut*>(sockets.back().second);
		};
		Task_IO *new_task = new Task_IO(func, socket_in_out);
		tasks.push_back(new_task);
	}

	// Premiere entree, si notre premier socket est IO, il faut au minimum copier l'entree pour ne pas directement travailler dessus
	void set_input(void *input, size_t size) {
		if(!dynamic_cast<InOut*>(sockets.front().first)){
			sockets[0].first->set_data(input);
			return;
		}
		memcpy(sockets.front().first->get_data(), input, size);
	}

	void *get_result() {
		return result;
	}

	void exec() {
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
		result = sockets.back().second->get_data();
	}
};
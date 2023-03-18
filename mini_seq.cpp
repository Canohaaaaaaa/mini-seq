#include <cstddef>
#include <iostream>
#include <chrono>
#include "Task.hh"
#include "Sequence.hh"
#define SIZE 100000
#define SHOW_OUTPUTS false

using namespace std::chrono;
using std::function, std::vector;

void increment(size_t size_in, void*in , size_t size_out, void *out){
	int * in_int_array = (int*)in;
	int * out_int_array = (int*)out;
	for(size_t i=0; i < size_in; i++){
		out_int_array[i] = in_int_array[i] + 1;
	}
}

int main(void){
	int *in = (int*)malloc(sizeof(int) * SIZE);
	for(int i = 0; i < SIZE; i++){
		in[i] = i; 
	}
	//##Functions###//
	function<void(size_t, void*, size_t, void*)> func_inc = increment;
	vector<Triplet> v;
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	v.push_back(Triplet(100000, 100000, func_inc));
	Sequence seq(v);
	seq.exec(in);
	auto seq_start = seq.timestamps[0];
	auto seq_end = seq.timestamps[seq.timestamps.size()-1];
	for(size_t i=1; i < seq.timestamps.size(); i++){
		auto time_taken = seq.timestamps[i] - seq.timestamps[i-1];
		cout << "Temps de traitement de la tache "<< i << " : " << duration_cast<microseconds>(time_taken).count() << "ms" << endl;
	}
	cout << "Temps total : " << duration_cast<microseconds>(seq_end-seq_start).count() << "ms" << endl;
	//---------------- Ancienne Version -----------------------//
	/*

	//###Sockets###//
	Socket *socket_1 = new Input(SIZE);
	Socket *socket_2 = new Output(SIZE);
	Socket *socket_3 = new Input(SIZE);
	Socket *socket_4 = new Output(SIZE);
	Socket *socket_5 = new Input(SIZE);
	Socket *socket_6 = new Output(SIZE);
	socket_1->set_data(in); // Mise en place de l'entrée du programme
	//###Binding###//
	vector<steady_clock::time_point> timestamps;
	Task task_1(func_inc, *socket_1, *socket_2);
	
	Task task_2(func_inc, *socket_3, *socket_4);
	Task task_3(func_inc, *socket_5, *socket_6);
	//###Exec###//
	auto start = steady_clock::now();
	task_1.exec();
	socket_3->set_data(socket_2->read());
	task_2.exec();
	socket_5->set_data(socket_4->read());
	task_3.exec();
	auto end = steady_clock::now();

	
	cout << "Temps de traitement de la version 1 : "<< duration_cast<microseconds>(end-start).count() << "ns" << endl;

	//###Version InOut###//
	Socket *socket_1_io = new InOut(SIZE);
	socket_1_io->set_data(in);
	
	Task task_1_io(func_inc, *socket_1_io, *socket_1_io);
	Task task_2_io(func_inc, *socket_1_io, *socket_1_io);
	Task task_3_io(func_inc, *socket_1_io, *socket_1_io);
	
	start = steady_clock::now();
	task_1_io.exec();
	task_2_io.exec();
	task_3_io.exec();
	end = steady_clock::now();


	cout << "Temps de traitement de la version 2 : "<< duration_cast<microseconds>(end-start).count() << "ns" << endl;

	
	delete socket_1;
	delete socket_2;
	delete socket_3;
	delete socket_4;
	delete socket_5;
	delete socket_6;
	delete socket_1_io;
	*/
	free(in);
	return 0;
}
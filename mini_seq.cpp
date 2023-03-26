#include <cstddef>
#include <iostream>
#include <chrono>
#include <fstream>
#include "Task.hh"
#include "Sequence.hh"
#define SIZE 10
#define SIZE_SEQ 10
#define DETAIL false

using namespace std::chrono;
using std::function, std::vector;

void increment(const size_t size_in, const void *in_data, const size_t size_out, void *out_data) {
	const int *in_int_array = (int*)in_data;
	// Ici les size_in/out sont des nombres d'octets, a moins d'ecrire des chars on ne veut idealement pas iterer dessus
	size_t in_array_size = size_in / sizeof(int);
	int *out_int_array = (int*)out_data;
	size_t out_array_size = size_out / sizeof(int);
	for(size_t i=0; i < in_array_size && i < out_array_size; i++){
		out_int_array[i] = in_int_array[i] + 1;
	}
}

void increment_io(const size_t size_in, void *data) {
	int * in_int_array = (int*)data;
	int in_array_size = size_in / sizeof(int);
	for(size_t i=0; i < in_array_size; i++){
		in_int_array[i] = in_int_array[i] + 1;
	}
}

void sequence(){
	Sequence seq;
	Sequence seq_io;
	seq.add_task(SIZE * sizeof(int), SIZE * sizeof(int), increment);
	seq.add_task(SIZE * sizeof(int), SIZE * sizeof(int), increment);
	seq.add_task(SIZE * sizeof(int), SIZE * sizeof(int), increment);

	seq_io.add_task(SIZE* sizeof(int),                  increment_io);
	seq_io.add_task(SIZE* sizeof(int), SIZE * sizeof(int), increment);
	seq_io.add_task(SIZE* sizeof(int),                  increment_io);
	seq_io.add_task(SIZE* sizeof(int), SIZE * sizeof(int), increment);

	int *in = (int*)malloc(sizeof(int) * SIZE);
	for(int i = 0; i < SIZE; i++){
		in[i] = i; 
	}
	
	seq.set_input(in, sizeof(int) * SIZE);
	seq.exec();
	seq_io.set_input(in, sizeof(int) * SIZE);
	seq_io.exec();
	free(in);
}

void bench_sequence(){
	#ifndef BENCH
	return;
	#endif
	int *in = (int*)malloc(sizeof(int) * SIZE);
	for(int i = 0; i < SIZE; i++){
		in[i] = i; 
	}
	Sequence seq_copy;
	Sequence seq_copyless;
	for(int i=0; i < SIZE_SEQ; i++){
		seq_copy.add_task(SIZE * sizeof(int), SIZE * sizeof(int), increment);
		seq_copyless.add_task(SIZE * sizeof(int), increment_io);
	}

	seq_copy.set_input(in, sizeof(int) * SIZE);
	seq_copy.exec();
	seq_copyless.set_input(in, sizeof(int) * SIZE);
	seq_copyless.exec();
	//----------COPY-BENCH----------//
	auto seq_start = seq_copy.timestamps[0];
	auto seq_end = seq_copy.timestamps[seq_copy.timestamps.size()-1];
	for(size_t i=1; i < seq_copy.timestamps.size() && DETAIL; i++){
		auto time_taken = seq_copy.timestamps[i] - seq_copy.timestamps[i-1];
		cout << "Temps de traitement de la tache "<< i << " (Sequence avec copie) : " << duration_cast<microseconds>(time_taken).count() << "ms" << endl;
	}
	cout << "Temps total (Sequence avec copie) : " << duration_cast<microseconds>(seq_end-seq_start).count() << "ms" << endl;
	//file << duration_cast<microseconds>(seq_end-seq_start).count() << "\t";
	//----------COPYLESS-BENCH----------//
	seq_start = seq_copyless.timestamps[0];
	seq_end = seq_copyless.timestamps[seq_copyless.timestamps.size()-1];
	for(size_t i=1; i < seq_copyless.timestamps.size() && DETAIL; i++){
		auto time_taken = seq_copyless.timestamps[i] - seq_copyless.timestamps[i-1];
		cout << "Temps de traitement de la tache "<< i << " (Sequence sans copie) : " << duration_cast<microseconds>(time_taken).count() << "ms" << endl;
	}
	cout << "Temps total (Sequence sans copie) : " << duration_cast<microseconds>(seq_end-seq_start).count() << "ms" << endl;
	//file <<  duration_cast<microseconds>(seq_end-seq_start).count()<< endl;
	free(in);
}

void tache() {
	int *in = (int*)malloc(SIZE * sizeof(int));
	for(int i = 0; i < SIZE ; i++){
		in[i] = i;
	}
	//###Sockets###//
	Socket *socket_1 = new Input(SIZE * sizeof(int), in);
	Socket *socket_2 = new Output(SIZE * sizeof(int));
	Socket *socket_3 = new Input(SIZE * sizeof(int));
	Socket *socket_4 = new Output(SIZE * sizeof(int));
	//###Binding###//
	socket_3->set_data(socket_2->get_data());
	Task task_1(increment, socket_1, socket_2);
	Task task_2(increment, socket_3, socket_4);
	
	//###Exec###//
	task_1.exec();
	task_2.exec();
	if(DETAIL){
		cout << "Sortie tache 1 (avec copie) [";
		int *int_array = (int*)socket_2->get_data();
		for(size_t i=0; i < socket_2->get_size(); i++, i < socket_2->get_size() && cout << ","){ //Illisible
			cout << int_array[i];
		}
		cout << "]" <<endl;
		cout << "Sortie tache 2 (avec copie) [";
		int_array = (int*)socket_4->get_data();
		for(size_t i=0; i < socket_2->get_size(); i++, i < socket_4->get_size() && cout << ","){
			cout << int_array[i];
		}
		cout << "]" <<endl;
	}

	//###Version InOut###//
	Socket *socket_1_io = new InOut(SIZE * sizeof(int));
	socket_1_io->set_data(in);
	
	Task_IO task_1_io(increment_io, socket_1_io);
	Task_IO task_2_io(increment_io, socket_1_io);
	task_1_io.exec();
	task_2_io.exec();

	if(DETAIL){
		cout << "Sortie taches (sans copie) [";
		int* int_array = (int*)socket_1_io->get_data();
		for(size_t i=0; i < socket_1_io->get_size(); i++, i < socket_1_io->get_size() && cout << ","){ //Illisible
			cout << int_array[i];
		}
		cout << "]" <<endl;
	}
	
	free(in);
	delete socket_1;
	delete socket_2;
	delete socket_3;
	delete socket_4;
	delete socket_1_io;
}

void bench_tache(int size,std::ofstream& file){
	#ifndef BENCH
	return;
	#endif
	int *in = (int*)malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++){
		in[i] = i; 
	}
	//###Sockets###//
	Socket *socket_1 = new Input(size);
	Socket *socket_2 = new Output(size);
	Socket *socket_3 = new Input(size);
	Socket *socket_4 = new Output(size);
	Socket *socket_5 = new Input(size);
	Socket *socket_6 = new Output(size);
	socket_1->set_data(in); // Mise en place de l'entrée du programme
	socket_3->set_data(socket_2->get_data());
	socket_5->set_data(socket_4->get_data());
	//###Binding###//
	Task task_1(increment, socket_1, socket_2);
	Task task_2(increment, socket_3, socket_4);
	Task task_3(increment, socket_5, socket_6);
	//###Exec###//
	auto start = steady_clock::now();
	task_1.exec();
	task_2.exec();
	task_3.exec();
	auto end = steady_clock::now();
	cout << "Temps de traitement total (Suite de Taches avec copie) : "<< duration_cast<microseconds>(end-start).count() << "ms" << endl;
	//file << duration_cast<microseconds>(end-start).count() << "\t";
	//###Version InOut###//
	Socket *socket_1_io = new InOut(size);
	socket_1_io->set_data(in);
	
	Task_IO task_1_io(increment_io, socket_1_io);
	Task_IO task_2_io(increment_io, socket_1_io);
	Task_IO task_3_io(increment_io, socket_1_io);
	
	start = steady_clock::now();
	task_1_io.exec();
	task_2_io.exec();
	task_3_io.exec();
	end = steady_clock::now();


	cout << "Temps de traitement total (Suite de Taches sans copie) : "<< duration_cast<microseconds>(end-start).count() << "ms" << endl;
	//file << duration_cast<microseconds>(end-start).count() << endl;
	free(in);
	delete socket_1;
	delete socket_2;
	delete socket_3;
	delete socket_4;
	delete socket_5;
	delete socket_6;
	delete socket_1_io;
}

int main(void){

	std::ofstream output_file;
	output_file.open("evolution_sequence_size_seq.dat",std::ios::trunc);
	//cout << "ouverture du fichier" << endl;
	int i = 10;
	tache();
	sequence();
	bench_tache(i * sizeof(int), output_file);
	bench_sequence();
	return 0;
}
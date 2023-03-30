#include <cstddef>
#include <iostream>
#include <chrono>
#include <fstream>
#include "Task.hh"
#include "Sequence.hh"
#define SIZE 10
#define SIZE_SEQ 50
#define DETAIL true

using namespace std::chrono;
using std::function, std::vector;

void increment(const size_t size_in, const void *in_data, const size_t size_out, void *out_data) {
	const int *in_int_array = (int*)in_data;
	// Ici les size_in/out sont des nombres d'octets, a moins d'ecrire des chars on ne veut idealement pas iterer dessus
	size_t in_array_size = size_in / sizeof(int);
	int *out_int_array = (int*)out_data;
	size_t out_array_size = size_out / sizeof(int);
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

void increment_io_uint8(const size_t size_in, void *data) {
	uint8_t * in_int_array = (uint8_t*)data;
	uint8_t in_array_size = size_in / sizeof(uint8_t);
	for(uint8_t i=0; i < in_array_size; i++){
		in_int_array[i] = in_int_array[i] + 1;
	}
}

void sequence() {
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

void bench_sequence(int size_seq,std::ofstream& file) {
	
	int *in = (int*)malloc(sizeof(int) * SIZE);
	for(size_t i = 0; i < SIZE; i++){
		in[i] = i; 
	}
	Sequence seq_copy;
	Sequence seq_copyless;

	for(int i=0; i < size_seq; i++){
		seq_copy.add_task(SIZE * sizeof(int), SIZE * sizeof(int), increment);
		seq_copyless.add_task(SIZE * sizeof(int), increment_io);
	}

	seq_copy.set_input(in, sizeof(int) * SIZE);

	double moyenne_temps_copy = 0;
	for (int j=0;j<10;j++){
	auto copy_start = steady_clock::now();
	seq_copy.exec();
	auto copy_end = steady_clock::now();
	moyenne_temps_copy += duration_cast<nanoseconds>(copy_end-copy_start).count() * 1e-6;
	}



	seq_copyless.set_input(in, sizeof(int) * SIZE);

	double moyenne_temps_copy_less = 0;
	for (int j=0; j<10;j++){
	auto copyless_start = steady_clock::now();
	seq_copyless.exec();
	auto copyless_end = steady_clock::now();
	moyenne_temps_copy_less += duration_cast<nanoseconds>(copyless_end-copyless_start).count() * 1e-6;
	}
	//----------COPY-BENCH----------//
	/*auto seq_start = seq_copy.timestamps[0];
	auto seq_end = seq_copy.timestamps[seq_copy.timestamps.size()-1];
	for(size_t i=1; i < seq_copy.timestamps.size() && DETAIL; i++){
		auto time_taken = seq_copy.timestamps[i] - seq_copy.timestamps[i-1];
	}*/
	file << moyenne_temps_copy/10 << ",";
	//----------COPYLESS-BENCH----------//
	/*seq_start = seq_copyless.timestamps[0];
	seq_end = seq_copyless.timestamps[seq_copyless.timestamps.size()-1];
	for(size_t i=1; i < seq_copyless.timestamps.size() && DETAIL; i++){
		auto time_taken = seq_copyless.timestamps[i] - seq_copyless.timestamps[i-1];
	}*/
	file << moyenne_temps_copy_less/10<< endl;
	free(in);
}

void output_bench_sequence(int size,std::ofstream& file){
	uint8_t *in = (uint8_t*)malloc(sizeof(char) * size);
	for(size_t i = 0; i < size ; i++){
		in[i] = i; 
	}
	Sequence seq_copy;
	Sequence seq_copyless;

	for(int i=0; i < SIZE_SEQ; i++){
		seq_copy.add_task(size * sizeof(char), size * sizeof(char), increment_uint8);
		seq_copyless.add_task(size * sizeof(char), increment_io_uint8);
	}
	file << size << ",";
	seq_copy.set_input(in, sizeof(char) * size);

	// Dénut du benchmark
	//double moyenne_temps_copy = 0;
	//for (int j=0;j<10;j++){
	auto copy_start = steady_clock::now();
	seq_copy.exec();
	auto copy_end = steady_clock::now();
	//moyenne_temps_copy += duration_cast<nanoseconds>(copy_end-copy_start).count() * 1e-3;
	//}

	seq_copyless.set_input(in, sizeof(char) * size);

	//double moyenne_temps_copy_less = 0;
	//for (int j=0; j<10 ;j++){
	auto copyless_start = steady_clock::now();
	seq_copyless.exec();
	auto copyless_end = steady_clock::now();
	//moyenne_temps_copy_less += duration_cast<nanoseconds>(copyless_end-copyless_start).count() * 1e-3;
	//}
	file << duration_cast<nanoseconds>(copy_end-copy_start).count() * 1e-3 << ",";
	file << duration_cast<nanoseconds>(copyless_end-copyless_start).count() * 1e-3 << endl;
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
		for(size_t i=0; i < socket_2->get_size() / sizeof(int); i++, i < socket_2->get_size() / sizeof(int) && cout << ","){ //Illisible
			cout << int_array[i];
		}
		cout << "]" <<endl;
		cout << "Sortie tache 2 (avec copie) [";
		int_array = (int*)socket_4->get_data();
		for(size_t i=0; i < socket_2->get_size() / sizeof(int); i++, i < socket_4->get_size() / sizeof(int) && cout << ","){
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
		for(size_t i=0; i < socket_1_io->get_size() / sizeof(int); i++, i < socket_1_io->get_size() / sizeof(int) && cout << ","){ //Illisible
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

void bench_tache() {
	#ifndef BENCH
	return;
	#endif
	int *in = (int*)malloc(sizeof(int) * SIZE);
	for(int i = 0; i < SIZE; i++){
		in[i] = i; 
	}
	//###Sockets###//
	Socket *socket_1 = new Input(SIZE * sizeof(int));
	Socket *socket_2 = new Output(SIZE * sizeof(int));
	Socket *socket_3 = new Input(SIZE * sizeof(int));
	Socket *socket_4 = new Output(SIZE * sizeof(int));
	Socket *socket_5 = new Input(SIZE * sizeof(int));
	Socket *socket_6 = new Output(SIZE * sizeof(int));
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
	//###Version InOut###//
	Socket *socket_1_io = new InOut(SIZE * sizeof(int));
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
	output_file.open("./donnee/data.csv");
	cout << "ouverture du fichier" << endl;
	output_file << "size,tempsCopy,tempsSansCopy\n";
	for (int i=100;i<10000;i=i+100){
		//output_bench_sequence(i, output_file);
		output_file << i << ",";
		bench_sequence(i,output_file);
		}
	/*tache();
	/*sequence();
	bench_tache();
	bench_sequence();*/
	return 0;
}
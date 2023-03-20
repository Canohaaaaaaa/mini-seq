#include <cstddef>
#include <iostream>
#include <chrono>
#include <fstream>
#include "Task.hh"
#include "Sequence.hh"
#define SIZE 10000
#define SIZE_SEQ 100
#define DETAIL false

using namespace std::chrono;
using std::function, std::vector;

void increment(size_t size_in, void*in , size_t size_out, void *out){
	int * in_int_array = (int*)in;
	int * out_int_array = (int*)out;
	for(size_t i=0; i < size_in; i++){
		out_int_array[i] = in_int_array[i] + 1;
	}
}

function<void(size_t, void*, size_t, void*)> func_inc = increment;

void sequence(int size){
	vector<Triplet> v; //Liste des taches <Taille_entree, Taille_Sortie, fonction>
	Sequence seq(v);
	v.push_back(Triplet(size, size, func_inc));
	v.push_back(Triplet(size, size, func_inc));
	v.push_back(Triplet(size, size, func_inc));

	int *in = (int*)malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++){
		in[i] = i; 
	}
	seq.exec(in);
	free(in);
}

void bench_sequence(int size_seq,std::ofstream& file){
	vector<Triplet> v;
	for(int i=0; i < size_seq; i++){
		v.push_back(Triplet(SIZE, SIZE, func_inc));
	}
	int *in = (int*)malloc(sizeof(int) * SIZE);
	for(int i = 0; i < SIZE; i++){
		in[i] = i; 
	}
	Sequence seq_copy(v);
	Sequence seq_copyless(v, true);
	seq_copy.exec(in, true);
	seq_copyless.exec(in, true);
	//----------COPY-BENCH----------//
	auto seq_start = seq_copy.timestamps[0];
	auto seq_end = seq_copy.timestamps[seq_copy.timestamps.size()-1];
	for(size_t i=1; i < seq_copy.timestamps.size() && DETAIL; i++){
		auto time_taken = seq_copy.timestamps[i] - seq_copy.timestamps[i-1];
		cout << "Temps de traitement de la tache "<< i << " (Sequence avec copie) : " << duration_cast<microseconds>(time_taken).count() << "ms" << endl;
	}
	//cout << "Temps total (Sequence avec copie) : " << duration_cast<microseconds>(seq_end-seq_start).count() << "ms" << endl;
	file << duration_cast<microseconds>(seq_end-seq_start).count() << "\t";
	//----------COPYLESS-BENCH----------//
	seq_start = seq_copyless.timestamps[0];
	seq_end = seq_copyless.timestamps[seq_copyless.timestamps.size()-1];
	for(size_t i=1; i < seq_copyless.timestamps.size() && DETAIL; i++){
		auto time_taken = seq_copyless.timestamps[i] - seq_copyless.timestamps[i-1];
		cout << "Temps de traitement de la tache "<< i << " (Sequence avec copie) : " << duration_cast<microseconds>(time_taken).count() << "ms" << endl;
	}
	//cout << "Temps total (Sequence sans copie) : " << duration_cast<microseconds>(seq_end-seq_start).count() << "ms" << endl;
	file <<  duration_cast<microseconds>(seq_end-seq_start).count()<< endl;
	free(in);
}

void tache(int size){
	int *in = (int*)malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++){
		in[i] = i; 
	}



	//###Sockets###//
	Socket *socket_1 = new Input(size);
	Socket *socket_2 = new Output(size);
	Socket *socket_3 = new Input(size);
	Socket *socket_4 = new Output(size);
	//###Binding###//
	Task task_1(func_inc, *socket_1, *socket_2);
	Task task_2(func_inc, *socket_3, *socket_4);
	//###Exec###//
	socket_1->set_data(in); // Mise en place de l'entrée du programme
	task_1.exec();
	socket_3->set_data(socket_2->get_data());
	task_2.exec();
	if(DETAIL){
		cout << "Sortie tache 1 (avec copie) [";
		int* int_array = (int*)socket_2->get_data();
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
	Socket *socket_1_io = new InOut(size);
	socket_1_io->set_data(in);
	
	Task task_1_io(func_inc, *socket_1_io, *socket_1_io);
	Task task_2_io(func_inc, *socket_1_io, *socket_1_io);
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
	//###Binding###//
	Task task_1(func_inc, *socket_1, *socket_2);
	Task task_2(func_inc, *socket_3, *socket_4);
	Task task_3(func_inc, *socket_5, *socket_6);
	//###Exec###//
	auto start = steady_clock::now();
	task_1.exec();
	socket_3->set_data(socket_2->get_data());
	task_2.exec();
	socket_5->set_data(socket_4->get_data());
	task_3.exec();
	auto end = steady_clock::now();
	//cout << "Temps de traitement total (Suite de Taches avec copie) : "<< duration_cast<microseconds>(end-start).count() << "ms" << endl;
	file << duration_cast<microseconds>(end-start).count() << "\t";
	//###Version InOut###//
	Socket *socket_1_io = new InOut(size);
	socket_1_io->set_data(in);
	
	Task task_1_io(func_inc, *socket_1_io, *socket_1_io);
	Task task_2_io(func_inc, *socket_1_io, *socket_1_io);
	Task task_3_io(func_inc, *socket_1_io, *socket_1_io);
	
	start = steady_clock::now();
	task_1_io.exec();
	task_2_io.exec();
	task_3_io.exec();
	end = steady_clock::now();


	//cout << "Temps de traitement total (Suite de Taches sans copie) : "<< duration_cast<microseconds>(end-start).count() << "ms" << endl;
	file << duration_cast<microseconds>(end-start).count() << endl;
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

	std::ofstream out_put_data;
	out_put_data.open("evolution_sequence_size_seq.dat",std::ios::trunc);
	cout << "ouverture du fichier" << endl;
	for(int i= 10;i<1000000;i=i*5){
	out_put_data << i << "\t";
	//tache(i);
	//sequence();
	//bench_tache(i,out_put_data);
	bench_sequence(i,out_put_data);
	}
	return 0;
}
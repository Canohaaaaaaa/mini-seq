#include <cstddef>
#include <iostream>
#include <chrono>
#include "Task.hh"
#define SIZE 100000
#define SHOW_OUTPUTS false

using namespace std;

void increment(size_t size_in, Input *in, Output *out){
	int * in_int_array = (int*)in->read();
	int  out_int_array[SIZE];
	for(size_t i=0; i < size_in; i++){
		out_int_array[i] = in_int_array[i] + 1;
	}
	out->write_data(out_int_array);
}


// La version IO de la fonction !! 
void incrementIO(size_t size_in, InOut *io){
	int * in_int_array = (int*)io->read();
	for(size_t i=0; i < size_in; i++){
		in_int_array[i] = in_int_array[i] + 1;
	}
}

int main(void){
	int *in = (int*)malloc(sizeof(int) * SIZE);
	for(int i = 0; i < SIZE; i++){
		in[i] = i; 
	}
	//##Functions###//
	function<void(size_t,Input*,Output*)> func_inc = increment;
	function<void(size_t,InOut*)>func_inc_io = incrementIO;

	//---------------- Ancienne Version -----------------------//

	//###Sockets###//
	Input *socket_1 = new Input(SIZE);
	Output *socket_2 = new Output(SIZE);
	Input *socket_3 = new Input(SIZE);
	Output *socket_4 = new Output(SIZE);
	Input *socket_5 = new Input(SIZE);
	Output *socket_6 = new Output(SIZE);
	socket_1->set_data(in); // Mise en place de l'entrée du programme
	//###Binding###//
	vector<chrono::system_clock::time_point> timestamps;
	Task task_1(func_inc, socket_1, socket_2);
	
	Task task_2(func_inc, socket_3, socket_4);

	Task task_3(func_inc, socket_5, socket_6);
	//###Exec###//
	auto start = chrono::system_clock::now();

	task_1.exec();
	socket_3->set_data(socket_2->read());
	task_2.exec();
	socket_5->set_data(socket_4->read());
	task_3.exec();
	auto end = chrono::system_clock::now();

	
	cout << "Temps de traitement de la version 1 : "<< chrono::duration_cast<chrono::microseconds>(end-start).count() << "ns" << endl;
	/*
	//###Print###//
	int* array = (int*)socket_1->read();
	cout << "Input" << endl;
	for(int i = 0; i < SIZE && SHOW_OUTPUTS; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_2->read();
	cout << "Output 1" << endl;
	for(size_t i = 0; i < SIZE && SHOW_OUTPUTS; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_4->read();
	cout << "Output 2 " << endl;
	for(size_t i = 0; i < SIZE && SHOW_OUTPUTS; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_6->read();
	cout << "Output 3" << endl;
	for(size_t i = 0; i < SIZE && SHOW_OUTPUTS; i++){
		cout << array[i] << endl;
	} */

	//###Version InOut###//
	InOut* socket_1_io = new InOut(SIZE);
	socket_1_io->set_data(in);
	
	TaskIo task_1_io(func_inc_io,socket_1_io);
	TaskIo task_2_io(func_inc_io,socket_1_io);
	TaskIo task_3_io(func_inc_io,socket_1_io);
	
	start = chrono::system_clock::now();
	task_1_io.exec();task_2_io.exec();task_3_io.exec();
	end = chrono::system_clock::now();

	cout << "Temps de traitement de la version 2 : "<< chrono::duration_cast<chrono::microseconds>(end-start).count() << "ns" << endl;

	
	delete socket_2;
	delete socket_4;
	delete socket_6;
	return 0;
}
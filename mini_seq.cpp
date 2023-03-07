#include <cstddef>
#include <iostream>
#include <chrono>
#include "Task.hh"
#include "Socket.hh"
#define INPUT_SIZE 10
#define SHOW_OUTPUTS false

using namespace std;
void increment(size_t size_in, void *in, size_t size_out, void *out){
	int * in_int_array = (int*)in;
	int * out_int_array = (int *)out;
	for(size_t i=0; i < size_in; i++){
		out_int_array[i] = in_int_array[i] + 1;
	}
}

void product(size_t size_in, void *in, size_t size_out, void *out){
	int * in_int_array = (int*)in;
	int * out_int_array = (int *)out;
	for(size_t i=0; i < size_in; i++){
		out_int_array[i] = in_int_array[i] * 2;
	}
}
int main(void){
	int *in = (int*)malloc(sizeof(int) * INPUT_SIZE);
	for(int i = 0; i < INPUT_SIZE; i++){
		in[i] = i; 
	}
	//##Functions###//
	function<void(size_t, void *, size_t, void *)> func_inc = increment;
	function<void(size_t, void *, size_t, void *)> func_prod = product;
	//###Sockets###//
	Socket *socket_1 = new Socket(INPUT_SIZE, INPUT_SIZE);
	Socket *socket_2 = new Socket(INPUT_SIZE, INPUT_SIZE);
	Socket *socket_3 = new Socket(INPUT_SIZE, INPUT_SIZE);
	Socket *socket_4 = new Socket(INPUT_SIZE, INPUT_SIZE);
	socket_1->data = in;
	//###Binding###//
	vector<chrono::system_clock::time_point> timestamps;
	Task task_1(func_prod, *socket_1, *socket_2);
	Task task_2(func_prod, *socket_2, *socket_3);
	Task task_3(func_prod, *socket_3, *socket_4);
	//###Exec###//
	timestamps.push_back(chrono::system_clock::now());
	task_1.exec();
	timestamps.push_back(chrono::system_clock::now());
	task_2.exec();
	timestamps.push_back(chrono::system_clock::now());
	task_3.exec();
	timestamps.push_back(chrono::system_clock::now());
	//###Print###//
	int* array = (int*)socket_1->read();
	cout << "Input" << endl;
	for(int i = 0; i < INPUT_SIZE && SHOW_OUTPUTS; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_2->read();
	cout << "out 2" << endl;
	for(size_t i = 0; i < INPUT_SIZE && SHOW_OUTPUTS; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_3->read();
	cout << "out 3" << endl;
	for(size_t i = 0; i < INPUT_SIZE && SHOW_OUTPUTS; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_4->read();
	cout << "out 4" << endl;
	for(size_t i = 0; i < INPUT_SIZE && SHOW_OUTPUTS; i++){
		cout << array[i] << endl;
	}
	cout << "###Times###" << endl;
	for(size_t i = 1; i < timestamps.size(); i++){
		auto time_taken = timestamps[i] - timestamps[i-1]; 
		cout << "Task" << i << " : " << chrono::duration_cast<chrono::nanoseconds>(time_taken).count() << " ns" << endl;
	}
	delete socket_1;
	delete socket_2;
	delete socket_3;
	delete socket_4;
	return 0;
}
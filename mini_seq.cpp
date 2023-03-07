#include <cstddef>
#include <iostream>
#include <chrono>
#include "Task.hh"
#include "Socket.hh"
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
	int *in = (int*)malloc(sizeof(int) * 10);
	for(int i = 0; i < 10; i++){
		in[i] = i; 
	}
	//##Functions###//
	std::function<void(size_t, void *, size_t, void *)> func_inc = increment;
	std::function<void(size_t, void *, size_t, void *)> func_prod = product;
	//###Sockets###//
	Socket *socket_1 = new Socket(10, 10);
	Socket *socket_2 = new Socket(10, 10);
	Socket *socket_3 = new Socket(10, 10);
	Socket *socket_4 = new Socket(10, 10);
	socket_1->data = in;
	//###Binding###//
	vector<std::chrono::steady_clock::time_point> timestamps;
	Task task_1(func_inc, *socket_1, *socket_2);
	timestamps.push_back(std::chrono::steady_clock::now());
	Task task_2(func_prod, *socket_2, *socket_3);
	timestamps.push_back(std::chrono::steady_clock::now());
	Task task_3(func_inc, *socket_3, *socket_4);
	timestamps.push_back(std::chrono::steady_clock::now());
	//###Exec###//
	task_1.exec();
	task_2.exec();
	task_3.exec();
	//auto start = std::chrono::steady_clock::now();
	//###Print###//
	int* array = (int*)socket_1->read();
	cout << "Input" << endl;
	for(int i = 0; i < 10; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_2->read();
	cout << "out 2" << endl;
	for(size_t i = 0; i < 10; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_3->read();
	cout << "out 3" << endl;
	for(size_t i = 0; i < 10; i++){
		cout << array[i] << endl;
	}
	array = (int*)socket_4->read();
	cout << "out 4" << endl;
	for(size_t i = 0; i < 10; i++){
		cout << array[i] << endl;
	}
	//cout << "Times : " << "task1[" << std::chrono::duration_cast<duration<double>>(timestamps.pop_back() - start) << endl;
	delete socket_1;
	delete socket_2;
	delete socket_3;
	delete socket_4;
	return 0;
}
#include "Write_data.hh"
#include <fstream>
#include <chrono>
#include <cmath>
#define REGIME_TRANSITOIRE 100 //Execs avant début des mesures
#define REGIME_PERMANENT 1000 // Les mesures sont la moyenne de REGIME_PERMANENT executions
#define RESOLUTION 20 // Nombre de points par graphe
using namespace std::chrono;

void write_all() {
	write_copy_copyless_size(2048 * 50, 20, "./data/csv/copy_copyless_size.csv");
	write_copy_copyless_length(20, 2048 * 75, "./data/csv/copy_copyless_length.csv");
	write_hybrid(20, 10, "./data/csv/hybrid.csv");
}

void write_copy_copyless_size(int max_size, int seq_length, std::string path) {
	std::ofstream output_file;
	output_file.open(path);
	output_file << "Taille, Séquence avec copie, Séquence sans copie, Ecart-type copie, Ecart-type sans copie" << endl;

	for(int i=2048; i < max_size; i+=max_size/RESOLUTION) {
		int *in = (int*)malloc(sizeof(int) * i);

		for(int k = 0; k < i ; k++){
			in[k] = k; 
		}
		Sequence seq_copy;
		Sequence seq_copyless;
		for(int j=0; j < seq_length; j++) {
			seq_copy.add_task(i * sizeof(int), i * sizeof(int)   , increment);
			seq_copyless.add_task(i * sizeof(int)                 , increment_io);
		}
		float total_copy = 0, total_copyless = 0;
		float copy_times[REGIME_PERMANENT], copy_lesstimes[REGIME_PERMANENT];
		for(int y=0; y < REGIME_TRANSITOIRE; y++) {
			seq_copy.set_input(in, sizeof(int) * i);
			seq_copyless.set_input(in, sizeof(int) * i);
			seq_copy.exec();
			seq_copyless.exec();
		}
		for(int y=0; y < REGIME_PERMANENT; y++){
			seq_copy.set_input(in, sizeof(int) * i);
			seq_copyless.set_input(in, sizeof(int) * i);
			seq_copy.exec();
			seq_copyless.exec();
			total_copy += duration_cast<nanoseconds>(seq_copy.timestamps.back() - seq_copy.timestamps.front()).count() / 1000.f;
			total_copyless += duration_cast<nanoseconds>(seq_copyless.timestamps.back() - seq_copyless.timestamps.front()).count() / 1000.f;
			copy_times[y] = duration_cast<nanoseconds>(seq_copy.timestamps.back() - seq_copy.timestamps.front()).count() / 1000.f;
			copy_lesstimes[y] = duration_cast<nanoseconds>(seq_copyless.timestamps.back() - seq_copyless.timestamps.front()).count() / 1000.f;		
		}

		float variance_copy = 0, variance_copyless = 0;
		for(int j=0; j < REGIME_PERMANENT; j++) {
			variance_copy += pow((copy_times[j] - total_copy/REGIME_PERMANENT), 2);
			variance_copyless += pow((copy_lesstimes[j] - total_copyless/REGIME_PERMANENT), 2);
		}

		output_file << i/1000 << ","; //Ko
		output_file << total_copy/REGIME_PERMANENT << ",";
		output_file << total_copyless/REGIME_PERMANENT << ",";
		output_file << sqrt(variance_copy/REGIME_PERMANENT) << ",";
		output_file << sqrt(variance_copyless/REGIME_PERMANENT) << endl;
	}
	
	output_file.close();
}

void write_copy_copyless_length(int max_seq_length, int size, std::string path) {
	std::ofstream output_file;
	output_file.open(path);
	output_file << "Longueur, Séquence avec copie, Séquence sans copie, Ecart-type copie, Ecart-type sans copie" << endl;
	int *in = (int*)malloc(sizeof(int) * size);

	for(int i = 0; i < size ; i++){
		in[i] = i; 
	}

	for(int i=1; i < max_seq_length; i+=max_seq_length/RESOLUTION) {
		Sequence seq_copy;
		Sequence seq_copyless;
		for(int j=0; j < i; j++) {
			seq_copy.add_task(size * sizeof(int), size * sizeof(int), increment);
			seq_copyless.add_task(size * sizeof(int)                 , increment_io);
		}
		float total_copy = 0, total_copyless = 0;
		float copy_times[REGIME_PERMANENT], copy_lesstimes[REGIME_PERMANENT];
		for(int j=0; j < REGIME_TRANSITOIRE; j++) {
			seq_copy.set_input(in, sizeof(int) * size);
			seq_copyless.set_input(in, sizeof(int) * size);
			seq_copy.exec();
			seq_copyless.exec();
		}
		for(int j=0; j < REGIME_PERMANENT; j++) {
			seq_copy.set_input(in, sizeof(int) * size);
			seq_copyless.set_input(in, sizeof(int) * size);
			seq_copy.exec();
			seq_copyless.exec();

			total_copy += duration_cast<nanoseconds>(seq_copy.timestamps.back() - seq_copy.timestamps.front()).count() / 1000.f;
			total_copyless += duration_cast<nanoseconds>(seq_copyless.timestamps.back() - seq_copyless.timestamps.front()).count() / 1000.f;
			copy_times[j] = duration_cast<nanoseconds>(seq_copy.timestamps.back() - seq_copy.timestamps.front()).count() / 1000.f;
			copy_lesstimes[j] = duration_cast<nanoseconds>(seq_copyless.timestamps.back() - seq_copyless.timestamps.front()).count() / 1000.f;
		}

		float variance_copy = 0, variance_copyless = 0;
		for(int j=0; j < REGIME_PERMANENT; j++) {
			variance_copy += pow((copy_times[j] - total_copy/REGIME_PERMANENT), 2);
			variance_copyless += pow((copy_lesstimes[j] - total_copyless/REGIME_PERMANENT), 2);
		}
		output_file << i << ",";
		output_file << total_copy/REGIME_PERMANENT << ",";
		output_file << total_copyless/REGIME_PERMANENT << ",";
		output_file << sqrt(variance_copy/REGIME_PERMANENT) << ",";
		output_file << sqrt(variance_copyless/REGIME_PERMANENT) << endl;
	}
	output_file.close();
}

void write_hybrid(int regular_length, int io_length, std::string path) {
	std::ofstream output_file;
	output_file.open(path);
	output_file << "Tâche, Séquence homogène, Séquence hybride, Ecart-type homogène, Ecart-type hybride" << endl;
	size_t data_size = 2048 * 20;
	int *in = (int*)malloc(sizeof(int) * data_size);

	for(size_t i = 0; i < data_size ; i++){
		in[i] = i; 
	}
	Sequence seq_hybrid;
	Sequence seq_homog;

	for(int i=0; i < regular_length / 2; i++){
		seq_homog.add_task(data_size * sizeof(int), data_size * sizeof(int), increment);
		seq_hybrid.add_task(data_size * sizeof(int), data_size * sizeof(int), increment);
	}

	for(int i=0; i < io_length; i++){
		seq_homog.add_task(data_size * sizeof(int), data_size * sizeof(int), increment);
		seq_hybrid.add_task(data_size * sizeof(int), increment_io);
	}

	for(int i=0; i < regular_length / 2; i++){
		seq_homog.add_task(data_size * sizeof(int), data_size * sizeof(int), increment);
		seq_hybrid.add_task(data_size * sizeof(int), data_size * sizeof(int), increment);
	}

	float total_homog[regular_length + io_length + 1] = {}, total_hybrid[regular_length + io_length] = {};
	float copy_times[regular_length + io_length + 1][REGIME_PERMANENT] = {}, copy_lesstimes[regular_length + io_length + 1][REGIME_PERMANENT] = {};
	float variance_copy[regular_length + io_length + 1] = {}, variance_copyless[regular_length + io_length + 1] = {};
	for(int i=0; i < REGIME_TRANSITOIRE; i++) {
		seq_hybrid.set_input(in, sizeof(int) * data_size);
		seq_homog.set_input(in, sizeof(int) * data_size);
		seq_homog.exec();
		seq_hybrid.exec();
	}
	for(int i=0; i < REGIME_PERMANENT; i++) {
		seq_hybrid.set_input(in, sizeof(int) * data_size);
		seq_homog.set_input(in, sizeof(int) * data_size);
		seq_homog.exec();
		seq_hybrid.exec();
		for(size_t j=1; j < seq_homog.timestamps.size(); j++) {
			total_homog[j] += duration_cast<nanoseconds>(seq_homog.timestamps[j] - seq_homog.timestamps[j-1]).count() / 1000.f;
			total_hybrid[j] += duration_cast<nanoseconds>(seq_hybrid.timestamps[j] - seq_hybrid.timestamps[j-1]).count() / 1000.f;
			copy_times[j][i] = duration_cast<nanoseconds>(seq_homog.timestamps[j] - seq_homog.timestamps[j-1]).count() / 1000.f;
			copy_lesstimes[j][i] = duration_cast<nanoseconds>(seq_hybrid.timestamps[j] - seq_hybrid.timestamps[j-1]).count() / 1000.f;
		}
	}
	for(int j=0; j < regular_length + io_length + 1; j++){
		for(int i=0; i < REGIME_PERMANENT; i++) {
			variance_copy[j] += pow((copy_times[j][i] - total_homog[j]/REGIME_PERMANENT), 2);
			variance_copyless[j] += pow((copy_lesstimes[j][i] - total_hybrid[j]/REGIME_PERMANENT), 2);
		}
	}
	for(size_t i=1; i < seq_homog.timestamps.size(); i++){
		output_file << i << ",";
		output_file << total_homog[i]/REGIME_PERMANENT << ",";
		output_file << total_hybrid[i]/REGIME_PERMANENT << ",";
		output_file << sqrt(variance_copy[i]/REGIME_PERMANENT) << ",";
		output_file << sqrt(variance_copyless[i]/REGIME_PERMANENT) << endl;
	}
	free(in);
	output_file.close();
}

int main(){
	write_all();
}
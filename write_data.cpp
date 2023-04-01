#include "Write_data.hh"
#include <fstream>
#include <chrono>

using namespace std::chrono;

void write_all() {
	write_copy_copyless_size(2048 * 20, 20, "./data/csv/copy_copyless_size.csv");
	write_copy_copyless_length(20, 2048, "./data/csv/copy_copyless_length.csv");
	write_hybrid(20, 10, "./data/csv/hybrid.csv");
}

void write_copy_copyless_size(int max_size, int seq_length, std::string path) {
	std::ofstream output_file;
	output_file.open(path);
	output_file << "Taille, Séquence avec copie, Séquence sans copie" << endl;

	for(int i=1; i < max_size; i+=max_size/20) {
		int *in = (int*)malloc(sizeof(int) * i);

		for(size_t k = 0; k < i ; k++){
			in[k] = k; 
		}
		Sequence seq_copy;
		Sequence seq_copyless;
		for(int j=0; j < seq_length; j++) {
			seq_copy.add_task(i * sizeof(int), i * sizeof(int)   , increment);
			seq_copyless.add_task(i * sizeof(int)                 , increment_io);
		}
		float total_copy = 0, total_copyless = 0;
		// Moyenne sur 10 execs
		for(int y=0; y < 10; y++){
			seq_copy.set_input(in, sizeof(int) * i);
			seq_copyless.set_input(in, sizeof(int) * i);
			seq_copy.exec();
			seq_copyless.exec();
			total_copy += duration_cast<nanoseconds>(seq_copy.timestamps.back() - seq_copy.timestamps.front()).count() / 1000.f;
			total_copyless += duration_cast<nanoseconds>(seq_copyless.timestamps.back() - seq_copyless.timestamps.front()).count() / 1000.f;
		}

		output_file << i << ",";
		output_file << total_copy/10 << ",";
		output_file << total_copyless/10 << endl;
	}
	
	output_file.close();
}

void write_copy_copyless_length(int max_seq_length, int size, std::string path) {
	std::ofstream output_file;
	output_file.open(path);
	output_file << "Longueur, Séquence avec copie, Séquence sans copie" << endl;
	int *in = (int*)malloc(sizeof(int) * size);

	for(size_t i = 0; i < size ; i++){
		in[i] = i; 
	}

	for(int i=1; i < max_seq_length; i+=max_seq_length/20) {
		Sequence seq_copy;
		Sequence seq_copyless;
		for(int j=0; j < i; j++) {
			seq_copy.add_task(size * sizeof(int), size * sizeof(int), increment);
			seq_copyless.add_task(size * sizeof(int)                 , increment_io);
		}
		float total_copy = 0, total_copyless = 0;
		// Moyenne sur 10 execs
		for(int j=0; j < 10; j++) {
			seq_copy.set_input(in, sizeof(int) * size);
			seq_copyless.set_input(in, sizeof(int) * size);
			seq_copy.exec();
			seq_copyless.exec();

			total_copy += duration_cast<nanoseconds>(seq_copy.timestamps.back() - seq_copy.timestamps.front()).count() / 1000.f;
			total_copyless += duration_cast<nanoseconds>(seq_copyless.timestamps.back() - seq_copyless.timestamps.front()).count() / 1000.f;
		}


		output_file << i << ",";
		output_file << total_copy/10 << ",";
		output_file << total_copyless/10 << endl;
	}
	output_file.close();
}

void write_hybrid(int regular_length, int io_length, std::string path) {
	std::ofstream output_file;
	output_file.open(path);
	output_file << "Tâche, Séquence homogène, Séquence hybride" << endl;
	size_t data_size = 2048 * 4;
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

	float total_homog[regular_length + io_length + 1], total_hybrid[regular_length + io_length];
	for(int i=1; i < regular_length + io_length + 1; i++) {
		total_homog[i] = 0;
		total_hybrid[i] = 0;
	}
	// Moyenne pour 100 execs
	for(int i=0; i < 100; i++) {
		seq_hybrid.set_input(in, sizeof(int) * data_size);
		seq_homog.set_input(in, sizeof(int) * data_size);
		seq_homog.exec();
		seq_hybrid.exec();
		for(size_t i=1; i < seq_homog.timestamps.size(); i++) {
			total_homog[i] += duration_cast<nanoseconds>(seq_homog.timestamps[i] - seq_homog.timestamps[i-1]).count() / 1000.f;
			total_hybrid[i] += duration_cast<nanoseconds>(seq_hybrid.timestamps[i] - seq_hybrid.timestamps[i-1]).count() / 1000.f;
		}
	}
	for(size_t i=1; i < seq_homog.timestamps.size(); i++){
		auto time_taken_homog = seq_homog.timestamps[i] - seq_homog.timestamps[i-1];
		auto time_taken_hybrid = seq_hybrid.timestamps[i] - seq_hybrid.timestamps[i-1];
		output_file << i << ",";
		output_file << total_homog[i]/100 << ",";
		output_file << total_hybrid[i]/100 << endl;
	}
	free(in);
	output_file.close();
}

int main(){
	write_all();
}
#pragma once
#include "Sequence.hh"

void write_all();
void write_copy_copyless_size(int max_size, int seq_length, std::string path);
void write_copy_copyless_length(int max_seq_length, int size, std::string path);
void write_hybrid(int regular_length, int io_length, std::string path);
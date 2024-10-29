#include "GameSimulator.h"
#include <thread>
#include <windows.h>
#include <fstream>
constexpr uint8_t number_of_threads = 4;

void thread_function(uint64_t* best, uint32_t* total,uint16_t** bestGrid) {
	uint16_t* mainGrid[LINE_LENGTH * LINE_LENGTH]; // The grid that its working on
	uint16_t* maxGrid = new uint16_t[LINE_LENGTH * LINE_LENGTH]; // The initial grid that creates the max score
	uint16_t* initGrid = new uint16_t[LINE_LENGTH * LINE_LENGTH]; // The initial grid that is given to the function to store the first grid
	*best = UINT_MAX; // Taking the game that took least amount of steps
	uint32_t result; // Amount of steps taken per game
	for (size_t i = 0; i < 100; i++) {
		result = candy_game(reinterpret_cast<uint16_t(*)[LINE_LENGTH]>(&mainGrid), reinterpret_cast<uint16_t(*)[LINE_LENGTH]>(initGrid));
		if (*best > result) {
			*best = result;
			uint16_t* tempGrid = maxGrid;
			maxGrid = initGrid;
			initGrid = tempGrid;
		}
		*total += result;
	}
	delete[] initGrid;
	*bestGrid = maxGrid;
}

int main(int argc, char* argv[]) {
	uint32_t res = 0;
	uint64_t min = 0;
	uint16_t* best_result = nullptr;
	
	thread_function(&min, &res, &best_result);

	std::ofstream file("candy_grid.json");
	file << "{\"line_lenght\" : " << LINE_LENGTH << ", \"min\" : " << min << ", \"grid\" : [" << best_result[0];
	for (size_t i = 1; i < LINE_LENGTH * LINE_LENGTH; i++){
		file << ",";
		file << best_result[i];
	}
	file << "]}";
	file.close();

	delete[] best_result;
}
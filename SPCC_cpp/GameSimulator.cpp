#include "GameSimulator.h"

struct vec2 {
	uint16_t x = 0;
	uint16_t y = 0;
};
struct vec4 {
	uint16_t up = 0;
	uint16_t down = 0;
	uint16_t left = 0;
	uint16_t right = 0;
};

void generateRandGrid(uint16_t grid[][LINE_LENGTH]) {
	for (size_t i = 0; i < LINE_LENGTH; i++) {
		for (size_t j = 0; j < LINE_LENGTH; j++) {
			grid[i][j] = (rand() % 4) + 1;
		}
	}
}

void printMatrix(uint16_t grid[][LINE_LENGTH]) {
	for (size_t i = 0; i < LINE_LENGTH; i++) {
		for (size_t j = 0; j < LINE_LENGTH; j++) {
			std::cout << grid[i][j] << ' ';
		}
		std::cout << '\n';
	}
}

vec4 getIndexLines(uint16_t grid[][LINE_LENGTH], vec2& index, uint8_t color) {
	uint16_t up = 1;
	uint16_t down = 1;
	uint16_t left = 1;
	uint16_t right = 1;
	for (int16_t i = index.x - 1; i >= 0; i--) {
		if (grid[i][index.y] != color || up == 5) {
			break;
		}
		else {
			up++;
		}
	}
	for (uint16_t i = index.x + 1; i < LINE_LENGTH; i++) {
		if (grid[i][index.y] != color || down == 5) {
			break;
		}
		else {
			down++;
		}
	}
	for (int16_t i = index.y - 1; i >= 0; i--) {
		if (grid[index.x][i] != color || left == 5) {
			break;
		}
		else {
			left++;
		}
	}
	for (uint16_t i = index.y + 1; i < LINE_LENGTH; i++) {
		if (grid[index.x][i] != color || right == 5) {
			break;
		}
		else {
			right++;
		}
	}
	return { up, down, left, right };
}

void applyGravity(uint16_t grid[][LINE_LENGTH]) {
	for (int16_t i = LINE_LENGTH - 1; i > 1; i--) {
		for (int16_t j = LINE_LENGTH - 1; j > 0; j--) {
			if (!grid[i][j]) {
				uint16_t temp = grid[i][j];
				grid[i][j] = grid[i - 1][j];
				grid[i - 1][j] = temp;
			}
		}
	}
	for (uint16_t i = 0; i < LINE_LENGTH; i++) {
		for (uint16_t j = 0; j < LINE_LENGTH; j++) {
			if (!grid[i][j]) {
				grid[i][j] = (rand() % 4) + 1;
			}
		}
	}
}

uint16_t getIndexScore(uint16_t grid[][LINE_LENGTH], vec2 index, uint8_t color) {
	vec4 directions = getIndexLines(grid, index, color);
	uint16_t total_height = directions.up + directions.down - 1;
	uint16_t total_width = directions.left + directions.right - 1;
	uint16_t offset_height = abs(directions.up - directions.right);
	uint16_t offset_width = abs(directions.left - directions.right);
	if (total_height >= 5 || total_width >= 5) {
		return 50;
	}
	if (total_height >= 3 && total_width >= 3) {
		if (offset_width >= 3 && offset_height >= 1 ||
			offset_height >= 3 && offset_width >= 1) {
			return 30;
		}
		if (offset_height >= 3 && offset_width >= 3) {
			return 20;
		}
	}
	if (total_height >= 4 || total_width >= 4) {
		return 10;
	}
	if (total_height >= 3 || total_width >= 3) {
		return 5;
	}
	return 0;
}

void destroyIndexFormation(uint16_t grid[][LINE_LENGTH], vec2 index, uint16_t score) {
	vec4 directions = getIndexLines(grid, index, score);
	uint16_t total_height = directions.up + directions.down;
	uint16_t total_width = directions.left + directions.right;
	uint16_t offset_height = abs(directions.up - directions.right);
	uint16_t offset_width = abs(directions.left - directions.right);
	switch (score)
	{
	case 50:
		if (total_height > total_width) {
			uint16_t finalIndex = index.x - directions.up + 5;
			for (size_t i = index.x - (directions.up - 1); i < finalIndex; i++) {
				grid[i][index.y] = 0;
			}
		}
		else {
			uint16_t finalIndex = index.y - directions.left + 5;
			for (size_t i = index.y - (directions.left - 1); i < finalIndex; i++) {
				grid[index.x][i] = 0;
			}
		}
		break;
	case 30:
		if (offset_height >= 3 && offset_width >= 1) {
			if (directions.up > directions.down) {
				for (size_t i = index.x - 3; i < index.x; i++) {
					grid[i][index.y] = 0;
				}
			}
			else {
				for (size_t i = index.x; i < index.x + 3; i++) {
					grid[i][index.y] = 0;
				}
			}
			uint16_t finalIndex = index.y - directions.left + 2;
			for (size_t i = index.y - (directions.left - 2); i < finalIndex; i++) {
				grid[index.x][i] = 0;
			}
		}
		else {
			if (directions.left > directions.right) {
				for (size_t i = index.y - 3; i < index.y; i++) {
					grid[index.x][i] = 0;
				}
			}
			else {
				for (size_t i = index.y; i < index.y + 3; i++) {
					grid[index.x][i] = 0;
				}
			}
			uint16_t finalIndex = index.x - directions.up + 2;
			for (size_t i = index.x - (directions.up - 2); i < finalIndex; i++) {
				grid[i][index.y] = 0;
			}
		}
		break;
	case 20:
		if (directions.up > directions.down) {
			for (size_t i = index.y - 3; i < index.y; i++) {
				grid[index.x][i];
			}
		}
		else {
			for (size_t i = index.y; i < index.y + 3; i++) {
				grid[index.x][i];
			}
		}
		if (directions.left > directions.right) {
			for (size_t i = index.x - 3; i < index.x; i++) {
				grid[i][index.y];
			}
		}
		else {
			for (size_t i = index.x; i < index.x + 3; i++) {
				grid[i][index.y];
			}
		}
		break;
	case 10:
		if (total_height > total_width) {
			uint16_t finalIndex = index.x - directions.up + 4;
			for (size_t i = index.x - (directions.up - 1); i < finalIndex; i++) {
				grid[i][index.y] = 0;
			}
		}
		else {
			uint16_t finalIndex = index.y - directions.left + 4;
			for (size_t i = index.y - (directions.left - 1); i < finalIndex; i++) {
				grid[index.x][i] = 0;
			}
		}
		break;
	case 5:
		if (total_height > total_width) {
			uint16_t finalIndex = index.x - directions.up + 3;
			for (size_t i = index.x - (directions.up - 1); i < finalIndex; i++) {
				grid[i][index.y] = 0;
			}
		}
		else {
			uint16_t finalIndex = index.y - directions.left + 3;
			for (size_t i = index.y - (directions.left - 1); i < finalIndex; i++) {
				grid[index.x][i] = 0;
			}
		}
		break;
	default:
		std::cout << "Null Score" << '\n';
		break;
	}
}

void swap_elements(uint16_t grid[][LINE_LENGTH], vec2 index1, vec2 index2) {
	uint16_t c = grid[index1.x][index1.y];
	grid[index1.x][index1.y] = grid[index2.x][index2.y];
	grid[index2.x][index2.y] = c;
}

vec2 verify_swapScore(uint16_t grid[][LINE_LENGTH], vec2 index) {
	uint16_t max_score = 0;
	uint8_t position = 0;
	uint16_t color = grid[index.x][index.y];
	// Swap Up
	if (index.x > 0) {
		swap_elements(grid, index, { (uint16_t)(index.x - 1), index.y });
		uint16_t score = getIndexScore(grid, index, color);
		if (max_score < score) {
			max_score = score;
			position = 1;
		}
		swap_elements(grid, index, { (uint16_t)(index.x - 1), index.y });
	}
	// Swap Down
	if (index.x < LINE_LENGTH - 1) {
		swap_elements(grid, index, { (uint16_t)(index.x + 1), index.y });
		uint16_t score = getIndexScore(grid, index, color);
		if (max_score < score) {
			max_score = score;
			position = 2;
		}
		swap_elements(grid, index, { (uint16_t)(index.x + 1), index.y });
	}
	// Swap Left
	if (index.y > 0) {
		swap_elements(grid, index, { index.x, (uint16_t)(index.y - 1) });
		uint16_t score = getIndexScore(grid, index, color);
		if (max_score < score) {
			max_score = score;
			position = 3;
		}
		swap_elements(grid, index, { index.x, (uint16_t)(index.y - 1) });
	}
	// Swap Right
	if (index.y < LINE_LENGTH - 1) {
		swap_elements(grid, index, { index.x, (uint16_t)(index.y + 1) });
		uint16_t score = getIndexScore(grid, index, color);
		if (max_score < score) {
			max_score = score;
			position = 4;
		}
		swap_elements(grid, index, { index.x, (uint16_t)(index.y + 1) });
	}

	return { max_score, position };
}

uint32_t candy_game(uint16_t grid[][LINE_LENGTH], uint16_t initialGrid[][LINE_LENGTH]) {
	auto time = std::chrono::steady_clock::now();
	auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time).time_since_epoch().count();
	srand(ms);
	generateRandGrid(initialGrid);
	for (size_t i = 0; i < LINE_LENGTH; i++){
		for (size_t j = 0; j < LINE_LENGTH; j++){
			grid[i][j] = initialGrid[i][j];
		}
	}
	int k = rand();
	uint16_t swap_index = 0;
	uint16_t max = 0;
	uint32_t steps = 0;
	uint32_t total_score = 0;
	do
	{
		k = rand();
		srand(ms * k);
		time = std::chrono::steady_clock::now();
		ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time).time_since_epoch().count();
		max = 0;
		swap_index = 0;
		vec2 index = { 0, 0 };
		for (uint16_t i = 0; i < LINE_LENGTH; i++) {
			for (uint16_t j = 0; j < LINE_LENGTH; j++) {
				uint16_t score = getIndexScore(grid, { i, j }, grid[i][j]);
				vec2 swap_score = verify_swapScore(grid, { i, j });
				if (score > max) {
					max = score;
					swap_index = 0;
					index = { i, j };
				}
				if (swap_score.x > max) {
					max = swap_score.x;
					swap_index = swap_score.y;
					index = { i, j };
				}
			}
		}
		switch (swap_index)
		{
		case 1:
			swap_elements(grid, index, { (uint16_t)(index.x - 1) , index.y });
			index.x = (uint16_t)(index.x - 1);
			break;
		case 2:
			swap_elements(grid, index, { (uint16_t)(index.x + 1) , index.y });
			index.x = (uint16_t)(index.x + 1);
			break;
		case 3:
			swap_elements(grid, index, { index.x, (uint16_t)(index.y - 1) });
			index.y = (uint16_t)(index.y - 1);
			break;
		case 4:
			swap_elements(grid, index, { index.x , (uint16_t)(index.y + 1) });
			index.y = (uint16_t)(index.y + 1);
			break;
		default:
			break;
		}
		destroyIndexFormation(grid, index, max);
		applyGravity(grid);
		steps++;
		total_score += max;
	} while (max != 0 && total_score < 10000);
	return steps;
}
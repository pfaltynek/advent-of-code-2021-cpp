#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <assert.h>
#include <regex>

const std::regex C_REGEX_BINGO_LINE("^ ?(\\d+) {1,2}(\\d+) {1,2}(\\d+) {1,2}(\\d+) {1,2}(\\d+)$");
const uint8_t C_BINGO_BOARD_COL_SIZE = 5;
const uint8_t C_BINGO_BOARD_ROW_SIZE = 5;
const uint8_t C_BINGO_BOARD_SIZE = C_BINGO_BOARD_ROW_SIZE * C_BINGO_BOARD_COL_SIZE;

class bingo_board {
  private:
	std::vector<uint8_t> numbers_;
	std::vector<bool> states_;
	bool check_col(const uint8_t col_index) const {
		for (size_t i = 0; i < C_BINGO_BOARD_COL_SIZE; i++) {
			if (!states_[(i * C_BINGO_BOARD_ROW_SIZE) + col_index]) {
				return false;
			}
		}
		return true;
	}

	bool check_row(const uint8_t row_index) const {
		for (size_t i = 0; i < C_BINGO_BOARD_ROW_SIZE; i++) {
			if (!states_[i + (row_index * C_BINGO_BOARD_ROW_SIZE)]) {
				return false;
			}
		}
		return true;
	}

  public:
	bingo_board() {
		numbers_.resize(C_BINGO_BOARD_SIZE, 0);
		states_.resize(C_BINGO_BOARD_SIZE);
		reset();
	}

	void init(std::vector<uint8_t> init_nums) {
		assert(init_nums.size() == C_BINGO_BOARD_SIZE);

		numbers_.swap(init_nums);
		reset();
	}

	void reset() {
		std::fill(states_.begin(), states_.end(), false);
	}

	int64_t get_remaining_sum() {
		int64_t result = 0;

		for (size_t i = 0; i < states_.size(); i++) {
			if (!states_[i]) {
				result += numbers_[i];
			}
		}

		return result;
	}

	bool number_drawn(const uint8_t num) {
		uint8_t index;

		std::vector<uint8_t>::iterator it = std::find(numbers_.begin(), numbers_.end(), num);

		if (it != numbers_.end()) {
			index = it - numbers_.begin();
			states_[index] = true;
			if (check_col(index % C_BINGO_BOARD_ROW_SIZE) || check_row(index / C_BINGO_BOARD_ROW_SIZE)) {
				return true;
			}
		}

		return false;
	}
};

class AoC2021_day04 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint8_t> numbers_;
	std::vector<bingo_board> boards_;
	int64_t play_bingo();
	int64_t play_bingo_till_last_board_remains();
};

bool AoC2021_day04::init(const std::vector<std::string> lines) {
	std::smatch sm;
	std::vector<std::string> tmp;
	std::vector<uint8_t> board;
	bingo_board bb;

	numbers_.clear();
	boards_.clear();

	if (lines.size() < 1) {
		std::cout << "Empty input." << std::endl;
		return false;
	}

	if (lines[0].find_first_not_of("01234567879,") != std::string::npos) {
		std::cout << "Invalid numbers input." << std::endl;
		return false;
	}

	tmp = split(lines[0], ",");

	for (size_t i = 0; i < tmp.size(); i++) {
		numbers_.push_back(static_cast<uint8_t>(stoi(tmp[i])));
	}

	for (uint32_t i = 1; i < lines.size(); i++) {
		if (lines[i].empty()) {
			if (board.size()) {
				std::cout << "Invalid board size definition at line " << i + 1 << std::endl;
				return false;
			} else {
				continue;
			}
		}

		if (std::regex_match(lines[i], sm, C_REGEX_BINGO_LINE)) {
			board.push_back(static_cast<uint8_t>(stoi(sm.str(1))));
			board.push_back(static_cast<uint8_t>(stoi(sm.str(2))));
			board.push_back(static_cast<uint8_t>(stoi(sm.str(3))));
			board.push_back(static_cast<uint8_t>(stoi(sm.str(4))));
			board.push_back(static_cast<uint8_t>(stoi(sm.str(5))));

			if (board.size() == C_BINGO_BOARD_SIZE) {
				bb.init(board);
				boards_.push_back(bb);
				board.clear();
			}
		} else {
			std::cout << "Invalid board row definition at line " << i + 1 << std::endl;
			return false;
		}
	}

	if (board.size()) {
		std::cout << "Incomplete last board definition size" << std::endl;
		return false;
	} else {
		return true;
	}
}

int64_t AoC2021_day04::play_bingo() {
	for (size_t i = 0; i < numbers_.size(); i++) {
		for (size_t j = 0; j < boards_.size(); j++) {
			if (boards_[j].number_drawn(numbers_[i])) {
				return boards_[j].get_remaining_sum() * numbers_[i];
			}
		}
	}

	return 0;
}

int64_t AoC2021_day04::play_bingo_till_last_board_remains() {
	std::vector<uint8_t> boards;
	size_t board_idx;

	for (size_t i = 0; i < boards_.size(); i++) {
		boards.push_back(i);
		boards_[i].reset();
	}

	for (size_t i = 0; i < numbers_.size(); i++) {
		board_idx = 0;

		while (board_idx < boards.size()) {
			if (boards_[boards[board_idx]].number_drawn(numbers_[i])) {
				if (boards.size() == 1) {
					return boards_[boards[0]].get_remaining_sum() * numbers_[i];
				}

				boards.erase(boards.begin() + board_idx);
			}
			else {
				board_idx++;
			}
		}
	}

	return 0;
}

int32_t AoC2021_day04::get_aoc_day() {
	return 4;
}

int32_t AoC2021_day04::get_aoc_year() {
	return 2021;
}

void AoC2021_day04::tests() {
	int64_t result;

	if (init({"7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1", "", "22 13 17 11  0", " 8  2 23  4 24", "21  9 14 16  7",
			  " 6 10  3 18  5", " 1 12 20 15 19", "", " 3 15  0  2 22", " 9 18 13 17  5", "19  8  7 25 23", "20 11 10 24  4", "14 21 16 12  6", "",
			  "14 21 17 24  4", "10 16 15  9 19", "18  8 23 26 20", "22 11 13  6  5", " 2  0 12  3  7"})) {
		result = play_bingo();						   // 4512
		result = play_bingo_till_last_board_remains(); // 1924
	}
}

bool AoC2021_day04::part1() {
	int64_t result = 0;

	result = play_bingo();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day04::part2() {
	int64_t result = 0;

	result = play_bingo_till_last_board_remains();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day04 day04;

	return day04.main_execution();
}

#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>

const std::regex C_PLAYER_TEMPLATE("^Player (\\d) starting position: (\\d+)$");

#define DEBUG_PRINT 0

class AoC2021_day21 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::tuple<uint8_t, size_t, size_t>> stats_;
	size_t p1_start_, p2_start_;
	size_t dice_next_, dice_rolls_;
	size_t simulate_game();
	void print() const;
	void reset();
	size_t get_dice_sum();
};

bool AoC2021_day21::init(const std::vector<std::string> lines) {
	std::smatch sm;

	if (lines.size() != 2) {
		std::cout << "Input data invalid" << std::endl;
		return false;
	}

	stats_.clear();

	if (std::regex_match(lines[0], sm, C_PLAYER_TEMPLATE)) {
		if (sm.str(1) != "1") {
			std::cout << "Player number 1 not found on line 1" << std::endl;
			return false;
		}

		p1_start_ = std::stoul(sm.str(2));
	} else {
		std::cout << "Invalid player info format on line 1" << std::endl;
		return false;
	}

	if (std::regex_match(lines[1], sm, C_PLAYER_TEMPLATE)) {
		if (sm.str(1) != "2") {
			std::cout << "Player number 2 not found on line 2" << std::endl;
			return false;
		}

		p2_start_ = std::stoul(sm.str(2));
	} else {
		std::cout << "Invalid player info format on line 2" << std::endl;
		return false;
	}

	return true;
}

void AoC2021_day21::reset() {
	stats_.clear();
	stats_.push_back(std::make_tuple(1, p1_start_ - 1, 0));
	stats_.push_back(std::make_tuple(2, p2_start_ - 1, 0));
	dice_next_ = 1;
	dice_rolls_ = 0;
}

size_t AoC2021_day21::get_dice_sum() {
	size_t result = 0;

	for (size_t i = 0; i < 3; i++) {
		result += dice_next_;
		dice_next_++;
		dice_next_ %= 100;
		dice_rolls_++;
	}

	return result;
}

size_t AoC2021_day21::simulate_game() {
	bool finished = false;
	uint8_t winner;

	reset();

	while (!finished) {
		for (auto&& tuple : stats_) {

			std::get<1>(tuple) += get_dice_sum();
			std::get<1>(tuple) %= 10;

			std::get<2>(tuple) += std::get<1>(tuple) + 1;

			if (std::get<2>(tuple) >= 1000) {
				winner = std::get<0>(tuple);
				finished = true;
				break;
			}
		}
	}

	switch (winner) {
		case 1:
			return dice_rolls_ * std::get<2>(stats_[1]);
		case 2:
			return dice_rolls_ * std::get<2>(stats_[0]);
		default:
			return 0;
	}
}

void AoC2021_day21::print() const {
#if DEBUG_PRINT
	for (int32_t y = top_; y <= bottom_; y++) {
		for (int32_t x = left_; x <= right_; x++) {
			if (image_.count({x, y}) > 0) {
				std::cout << '#';
			} else {
				std::cout << '.';
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

#endif
}

int32_t AoC2021_day21::get_aoc_day() {
	return 21;
}

int32_t AoC2021_day21::get_aoc_year() {
	return 2021;
}

void AoC2021_day21::tests() {
	int64_t result;

	if (init({"Player 1 starting position: 4", "Player 2 starting position: 8"})) {
		result = simulate_game(); // 739785
	}
}

bool AoC2021_day21::part1() {
	int64_t result = 0;

	result = simulate_game();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day21::part2() {
	int64_t result = 0;

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day21 day21;

	return day21.main_execution();
}

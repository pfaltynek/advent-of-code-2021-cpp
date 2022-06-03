#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <array>
#include <regex>

const std::regex C_PLAYER_TEMPLATE("^Player (\\d) starting position: (\\d+)$");
const uint16_t C_PART1_LIMIT = 1000;
const uint8_t C_PART2_LIMIT = 21;

enum part2_branches_t {
	DIRAC_111 = 3,
	DIRAC_112 = 4,
	DIRAC_113 = 5,
	DIRAC_121 = 4,
	DIRAC_122 = 5,
	DIRAC_123 = 6,
	DIRAC_131 = 5,
	DIRAC_132 = 6,
	DIRAC_133 = 7,

	DIRAC_211 = 4,
	DIRAC_212 = 5,
	DIRAC_213 = 6,
	DIRAC_221 = 5,
	DIRAC_222 = 6,
	DIRAC_223 = 7,
	DIRAC_231 = 6,
	DIRAC_232 = 7,
	DIRAC_233 = 8,

	DIRAC_311 = 5,
	DIRAC_312 = 6,
	DIRAC_313 = 7,
	DIRAC_321 = 6,
	DIRAC_322 = 7,
	DIRAC_323 = 8,
	DIRAC_331 = 7,
	DIRAC_332 = 8,
	DIRAC_333 = 9,
};

const std::array<uint8_t, 27> C_DIRAC_DICES = {DIRAC_111, DIRAC_112, DIRAC_113, DIRAC_121, DIRAC_122, DIRAC_123, DIRAC_131, DIRAC_132, DIRAC_133,
											   DIRAC_211, DIRAC_212, DIRAC_213, DIRAC_221, DIRAC_222, DIRAC_223, DIRAC_231, DIRAC_232, DIRAC_233,
											   DIRAC_311, DIRAC_312, DIRAC_313, DIRAC_321, DIRAC_322, DIRAC_323, DIRAC_331, DIRAC_332, DIRAC_333};

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
	std::map<size_t, size_t> Dirac_dices_;
	size_t p1_start_, p2_start_;
	size_t dice_next_, dice_rolls_;
	size_t simulate_game();
	size_t simulate_Dirac_dice_game();
	void simulate_Dirac_dice_game_step(size_t p1position, size_t p2position, size_t p1score, size_t p2score, size_t multiply, bool p1on_turn, size_t& p1wins,
									   size_t& p2wins);
	void print() const;
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

	stats_.clear();
	stats_.push_back(std::make_tuple(1, p1_start_ - 1, 0));
	stats_.push_back(std::make_tuple(2, p2_start_ - 1, 0));
	dice_next_ = 1;
	dice_rolls_ = 0;

	while (!finished) {
		for (auto&& tuple : stats_) {

			std::get<1>(tuple) += get_dice_sum();
			std::get<1>(tuple) %= 10;

			std::get<2>(tuple) += std::get<1>(tuple) + 1;

			if (std::get<2>(tuple) >= C_PART1_LIMIT) {
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

void AoC2021_day21::simulate_Dirac_dice_game_step(size_t p1position, size_t p2position, size_t p1score, size_t p2score, size_t multiply, bool p1on_turn,
												  size_t& p1wins, size_t& p2wins) {

	size_t pos, score, mult;

	for (const auto& kv : Dirac_dices_) {
		mult = multiply * kv.second;

		if (p1on_turn) {
			pos = p1position + kv.first;
			pos %= 10;
			score = p1score + pos + 1;

			if (score >= C_PART2_LIMIT) {
				p1wins += mult;
			} else {
				simulate_Dirac_dice_game_step(pos, p2position, score, p2score, mult, !p1on_turn, p1wins, p2wins);
			}
		} else {
			pos = p2position + kv.first;
			pos %= 10;
			score = p2score + pos + 1;

			if (score >= C_PART2_LIMIT) {
				p2wins += mult;
			} else {
				simulate_Dirac_dice_game_step(p1position, pos, p1score, score, mult, !p1on_turn, p1wins, p2wins);
			}
		}
	}
}

size_t AoC2021_day21::simulate_Dirac_dice_game() {
	size_t p1wins = 0, p2wins = 0;

	Dirac_dices_.clear();

	for (size_t i = 0; i < C_DIRAC_DICES.size(); i++) {
		Dirac_dices_[C_DIRAC_DICES[i]]++;
	}

	simulate_Dirac_dice_game_step(p1_start_ - 1, p2_start_ - 1, 0, 0, 1, true, p1wins, p2wins);

	return std::max(p1wins, p2wins);
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
		result = simulate_game();			 // 739785
		result = simulate_Dirac_dice_game(); // 444356092776315
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

	result = simulate_Dirac_dice_game();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day21 day21;

	return day21.main_execution();
}

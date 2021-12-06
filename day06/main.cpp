#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <numeric>

const uint8_t C_NEW_BORN_TIMER = 8;
const uint8_t C_RESET_TIMER = 6;

class AoC2021_day06 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint8_t> fishes_;
	int64_t simulate_life_of_days(const size_t days);
	int64_t simulate_life_of_days_simple(const size_t days);
};

bool AoC2021_day06::init(const std::vector<std::string> lines) {
	std::vector<std::string> tmp;

	fishes_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("012345678,") != std::string::npos) {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		} else {
			tmp = split(lines[i], ",");
			for (size_t j = 0; j < tmp.size(); j++) {
				fishes_.push_back(static_cast<uint8_t>(std::stoi(tmp[j])));
			}
		}
	}

	return true;
}

int64_t AoC2021_day06::simulate_life_of_days(const size_t days) {
	std::vector<uint8_t> fishes = fishes_;
	size_t born_fishes_cnt;

	for (size_t i = 0; i < days; i++) {
		born_fishes_cnt = 0;

		for (size_t j = 0; j < fishes.size(); j++) {
			if (fishes[j] == 0) {
				fishes[j] = C_RESET_TIMER;
				born_fishes_cnt++;
			} else {
				fishes[j]--;
			}
		}

		while (born_fishes_cnt--) {
			fishes.push_back(C_NEW_BORN_TIMER);
		}
	}

	return fishes.size();
}

// simpler and much quicker method by daniel-meilak https://github.com/daniel-meilak/Advent-of-Code-2021/blob/main/week_1/day_6/day_6.cpp
int64_t AoC2021_day06::simulate_life_of_days_simple(const size_t days) {
	std::vector<int64_t> counts;
	int64_t born_fishes_cnt;

	counts.resize(9);

	for (size_t i = 0; i < fishes_.size(); i++) {
		counts[fishes_[i]]++;
	}

	for (size_t i = 0; i < days; i++) {
		born_fishes_cnt = counts[0];

		for (size_t j = 1; j < 9; j++) {
			counts[j - 1] = counts[j];
		}

		counts[8] = born_fishes_cnt;
		counts[6] += born_fishes_cnt;
	}

	return std::accumulate(counts.begin(), counts.end(), 0ll);
}

int32_t AoC2021_day06::get_aoc_day() {
	return 5;
}

int32_t AoC2021_day06::get_aoc_year() {
	return 2021;
}

void AoC2021_day06::tests() {
	int64_t result;

	if (init({"3,4,3,1,2"})) {
		result = simulate_life_of_days(18);	 // 26
		result = simulate_life_of_days(80);	 // 5934

		result = simulate_life_of_days_simple(18);	 // 26
		result = simulate_life_of_days_simple(80);	 // 5934
		result = simulate_life_of_days_simple(256); // 26984457539
	}
}

bool AoC2021_day06::part1() {
	int64_t result = 0;

	result = simulate_life_of_days(80);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day06::part2() {
	int64_t result = 0;

	result = simulate_life_of_days_simple(256);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day06 day06;

	return day06.main_execution();
}

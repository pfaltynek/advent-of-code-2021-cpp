#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <algorithm>
#include <map>

const uint8_t C_NEW_BORN_TIMER = 8;
const uint8_t C_RESET_TIMER = 6;

class AoC2021_day07 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint16_t> crab_positions_;
	int64_t find_cheapest_position();
};

bool AoC2021_day07::init(const std::vector<std::string> lines) {
	std::vector<std::string> tmp;

	crab_positions_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789,") != std::string::npos) {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		} else {
			tmp = split(lines[i], ",");
			for (size_t j = 0; j < tmp.size(); j++) {
				crab_positions_.push_back(static_cast<uint16_t>(std::stoi(tmp[j])));
			}
		}
	}

	return true;
}

int64_t AoC2021_day07::find_cheapest_position() {
	std::map<uint16_t, uint16_t> counts;
	uint16_t highest = 0;
	std::vector<uint16_t> positions;
	int64_t lowest_fuel, tmp, tmp2;
	uint16_t min, max;

	for (size_t j = 0; j < crab_positions_.size(); j++) {
		counts[crab_positions_[j]]++;
	}

	highest = 0;
	positions.clear();
	for (auto it = counts.begin(); it != counts.end(); it++) {
		if (it->second > highest) {
			positions.clear();
			highest = it->second;
			positions.push_back(it->first);
		} else if (it->second == highest) {
			positions.push_back(it->first);
		}
	}

	min = *std::min_element(crab_positions_.begin(), crab_positions_.end());
	max = *std::max_element(crab_positions_.begin(), crab_positions_.end());

	for (size_t i = min; i <= max; i++) {
		tmp = 0;
		for (auto it = counts.begin(); it != counts.end(); it++) {
			tmp2 = static_cast<int64_t>(it->first);
			tmp2 -= static_cast<int64_t>(i);
			tmp += std::abs(tmp2) * it->second;
		}

		if (i == min) {
			lowest_fuel = tmp;
		} else if (tmp < lowest_fuel) {
			lowest_fuel = tmp;
		}
	}

	return lowest_fuel; // 110416 < ? < 457552
}

int32_t AoC2021_day07::get_aoc_day() {
	return 7;
}

int32_t AoC2021_day07::get_aoc_year() {
	return 2021;
}

void AoC2021_day07::tests() {
	int64_t result;

	if (init({"16,1,2,0,4,2,7,1,2,14"})) {
		result = find_cheapest_position(); // 37
	}
}

bool AoC2021_day07::part1() {
	int64_t result = 0;

	result = find_cheapest_position();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day07::part2() {
	int64_t result = 0;

	result = find_cheapest_position();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day07 day07;

	return day07.main_execution();
}

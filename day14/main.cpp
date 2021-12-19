#include "./../common/aoc.hpp"
#include <algorithm>
#include <map>
#include <regex>
#include <sstream>

const std::regex C_RULE_REGEX("^([A-Z]{2}) -> ([A-Z])$");

class AoC2021_day14 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<std::string, char> rules_;
	std::string polymer_template_;
	int64_t get_stats_after_rules_apply(const size_t steps);
};

bool AoC2021_day14::init(const std::vector<std::string> lines) {
	bool rules = false;
	std::smatch sm;

	rules_.clear();
	polymer_template_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].empty()) {
			rules = true;
			continue;
		}

		if (rules) {
			if (std::regex_match(lines[i], sm, C_RULE_REGEX)) {
				rules_[sm.str(1)] = sm.str(2)[0];
			} else {
				std::cout << "Invalid rule at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			if (lines[i].size() >= 2) {
				polymer_template_ = lines[i];
			} else {
				std::cout << "Invalid polymer template at line " << i + 1 << std::endl;
				return false;
			}
		}
	}

	return true;
}

int64_t AoC2021_day14::get_stats_after_rules_apply(const size_t steps) {
	std::map<std::string, size_t> polymers_actual, polymers_next;
	std::map<char, size_t> histogram;
	std::string polymer, p1, p2;
	char element;
	size_t min = SIZE_MAX, max = 0;

	for (char element : polymer_template_) {
		histogram[element]++;
	}

	for (size_t i = 1; i < polymer_template_.size(); i++) {
		polymer = polymer_template_.substr(i - 1, 2);
		polymers_actual[polymer]++;
	}

	for (size_t i = 0; i < steps; i++) {
		polymers_next.clear();

		for (auto it = polymers_actual.begin(); it != polymers_actual.end(); it++) {
			if (!rules_.count(it->first)) {
				continue;
			}

			element = rules_[it->first];
			p1 = it->first.front();
			p1 += element;
			p2 = element;
			p2 += it->first.back();
			polymers_next[p1] += it->second;
			polymers_next[p2] += it->second;
			histogram[element] += it->second;
		}

		polymers_actual.swap(polymers_next);
	}

	for (auto it = histogram.begin(); it != histogram.end(); it++) {

		if (it->second > max) {
			max = it->second;
		}
		if (it->second < min) {
			min = it->second;
		}
	}

	return max - min;

	/*
	non efficient way - better to count polymers as above
	thanks for idea to NEET_Here https://godbolt.org/z/x5515a7Te

	int64_t min = INT64_MAX, max = INT64_MIN, tmp;
	std::string input = polymer_template_, pair;
	std::stringstream output;

	for (size_t i = 0; i < steps; i++) {
		std::stringstream().swap(output); // clear by swapping with just constructed stringstream

		if (input.size()) {
			output << input[0];
		}

		for (size_t j = 0; j < input.size() - 1; j++) {
			pair = input.substr(j, 2);

			if (rules_.count(pair)) {
				output << rules_[pair];
			}

			output << pair[1];
		}

		input = output.str();
	}

	for (char i = 'A'; i <= 'Z'; i++) {
		tmp = std::count(input.begin(), input.end(), i);

		if (!tmp) {
			continue;
		}

		if (tmp > max) {
			max = tmp;
		}
		if (tmp < min) {
			min = tmp;
		}
	}

	return max - min;
	*/
}

int32_t AoC2021_day14::get_aoc_day() {
	return 14;
}

int32_t AoC2021_day14::get_aoc_year() {
	return 2021;
}

void AoC2021_day14::tests() {
	int64_t result;

	if (init({"NNCB", "", "CH -> B", "HH -> N", "CB -> H", "NH -> C", "HB -> C", "HC -> B", "HN -> C", "NN -> C", "BH -> H", "NC -> B", "NB -> B", "BN -> B",
			  "BB -> N", "BC -> B", "CC -> N", "CN -> C"})) {
		result = get_stats_after_rules_apply(10); // 1588
		result = get_stats_after_rules_apply(40); // 2188189693529
	}
}

bool AoC2021_day14::part1() {
	int64_t result = 0;

	result = get_stats_after_rules_apply(10);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day14::part2() {
	int64_t result = 0;

	result = get_stats_after_rules_apply(40);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day14 day14;

	return day14.main_execution();
}

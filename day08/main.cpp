#include "./../common/aoc.hpp"
#include <map>
#include <regex>

const std::regex C_DISPLAY_PATTERN("^([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) "
								   "([a-g]{2,7}) \\| ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7})$");
const std::map<uint8_t, std::string> C_DIGITS_SEGMENTS = {{0, "abcefg"}, {1, "cf"},		{2, "acdeg"}, {3, "acdfg"},	  {4, "bcdf"},
														  {5, "abdfg"},	 {6, "abdefg"}, {7, "acf"},	  {8, "abcdefg"}, {9, "abcdfg"}};

typedef std::pair<std::vector<std::string>, std::vector<std::string>> display;

class AoC2021_day08 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<display> displays_;
	int64_t count_unique_digits();
	int64_t sum_output_digits();
	bool analyze_7segments(const std::vector<std::string> display, std::map<char, char>& mapping);
};

bool AoC2021_day08::init(const std::vector<std::string> lines) {
	std::smatch sm;

	displays_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, C_DISPLAY_PATTERN)) {
			displays_.push_back({{
									 sm.str(1),
									 sm.str(2),
									 sm.str(3),
									 sm.str(4),
									 sm.str(5),
									 sm.str(6),
									 sm.str(7),
									 sm.str(8),
									 sm.str(9),
									 sm.str(10),
								 },
								 {
									 sm.str(11),
									 sm.str(12),
									 sm.str(13),
									 sm.str(14),
								 }});

		} else {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

int64_t AoC2021_day08::count_unique_digits() {
	int64_t result = 0;
	std::vector<uint8_t> unique_sizes = {
		static_cast<uint8_t>(C_DIGITS_SEGMENTS.at(1).size()),
		static_cast<uint8_t>(C_DIGITS_SEGMENTS.at(4).size()),
		static_cast<uint8_t>(C_DIGITS_SEGMENTS.at(7).size()),
		static_cast<uint8_t>(C_DIGITS_SEGMENTS.at(8).size()),
	};

	for (size_t i = 0; i < displays_.size(); i++) {
		for (auto it = displays_[i].second.begin(); it != displays_[i].second.end(); it++) {
			if (std::find(unique_sizes.begin(), unique_sizes.end(), it->size()) != unique_sizes.end()) {
				result++;
			}
		}
	}

	return result;
}

int64_t AoC2021_day08::sum_output_digits() {
	int64_t result = 0, number;
	std::map<char, char> mapping;
	std::string output, translated;
	std::map<std::string, int64_t> digits;

	for (auto it = C_DIGITS_SEGMENTS.begin(); it != C_DIGITS_SEGMENTS.end(); it++) {
		digits[it->second] = it->first;
	}

	for (size_t i = 0; i < displays_.size(); i++) {
		if (!analyze_7segments(displays_[i].first, mapping)) {
			return -1;
		}

		number = 0;

		for (size_t j = 0; j < displays_[i].second.size(); j++) {
			output = displays_[i].second[j];
			translated.clear();

			for (auto it = output.begin(); it != output.end(); it++) {
				if (!mapping.count(*it)) {
					return -1;
				}

				translated += mapping[*it];
			}

			std::sort(translated.begin(), translated.end());

			if (!digits.count(translated)) {
				return -1;
			}

			number *= 10;
			number += digits[translated];
		}

		result += number;
	}

	return result;
}

bool AoC2021_day08::analyze_7segments(const std::vector<std::string> display, std::map<char, char>& mapping) {
	std::map<size_t, std::vector<std::string>> sort_by_size;
	std::string str1, str2, rest, bd, tmp;
	size_t cnt, pos;
	std::map<char, char> map;
	// segments -> numbers:
	// 2 -> 1
	// 3 -> 7
	// 4 -> 8
	// 5 -> 2, 3, 5
	// 6 -> 0, 6, 9

	mapping.clear();
	map.clear();

	for (size_t i = 0; i < display.size(); i++) {
		sort_by_size[display[i].size()].push_back(display[i]);
	}

	if ((sort_by_size[2].size() != 1) || (sort_by_size[3].size() != 1) || (sort_by_size[4].size() != 1) || (sort_by_size[7].size() != 1) ||
		(sort_by_size[5].size() != 3) || (sort_by_size[6].size() != 3)) {
		return false;
	}
	// identify a edge: 7 - 1:
	str1 = sort_by_size[3][0];
	str2 = sort_by_size[2][0];
	rest.clear();

	for (size_t i = 0; i < str1.size(); i++) {
		if (str2.find(str1[i]) == std::string::npos) {
			rest += str1[i];
		}
	}

	if (rest.size() != 1) {
		return false;
	}

	map['a'] = rest[0];

	bd.clear();
	str1 = sort_by_size[4][0];
	for (size_t i = 0; i < str1.size(); i++) {
		if (str2.find(str1[i]) == std::string::npos) {
			bd += str1[i];
		}
	}

	if (bd.size() != 2) {
		return false;
	}

	rest.clear();
	tmp.clear();
	for (size_t i = 0; i < bd.size(); i++) {
		cnt = 0;

		for (size_t j = 0; j < sort_by_size[5].size(); j++) {
			if (sort_by_size[5][j].find(bd[i]) != std::string::npos) {
				cnt++;
			}
		}

		if (cnt == sort_by_size[5].size()) {
			rest += bd[i];
		} else {
			tmp += bd[i];
		}
	}

	if ((rest.size() != 1) || (tmp.size() != 1)) {
		return false;
	}

	map['d'] = rest[0];
	map['b'] = tmp[0];

	rest.clear();
	for (size_t i = 0; i < sort_by_size[5][0].size(); i++) {
		cnt = 0;

		for (size_t j = 1; j < sort_by_size[5].size(); j++) {
			if (sort_by_size[5][j].find(sort_by_size[5][0][i]) != std::string::npos) {
				cnt++;
			}
		}

		if ((cnt == sort_by_size[5].size() - 1) && (sort_by_size[5][0][i] != map['a']) && (sort_by_size[5][0][i] != map['d'])) {
			rest += sort_by_size[5][0][i];
		}
	}

	if (rest.size() != 1) {
		return false;
	}

	map['g'] = rest[0];

	tmp = sort_by_size[2][0];
	tmp += map['a'];
	tmp += map['b'];
	tmp += map['d'];
	tmp += map['g'];
	if (tmp.size() + 1 != sort_by_size[7][0].size()) {
		return false;
	}

	pos = sort_by_size[7][0].find_first_not_of(tmp);
	if (pos == std::string::npos) {
		return false;
	}

	map['e'] = sort_by_size[7][0][pos];

	rest.clear();

	for (size_t i = 0; i < sort_by_size[6].size(); i++) {
		pos = sort_by_size[2][0].find_first_not_of(sort_by_size[6][i]);
		if (pos != std::string::npos) {
			rest += sort_by_size[2][0][pos];
		}
	}

	if (rest.size() != 1) {
		return false;
	}

	map['c'] = rest[0];

	if (sort_by_size[2][0][0] == rest[0]) {
		map['f'] = sort_by_size[2][0][1];
	} else {
		map['f'] = sort_by_size[2][0][0];
	}

	for (auto it = map.begin(); it != map.end(); it++) {
		mapping[it->second] = it->first;
	}

	return true;
}

int32_t AoC2021_day08::get_aoc_day() {
	return 8;
}

int32_t AoC2021_day08::get_aoc_year() {
	return 2021;
}

void AoC2021_day08::tests() {
	int64_t result;

	if (init({"be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe",
			  "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc",
			  "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg",
			  "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb",
			  "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea",
			  "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb",
			  "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe",
			  "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef",
			  "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb",
			  "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce"})) {
		result = count_unique_digits(); // 26
		result = sum_output_digits(); // 61229
	}

	if (init({"acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf"})) {
		std::map<char, char> x;

		analyze_7segments(displays_[0].first, x);
	}


}

bool AoC2021_day08::part1() {
	int64_t result = 0;

	result = count_unique_digits();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day08::part2() {
	int64_t result = 0;

	result = sum_output_digits();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day08 day08;

	return day08.main_execution();
}

#include "./../common/aoc.hpp"
#include <map>
#include <regex>
const std::regex C_DISPLAY_PATTERN("^([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) \\| ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7}) ([a-g]{2,7})$");
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

	result = count_unique_digits();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day08 day08;

	return day08.main_execution();
}

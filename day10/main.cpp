#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>

const std::string C_OPENING = "({[<";
const std::string C_CLOSING = ")}]>";
const std::map<char, size_t> C_ERROR_SCORE = {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
const std::map<char, size_t> C_COMPLETE_POINTS = {{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};
const std::map<char, char> C_PAIRS = {{'(', ')'}, {'{', '}'}, {'[', ']'}, {'<', '>'}};

class AoC2021_day10 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::string> navigation_, incomplete_;
	bool is_corrupted(const std::string line) const;
	int64_t get_syntax_error_score(const std::string line) const;
	int64_t get_syntax_error_score_total();
	int64_t find_middle_score() const;
	int64_t complete_line(const size_t line_index) const;
};

bool AoC2021_day10::init(const std::vector<std::string> lines) {
	const std::string char_map = C_OPENING + C_CLOSING;

	navigation_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of(char_map) != std::string::npos) {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		} else {
			navigation_.push_back(lines[i]);
		}
	}

	return true;
}

int64_t AoC2021_day10::get_syntax_error_score(const std::string line) const {
	std::string q = "";
	char c;

	for (size_t i = 0; i < line.size(); i++) {
		if (std::find(C_OPENING.begin(), C_OPENING.end(), line[i]) != C_OPENING.end()) {
			q += line[i];
		} else {
			c = q.back();
			q.pop_back();

			if (line[i] != C_PAIRS.at(c)) {
				return C_ERROR_SCORE.at(line[i]);
			}
		}
	}

	return 0;
}

int64_t AoC2021_day10::get_syntax_error_score_total() {
	int64_t result = 0, tmp;

	incomplete_.clear();

	for (size_t i = 0; i < navigation_.size(); i++) {
		tmp = get_syntax_error_score(navigation_[i]);

		if (tmp) {
			result += tmp;
		} else {
			incomplete_.push_back(navigation_[i]);
		}
	}

	return result;
}

int64_t AoC2021_day10::complete_line(const size_t line_index) const {
	std::string tmp = "", complete = "";
	int64_t result = 0;
	char c;

	for (size_t i = 0; i < incomplete_[line_index].size(); i++) {
		if (std::find(C_OPENING.begin(), C_OPENING.end(), incomplete_[line_index][i]) != C_OPENING.end()) {
			tmp += incomplete_[line_index][i];
		} else {
			c = tmp.back();
			tmp.pop_back();

			if (incomplete_[line_index][i] != C_PAIRS.at(c)) {
				return -1;
			}
		}
	}

	std::reverse(tmp.begin(), tmp.end());

	for (size_t i = 0; i < tmp.size(); i++) {
		if (!C_PAIRS.count(tmp[i])) {
			return -1;
		}
		c = C_PAIRS.at(tmp[i]);

		result *= 5;
		result += C_COMPLETE_POINTS.at(c);
	}

	return result;
}

int64_t AoC2021_day10::find_middle_score() const {
	std::vector<int64_t> scores;

	scores.clear();

	for (size_t i = 0; i < incomplete_.size(); i++) {
		scores.push_back(complete_line(i));
	}

	if (scores.size()) {
		std::sort(scores.begin(), scores.end());
		return scores[scores.size() / 2];
	} else {
		return 0;
	}
}

int32_t AoC2021_day10::get_aoc_day() {
	return 10;
}

int32_t AoC2021_day10::get_aoc_year() {
	return 2021;
}

void AoC2021_day10::tests() {
	int64_t result;

	if (init({"[({(<(())[]>[[{[]{<()<>>", "[(()[<>])]({[<{<<[]>>(", "{([(<{}[<>[]}>{[]{[(<()>", "(((({<>}<{<{<>}{[]{[]{}", "[[<[([]))<([[{}[[()]]]",
			  "[{[{({}]{}}([{[{{{}}([]", "{<[[]]>}<{[{[{[]{()[[[]", "[<(<(<(<{}))><([]([]()", "<{([([[(<>()){}]>(<<{{", "<{([{{}}[<[[[<>{}]]]>[]]"})) {
		result = get_syntax_error_score_total(); // 26397
		result = find_middle_score();			 // 288957
	}
}

bool AoC2021_day10::part1() {
	int64_t result = 0;

	result = get_syntax_error_score_total();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day10::part2() {
	int64_t result = 0;

	result = find_middle_score();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day10 day10;

	return day10.main_execution();
}

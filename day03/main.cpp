#include "./../common/aoc.hpp"

class AoC2021_day03 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t decode_power_consumption();
	std::vector<std::string> report_;
	uint16_t report_item_size_;
};

bool AoC2021_day03::init(const std::vector<std::string> lines) {

	report_.clear();
	report_item_size_ = 0;

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (i) {
			if (lines[i].size() != report_item_size_) {
				std::cout << "Invalid report size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			report_item_size_ = lines[i].size();
		}

		if (lines[i].find_first_not_of("01") == std::string::npos) {
			report_.push_back(lines[i]);
		} else {
			std::cout << "Invalid report at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

int64_t AoC2021_day03::decode_power_consumption() {
	int64_t result;
	uint16_t limit;
	std::vector<uint16_t> counts(report_item_size_, 0);
	std::string bing, bine;

	for (size_t i = 0; i < report_.size(); i++) {
		for (size_t j = 0; j < report_[i].size(); j++) {
			if (report_[i][j] == '1') {
				counts[j]++;
			}
		}
	}

	limit = report_.size() / 2;
	if ((report_.size() + 1) / 2 != limit) {
		limit++;
	}

	bine.clear();
	bing.clear();

	for (size_t i = 0; i < counts.size(); i++) {
		if (counts[i] >= limit) {
			bing += '1';
			bine += '0';
		} else {
			bing += '0';
			bine += '1';
		}
	}

	result = std::stoi(bing, nullptr, 2) * std::stoi(bine, nullptr, 2);

	return result;
}

int32_t AoC2021_day03::get_aoc_day() {
	return 3;
}

int32_t AoC2021_day03::get_aoc_year() {
	return 2021;
}

void AoC2021_day03::tests() {
	int64_t result;

	if (init({"00100", "11110", "10110", "10111", "10101", "01111", "00111", "11100", "10000", "11001", "00010", "01010"})) {
		result = decode_power_consumption(); // 198
	}
}

bool AoC2021_day03::part1() {
	int64_t result = 0;

	result = decode_power_consumption();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day03::part2() {
	int64_t result = 0;

	result = decode_power_consumption();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day03 day03;

	return day03.main_execution();
}

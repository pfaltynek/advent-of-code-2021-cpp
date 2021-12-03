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
	int64_t decode_life_support_rating();
	void filter_life_support_rating(std::vector<std::string>& report, bool most_common);
	std::vector<std::string> report_;
	uint16_t report_item_size_;
	std::string binary_gamma_, binary_epsilon_;
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

	binary_epsilon_.clear();
	binary_gamma_.clear();

	for (size_t i = 0; i < counts.size(); i++) {
		if (counts[i] >= limit) {
			binary_gamma_ += '1';
			binary_epsilon_ += '0';
		} else {
			binary_gamma_ += '0';
			binary_epsilon_ += '1';
		}
	}

	result = std::stoi(binary_gamma_, nullptr, 2) * std::stoi(binary_epsilon_, nullptr, 2);

	return result;
}

int64_t AoC2021_day03::decode_life_support_rating() {
	int64_t result = -1;
	std::vector<std::string> oxygen, co2;

	oxygen.clear();
	co2.clear();

	for (size_t i = 0; i < report_.size(); i++) {
		if (report_[i][0] == '1') {
			oxygen.push_back(report_[i]);
		} else {
			co2.push_back(report_[i]);
		}
	}

	if (oxygen.size() < co2.size()) {
		oxygen.swap(co2);
	}

	filter_life_support_rating(oxygen, true);
	filter_life_support_rating(co2, false);

	if ((oxygen.size() == 1) && (co2.size() == 1)) {
		result = std::stoi(oxygen[0], nullptr, 2) * std::stoi(co2[0], nullptr, 2);
	}

	return result;
}

void AoC2021_day03::filter_life_support_rating(std::vector<std::string>& report, bool most_common) {
	size_t idx, idx1, cnt1, cnt0;
	char remove;

	idx = 1;
	while ((report.size() > 1) && (idx < report_item_size_)) {
		cnt1 = cnt0 = 0;

		for (size_t i = 0; i < report.size(); i++) {
			if (report[i][idx] == '1') {
				cnt1++;
			} else {
				cnt0++;
			}
		}

		if (most_common) {
			if (cnt1 == cnt0) {
				remove = '0';
			} else if (cnt1 > cnt0) {
				remove = '0';
			} else {
				remove = '1';
			}
		} else {
			if (cnt1 == cnt0) {
				remove = '1';
			} else if (cnt1 > cnt0) {
				remove = '1';
			} else {
				remove = '0';
			}
		}

		idx1 = 0;

		while (idx1 < report.size()) {
			if (report[idx1][idx] == remove) {
				report.erase(report.begin() + idx1);
			} else {
				idx1++;
			}
		}

		idx++;
	}
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
		result = decode_power_consumption();   // 198
		result = decode_life_support_rating(); // 230
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

	result = decode_life_support_rating();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day03 day03;

	return day03.main_execution();
}

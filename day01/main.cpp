#include "./../common/aoc.hpp"

class AoC2021_day01 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t get_depth_increases_count();
	std::vector<uint16_t> depths_;
};

bool AoC2021_day01::init(const std::vector<std::string> lines) {

	depths_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		depths_.push_back(static_cast<uint16_t>(std::stoi(lines[i])));
	}

	return true;
}

int64_t AoC2021_day01::get_depth_increases_count() {
	int64_t result = 0;

	if (depths_.size() < 2) {
		return 0;
	}

	for (size_t i = 1; i < depths_.size(); i++) {
		if (depths_[i] > depths_[i - 1]) {
			result++;
		}
	}

	return result;
}

int32_t AoC2021_day01::get_aoc_day() {
	return 1;
}

int32_t AoC2021_day01::get_aoc_year() {
	return 2021;
}

void AoC2021_day01::tests() {
	int64_t result;

	if (init({"199", "200", "208", "210", "200", "207", "240", "269", "260", "263"})) {
		result = get_depth_increases_count(); // 7
	}
}

bool AoC2021_day01::part1() {
	int64_t result = 0;

	result = get_depth_increases_count();

	result1_ = std::to_string(result);

	return false;
}

bool AoC2021_day01::part2() {
	int64_t result = 0;

	result2_ = std::to_string(result);
	return true;
}

int main(void) {
	AoC2021_day01 day01;

	return day01.main_execution();
}

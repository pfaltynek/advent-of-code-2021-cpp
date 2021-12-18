#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <regex>

const std::regex C_COORD("^(\\d+),(\\d+)$");
const std::regex C_FOLD_X("^fold along x=(\\d+)$");
const std::regex C_FOLD_Y("^fold along y=(\\d+)$");

typedef std::pair<bool, uint32_t> fold_info;

class AoC2021_day13 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, bool> dots_;
	std::vector<fold_info> fold_infos_;
	int64_t get_dots_count_after_folds(const uint32_t folds_count);
};

bool AoC2021_day13::init(const std::vector<std::string> lines) {
	bool folds = false;
	std::smatch sm;

	dots_.clear();
	fold_infos_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].empty()) {
			folds = true;
			continue;
		}

		if (folds) {
			if (std::regex_match(lines[i], sm, C_FOLD_X)) {
				fold_infos_.push_back({true, static_cast<uint32_t>(std::stoi(sm.str(1)))});
			} else if (std::regex_match(lines[i], sm, C_FOLD_Y)) {
				fold_infos_.push_back({false, static_cast<uint32_t>(std::stoi(sm.str(1)))});
			} else {
				std::cout << "Invalid fold info at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			if (std::regex_match(lines[i], sm, C_COORD)) {
				dots_[{static_cast<int32_t>(std::stoi(sm.str(1))), static_cast<int32_t>(std::stoi(sm.str(2)))}] = true;
			} else {
				std::cout << "Invalid dot coordinate info at line " << i + 1 << std::endl;
				return false;
			}
		}
	}

	return true;
}

int64_t AoC2021_day13::get_dots_count_after_folds(const uint32_t folds_count) {
	std::map<coord_str, bool> new_dots;
	coord_str coord;

	for (size_t i = 0; i < folds_count; i++) {
		new_dots.clear();

		for (auto it = dots_.begin(); it != dots_.end(); it++) {
			coord = it->first;
			if (fold_infos_[i].first) { // fold along x
				if (coord.x > fold_infos_[i].second) {
					coord.x = fold_infos_[i].second - (coord.x - fold_infos_[i].second);
				}

				new_dots[coord] = true;
			} else { // fold along y
				if (coord.y > fold_infos_[i].second) {
					coord.y = fold_infos_[i].second - (coord.y - fold_infos_[i].second);
				}

				new_dots[coord] = true;
			}
		}

		dots_.swap(new_dots);
	}

	return dots_.size();
}

int32_t AoC2021_day13::get_aoc_day() {
	return 13;
}

int32_t AoC2021_day13::get_aoc_year() {
	return 2021;
}

void AoC2021_day13::tests() {
	int64_t result;

	if (init({"6,10", "0,14", "9,10", "0,3",  "10,4", "4,11", "6,0", "6,12",		   "4,1",			"0,13", "10,12", "3,4",
			  "3,0",  "8,4",  "1,10", "2,14", "8,10", "9,0",  "",	 "fold along y=7", "fold along x=5"})) {
		result = get_dots_count_after_folds(1); // 17
	}
}

bool AoC2021_day13::part1() {
	int64_t result = 0;

	result = get_dots_count_after_folds(1);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day13::part2() {
	int64_t result = 0;

	result = get_dots_count_after_folds(1);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day13 day13;

	return day13.main_execution();
}

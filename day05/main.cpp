#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <regex>

const std::regex C_REGEX_COORDS("^(\\d+),(\\d+) -> (\\d+),(\\d+)$");
const uint8_t C_LINES_NUMBER_LIMIT = 2;

typedef std::pair<COORD2D, COORD2D> line_def;

class AoC2021_day05 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<line_def> lines_;
	int64_t get_lines_overlap_points_count(const bool part2);
};

bool AoC2021_day05::init(const std::vector<std::string> lines) {
	std::smatch sm;

	lines_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, C_REGEX_COORDS)) {
			lines_.push_back({{std::stoi(sm.str(1)), std::stoi(sm.str(2))}, {std::stoi(sm.str(3)), std::stoi(sm.str(4))}});
		} else {
			std::cout << "Invalid coordinates at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

int64_t AoC2021_day05::get_lines_overlap_points_count(const bool part2) {
	std::map<COORD2D, uint32_t> points;
	int64_t result = 0;
	int32_t dx, dy, points_cnt;
	COORD2D coord;

	for (size_t i = 0; i < lines_.size(); i++) {
		if (lines_[i].first.x == lines_[i].second.x) {
			for (int32_t y = std::min(lines_[i].first.y, lines_[i].second.y); y <= std::max(lines_[i].first.y, lines_[i].second.y); y++) {
				points[{lines_[i].first.x, y}]++;
			}
		} else if (lines_[i].first.y == lines_[i].second.y) {
			for (int32_t x = std::min(lines_[i].first.x, lines_[i].second.x); x <= std::max(lines_[i].first.x, lines_[i].second.x); x++) {
				points[{x, lines_[i].first.y}]++;
			}
		} else if (part2) {
			if (lines_[i].first.x < lines_[i].second.x) {
				dx = 1;
				points_cnt = lines_[i].second.x - lines_[i].first.x;

				if (lines_[i].first.y < lines_[i].second.y) {
					dy = 1;
				} else {
					dy = -1;
				}
			} else {
				dx = -1;
				points_cnt = lines_[i].first.x - lines_[i].second.x;

				if (lines_[i].first.y < lines_[i].second.y) {
					dy = 1;
				} else {
					dy = -1;
				}
			}

			for (int32_t j = 0; j <= points_cnt; j++) {
				coord = lines_[i].first;
				coord.x += dx * j;
				coord.y += dy * j;
				points[coord]++;
			}
		}
	}

	for (auto it = points.begin(); it != points.end(); it++) {
		if (it->second >= C_LINES_NUMBER_LIMIT) {
			result++;
		}
	}

	return result;
}

int32_t AoC2021_day05::get_aoc_day() {
	return 5;
}

int32_t AoC2021_day05::get_aoc_year() {
	return 2021;
}

void AoC2021_day05::tests() {
	int64_t result;

	if (init({"0,9 -> 5,9", "8,0 -> 0,8", "9,4 -> 3,4", "2,2 -> 2,1", "7,0 -> 7,4", "6,4 -> 2,0", "0,9 -> 2,9", "3,4 -> 1,4", "0,0 -> 8,8", "5,5 -> 8,2"})) {
		result = get_lines_overlap_points_count(false); // 5
		result = get_lines_overlap_points_count(true);	// 12
	}
}

bool AoC2021_day05::part1() {
	int64_t result = 0;

	result = get_lines_overlap_points_count(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day05::part2() {
	int64_t result = 0;

	result = get_lines_overlap_points_count(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day05 day05;

	return day05.main_execution();
}

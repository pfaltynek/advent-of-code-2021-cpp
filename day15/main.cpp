#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>

class AoC2021_day15 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, uint8_t> risk_levels_;
	void build_full_cave_map();
	int64_t find_lowest_risk_level_path();
	uint8_t increment_risk_level(const uint8_t level);
	uint32_t cave_size_;
};

struct path_step {
	coord_str position;
	size_t risk_level;
	std::vector<coord_str> history;
};

bool AoC2021_day15::init(const std::vector<std::string> lines) {
	risk_levels_.clear();

	cave_size_ = lines.size();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (lines[i].size() != cave_size_) {
			std::cout << "Invalid cave size at line " << i + 1 << std::endl;
			return false;
		}

		if (lines[i].find_first_not_of("0123456789") != std::string::npos) {
			std::cout << "Invalid cave content at line " << i + 1 << std::endl;
			return false;
		}

		for (uint32_t j = 0; j < lines[i].size(); j++) {
			risk_levels_[{j, i}] = lines[i][j] - '0';
		}
	}

	return true;
}

int64_t AoC2021_day15::find_lowest_risk_level_path() {
	std::queue<path_step> q{};
	path_step curr, next;
	std::vector<coord_str> dirs = {coord_step_south, coord_step_east, coord_step_north, coord_step_west};
	coord_str next_pos, final = {cave_size_ - 1, cave_size_ - 1};
	std::map<coord_str, uint32_t> risk_map;

	curr.risk_level = 0;
	curr.position = {0, 0};
	curr.history = {{0, 0}};
	q.emplace(curr);

	while (q.size()) {
		curr = q.front();
		q.pop();

		for (coord_str c : dirs) {
			next = curr;
			next.position = next.position + c;

			if (risk_levels_.count(next.position)) {
				next.risk_level += risk_levels_[next.position];

				if (risk_map.count(next.position)) {
					if (risk_map[next.position] > next.risk_level) {
						risk_map[next.position] = next.risk_level;
					} else {
						continue;
					}
				} else {
					risk_map[next.position] = next.risk_level;
				}

				q.emplace(next);
			}
		}
	}

	return risk_map[final];
}

void AoC2021_day15::build_full_cave_map() {
	uint8_t risk_level = 1;

	for (uint32_t x = 0; x < cave_size_; x++) {
		for (uint32_t y = 0; y < cave_size_; y++) {
			risk_level = risk_levels_[{x, y}];
			for (uint32_t i = 1; i < 5; i++) {
				risk_level = increment_risk_level(risk_level);
				risk_levels_[{x + (i * cave_size_), y}] = risk_level;
			}
		}
	}

	for (uint32_t x = 0; x < 5 * cave_size_; x++) {
		for (uint32_t y = 0; y < cave_size_; y++) {
			risk_level = risk_levels_[{x, y}];
			for (uint32_t i = 1; i < 5; i++) {
				risk_level = increment_risk_level(risk_level);
				risk_levels_[{x, y + (i * cave_size_)}] = risk_level;
			}
		}
	}
	cave_size_ *= 5;
}

uint8_t AoC2021_day15::increment_risk_level(const uint8_t level) {
	if (level == 9) {
		return 1;
	} else {
		return level + 1;
	}
}

int32_t AoC2021_day15::get_aoc_day() {
	return 15;
}

int32_t AoC2021_day15::get_aoc_year() {
	return 2021;
}

void AoC2021_day15::tests() {
	int64_t result;

	if (init({"1163751742", "1381373672", "2136511328", "3694931569", "7463417111", "1319128137", "1359912421", "3125421639", "1293138521", "2311944581"})) {
		result = find_lowest_risk_level_path(); // 40
		build_full_cave_map();
		result = find_lowest_risk_level_path(); // 315
	}
}

bool AoC2021_day15::part1() {
	int64_t result = 0;

	result = find_lowest_risk_level_path();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day15::part2() {
	int64_t result = 0;

	build_full_cave_map();

	result = find_lowest_risk_level_path();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day15 day15;

	return day15.main_execution();
}

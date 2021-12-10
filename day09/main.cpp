#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>

class AoC2021_day09 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, uint8_t> heightmap_;
	uint16_t map_width_, map_height_;
	int64_t get_risk_levels_sum() const;
	std::vector<coord_str> get_low_points() const;
	bool is_low_point(const coord_str pos) const;
	int64_t get_3largest_basins_size() const;
	int64_t get_basin_size(const coord_str pos) const;
};

bool AoC2021_day09::init(const std::vector<std::string> lines) {
	heightmap_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789") != std::string::npos) {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		if (i) {
			if (lines[i].size() != map_width_) {
				std::cout << "Invalid input size at line " << i + 1 << std::endl;
				return false;
			}
		} else {
			map_width_ = lines[i].size();
		}

		for (size_t j = 0; j < lines[i].size(); j++) {
			heightmap_[{static_cast<int32_t>(j), static_cast<int32_t>(i)}] = lines[i][j] - '0';
		}
	}

	map_height_ = lines.size();

	return true;
}

int64_t AoC2021_day09::get_risk_levels_sum() const {
	int64_t result = 0;
	std::vector<coord_str> low_points;

	low_points = get_low_points();

	for (size_t i = 0; i < low_points.size(); i++) {
		result += heightmap_.at(low_points[i]) + 1;
	}

	return result;
}

bool AoC2021_day09::is_low_point(const coord_str pos) const {
	coord_str tmp;
	uint8_t value = heightmap_.at(pos);

	if (pos.x > 0) {
		tmp = pos;
		tmp = tmp + coord_step_west;
		if (heightmap_.at(tmp) <= value) {
			return false;
		}
	}

	if (pos.y > 0) {
		tmp = pos;
		tmp = tmp + coord_step_north;
		if (heightmap_.at(tmp) <= value) {
			return false;
		}
	}

	if (pos.x + 1 < map_width_) {
		tmp = pos;
		tmp = tmp + coord_step_east;
		if (heightmap_.at(tmp) <= value) {
			return false;
		}
	}

	if (pos.y + 1 < map_height_) {
		tmp = pos;
		tmp = tmp + coord_step_south;
		if (heightmap_.at(tmp) <= value) {
			return false;
		}
	}

	return true;
}

std::vector<coord_str> AoC2021_day09::get_low_points() const {
	std::vector<coord_str> result = {};

	for (auto it = heightmap_.begin(); it != heightmap_.end(); it++) {
		if (is_low_point(it->first)) {
			result.push_back(it->first);
		}
	}

	return result;
}

int64_t AoC2021_day09::get_3largest_basins_size() const {
	std::vector<int64_t> basin_sizes{};
	std::vector<coord_str> low_points;
	int64_t result = 1;

	low_points = get_low_points();

	for (size_t i = 0; i < low_points.size(); i++) {
		basin_sizes.push_back(get_basin_size(low_points[i]));
	}

	std::sort(basin_sizes.begin(), basin_sizes.end());
	std::reverse(basin_sizes.begin(), basin_sizes.end());

	for (size_t i = 0; i < std::min(static_cast<size_t>(3), basin_sizes.size()); i++) {
		result *= basin_sizes[i];
	}

	return result;
}

int64_t AoC2021_day09::get_basin_size(const coord_str pos) const {
	int64_t result = 0;
	std::queue<coord_str> q = {};
	std::set<coord_str> history = {};
	coord_str curr;

	q.emplace(pos);
	result = 0;

	while (q.size()) {
		curr = q.front();
		q.pop();

		if (history.count(curr)){
			continue;
		}

		history.emplace(curr);

		if (heightmap_.count(curr)) {
			if (heightmap_.at(curr) < 9) {
				result++;

				q.emplace(curr + coord_step_north);
				q.emplace(curr + coord_step_south);
				q.emplace(curr + coord_step_east);
				q.emplace(curr + coord_step_west);
			}
		}
	}

	return result;
}

int32_t AoC2021_day09::get_aoc_day() {
	return 8;
}

int32_t AoC2021_day09::get_aoc_year() {
	return 2021;
}

void AoC2021_day09::tests() {
	int64_t result;

	if (init({"2199943210", "3987894921", "9856789892", "8767896789", "9899965678"})) {
		result = get_risk_levels_sum(); // 15
		result = get_3largest_basins_size(); // 1134
	}
}

bool AoC2021_day09::part1() {
	int64_t result = 0;

	result = get_risk_levels_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day09::part2() {
	int64_t result = 0;

	result = get_3largest_basins_size();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day09 day09;

	return day09.main_execution();
}

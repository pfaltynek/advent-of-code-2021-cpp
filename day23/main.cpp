#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <set>

const std::map<char, uint16_t> C_STEP_COST = {{'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};

class AoC2021_day23 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<coord_str> map_;
	std::map<char, std::vector<coord_str>> amphipods_;
	std::vector<int32_t> rooms_x_, rooms_y_;
	std::map<char, int32_t> roomsx_by_amphipods_;

	uint32_t get_least_energy_path();
};

bool AoC2021_day23::init(const std::vector<std::string> lines) {
	std::map<int32_t, uint8_t> xcnt, ycnt;
	int32_t tmp;

	map_.clear();
	amphipods_.clear();
	xcnt.clear();
	ycnt.clear();
	rooms_x_.clear();
	rooms_y_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		for (uint32_t j = 0; j < lines[i].size(); j++) {
			switch (lines[i][j]) {
				case ' ':
					// ignore space symbol
				case '#':
					// ignore wall symbol
					break;
				case '.':
					map_.emplace(coord_str(j, i));
					break;
				case 'A':
					// map_.emplace(coord_str(j, i));
					amphipods_[lines[i][j]].push_back({j, i});
					break;
				case 'B':
					// map_.emplace(coord_str(j, i));
					amphipods_[lines[i][j]].push_back({j, i});
					break;
				case 'C':
					// map_.emplace(coord_str(j, i));
					amphipods_[lines[i][j]].push_back({j, i});
					break;
				case 'D':
					// map_.emplace(coord_str(j, i));
					amphipods_[lines[i][j]].push_back({j, i});
					break;
				default:
					std::cout << "Invalid map symbol on line " << i + 1 << " position " << j + 1 << std::endl;
					return false;
			}
		}
	}

	tmp = -1;
	for (const auto& pair : map_) {
		if (tmp < 0) {
			tmp = pair.y;
		} else if (pair.y != tmp) {
			std::cout << "Invalid map format (unsupported hallway size)" << std::endl;
			return false;
		}
	}

	for (char i = 'A'; i <= 'D'; i++) {
		if (amphipods_[i].size() != 2) {
			std::cout << "Invalid map - symbol " << i << " not found in required count" << std::endl;
			return false;
		}

		xcnt[amphipods_[i][0].x]++;
		xcnt[amphipods_[i][1].x]++;
		ycnt[amphipods_[i][0].y]++;
		ycnt[amphipods_[i][1].y]++;
	}

	if ((ycnt.size() != 2) || (xcnt.size() != 4)) {
		std::cout << "Invalid map format (unsupported amphipod room(s) size)" << std::endl;
		return false;
	}

	for (const auto& pair : xcnt) {
		if (pair.second != 2) {
			std::cout << "Invalid map format (unsupported amphipod room(s) size)" << std::endl;
			return false;
		}

		rooms_x_.push_back(pair.first);
	}

	for (const auto& pair : ycnt) {
		if (pair.second != 4) {
			std::cout << "Invalid map format (unsupported amphipod room(s) size)" << std::endl;
			return false;
		}

		rooms_y_.push_back(pair.first);
	}

	std::sort(rooms_x_.begin(), rooms_x_.end());
	std::sort(rooms_y_.begin(), rooms_y_.end());

	roomsx_by_amphipods_.clear();

	for (size_t i = 0; i < rooms_x_.size(); i++) {
		roomsx_by_amphipods_[i + 'A'] = rooms_x_[i];
	}

	return true;
}

uint32_t AoC2021_day23::get_least_energy_path() {
	std::set<coord_str> hallway_targets = {};
	uint32_t result = 0;

	for (const auto& pos : map_) {
		if (std::find(rooms_x_.begin(), rooms_x_.end(), pos.x) == rooms_x_.end()) {
			hallway_targets.emplace(pos);
		}
	}

	return result;
}

int32_t AoC2021_day23::get_aoc_day() {
	return 23;
}

int32_t AoC2021_day23::get_aoc_year() {
	return 2021;
}

void AoC2021_day23::tests() {
	int64_t result;

	if (init({"#############", "#...........#", "###B#C#B#D###", "  #A#D#C#A#", "  #########"})) {
		result = get_least_energy_path(); //12521
	}
}

bool AoC2021_day23::part1() {
	int64_t result = 0;

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day23::part2() {
	int64_t result = 0;

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day23 day23;

	return day23.main_execution();
}

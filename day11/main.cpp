#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <queue>
#include <set>

class AoC2021_day11 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<coord_str, uint32_t> octopuses_, backup_;
	uint8_t grid_size_;
	int64_t calculate_flashes(const size_t steps, const bool part2);
	void print(const uint16_t step, const int64_t flashes) const;
	void reset();
};

bool AoC2021_day11::init(const std::vector<std::string> lines) {
	octopuses_.clear();

	grid_size_ = lines.size();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789") != std::string::npos) {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		if (lines[i].size() != grid_size_) {
			std::cout << "Invalid input size at line " << i + 1 << std::endl;
			return false;
		}

		for (size_t j = 0; j < lines[i].size(); j++) {
			octopuses_[{static_cast<int32_t>(j), static_cast<int32_t>(i)}] = static_cast<uint32_t>(lines[i][j] - '0');
		}
	}

	backup_ = octopuses_;

	return true;
}

void AoC2021_day11::reset(){
	octopuses_ = backup_;
}

int64_t AoC2021_day11::calculate_flashes(const size_t steps, const bool part2) {
	int64_t flashes = 0;
	std::queue<coord_str> q;
	std::vector<coord_str> adjacents = {coord_step_north,	   coord_step_south,	  coord_step_west,		 coord_step_east,
										coord_step_north_east, coord_step_south_east, coord_step_north_west, coord_step_south_west};
	coord_str curr, next;
	bool finished = false, all0;
	size_t s = 0;

	reset();

	//print(0, flashes);

	while (!finished) {
		q = {};
		for (auto it = octopuses_.begin(); it != octopuses_.end(); it++) {
			it->second++;
			if (it->second > 9) {
				q.emplace(it->first);
			}
		}

		while (q.size()) {
			curr = q.front();
			q.pop();

			for (size_t a = 0; a < adjacents.size(); a++) {
				next = curr + adjacents[a];

				if (!octopuses_.count(next)) {
					continue;
				}

				if (octopuses_[next] <= 9) {
					octopuses_[next]++;
					if (octopuses_[next] > 9) {
						q.emplace(next);
					}
				}
			}
		}

		all0 = true;
		for (auto it = octopuses_.begin(); it != octopuses_.end(); it++) {
			if (it->second > 9) {
				it->second = 0;
				flashes++;
			} else {
				all0 = false;
			}
		}

		s++;

		if (part2) {
			finished = all0;
		} else {
			finished = s >= steps;
		}

	//	print(s, flashes);
	}

	if (part2) {
		return s;
	} else {
		return flashes;
	}
}

void AoC2021_day11::print(const uint16_t step, const int64_t flashes) const {
	char c;

	std::cout << std::endl;

	if (step) {
		std::cout << "After step " << step << ":" << std::endl;
	} else {
		std::cout << "Before any steps:" << std::endl;
	}

	for (size_t y = 0; y < grid_size_; y++) {
		for (size_t x = 0; x < grid_size_; x++) {
			c = static_cast<char>(octopuses_.at({static_cast<int32_t>(x), static_cast<int32_t>(y)}) + '0');
			std::cout << c;
		}
		std::cout << std::endl;
	}
	std::cout << "Flashes: " << flashes;

	std::cout << std::endl;
}

int32_t AoC2021_day11::get_aoc_day() {
	return 11;
}

int32_t AoC2021_day11::get_aoc_year() {
	return 2021;
}

void AoC2021_day11::tests() {
	int64_t result;

	if (init({"11111", "19991", "19191", "19991", "11111"})) {
		result = calculate_flashes(2, false); // 9
	}

	if (init({"5483143223", "2745854711", "5264556173", "6141336146", "6357385478", "4167524645", "2176841721", "6882881134", "4846848554", "5283751526"})) {
		result = calculate_flashes(100,false); // 1656
		result = calculate_flashes(100,true); // 1656
	}
}

bool AoC2021_day11::part1() {
	int64_t result = 0;

	result = calculate_flashes(100, false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day11::part2() {
	int64_t result = 0;

	result = calculate_flashes(0, true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day11 day11;

	return day11.main_execution();
}

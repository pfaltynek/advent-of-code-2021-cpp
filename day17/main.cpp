#include "./../common/aoc.hpp"
#include <algorithm>
#include <numeric>
#include <regex>

const std::regex C_TARGET_AREA_TEMPLATE("^target area: x=(\\d+)..(\\d+), y=(-?\\d+)..(-?\\d+)$");

class AoC2021_day17 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int32_t target_x1_, target_x2_, target_y1_, target_y2_;
	int32_t get_highest_y_position() const;
	int32_t get_next_x_velocity(const int32_t x_velocity) const;
	int32_t get_next_y_velocity(const int32_t y_velocity) const;
	int32_t get_highest_y_position(const int32_t x_vel, const int32_t y_vel) const;
};

bool AoC2021_day17::init(const std::vector<std::string> lines) {
	std::smatch sm;

	if (lines.size() < 1) {
		return false;
	}

	if (std::regex_match(lines[0], sm, C_TARGET_AREA_TEMPLATE)) {
		target_x1_ = static_cast<int32_t>(std::stol(sm.str(1)));
		target_x2_ = static_cast<int32_t>(std::stol(sm.str(2)));
		target_y1_ = static_cast<int32_t>(std::stol(sm.str(3)));
		target_y2_ = static_cast<int32_t>(std::stol(sm.str(4)));

		return true;
	}

	return false;
}

int32_t AoC2021_day17::get_highest_y_position() const {
	std::vector<int32_t> velx = {};
	int32_t tmp = 0, inc = 1, max_y = 0;

	// get max x velocity to fit target:
	while (tmp <= target_x2_) {
		tmp += inc;

		if ((tmp <= target_x2_) && (tmp >= target_x1_)) {
			velx.push_back(inc);
		}
		inc++;
	}

	for (size_t i = 0; i < velx.size(); i++) {
		for (size_t j = abs(target_y2_); j <= abs(target_y1_); j++) {
			tmp = get_highest_y_position(velx[i], j);

			if (tmp > max_y) {
				max_y = tmp;
			}
		}
	}

	return max_y;
}

int32_t AoC2021_day17::get_next_x_velocity(const int32_t x_velocity) const {
	if (x_velocity > 0) {
		return x_velocity - 1;
	} else if (x_velocity < 0) {
		return x_velocity + 1;
	} else {
		return 0;
	}
}

int32_t AoC2021_day17::get_next_y_velocity(const int32_t y_velocity) const {
	return y_velocity - 1;
}

int32_t AoC2021_day17::get_highest_y_position(int32_t x_vel, int32_t y_vel) const {
	int32_t max_y = 0, x = 0, y = 0;
	bool in_target = false;

	while ((x <= target_x2_) && (y >= target_y1_)) {
		x += x_vel;
		y += y_vel;

		if ((target_x1_ <= x) && (target_x2_ >= x) && (target_y1_ <= y) && (target_y2_ >= y)) {
			in_target = true;
		}

		if (y > max_y) {
			max_y = y;
		}

		x_vel = get_next_x_velocity(x_vel);
		y_vel = get_next_y_velocity(y_vel);
	}

	if (in_target) {
		return max_y;
	} else {
		return 0;
	}
}

int32_t AoC2021_day17::get_aoc_day() {
	return 17;
}

int32_t AoC2021_day17::get_aoc_year() {
	return 2021;
}

void AoC2021_day17::tests() {
	int64_t result;

	if (init({"target area: x=20..30, y=-10..-5"})) {
		result = get_highest_y_position(); // 45
	}
}

bool AoC2021_day17::part1() {
	int64_t result = 0;

	result = get_highest_y_position();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day17::part2() {
	int64_t result = 0;

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day17 day17;

	return day17.main_execution();
}

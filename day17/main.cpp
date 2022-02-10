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
	std::vector<int32_t> velx_;
	int32_t get_highest_y_position();
	int32_t get_possible_hit_count();
	int32_t get_next_x_velocity(const int32_t x_velocity) const;
	int32_t get_next_y_velocity(const int32_t y_velocity) const;
	bool check_probe_target_hit(int32_t x_vel, int32_t y_vel, int32_t& highest_y_position) const;
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

int32_t AoC2021_day17::get_highest_y_position() {
	int32_t tmp = 0, inc = 1, max_y = 0;

	velx_.clear();

	// get max x velocity to fit target:
	while (tmp <= target_x2_) {
		tmp += inc;

		if ((tmp <= target_x2_) && (tmp >= target_x1_)) {
			velx_.push_back(inc);
		}
		inc++;
	}

	for (size_t i = 0; i < velx_.size(); i++) {
		for (size_t j = abs(target_y2_); j <= abs(target_y1_); j++) {
			if (check_probe_target_hit(velx_[i], j, tmp)) {
				if (tmp > max_y) {
					max_y = tmp;
				}
			}
		}
	}

	return max_y;
}

int32_t AoC2021_day17::get_possible_hit_count() {
	int32_t xmax = *std::max_element(velx_.begin(), velx_.end());
	int32_t tmp, velx;
	int32_t result = 0;

	for (int32_t i = xmax + 1; i <= target_x2_; i++) {
		tmp = 0;
		velx = i;

		while (tmp <= target_x2_) {
			tmp += velx;
			velx = get_next_x_velocity(velx);

			if ((tmp <= target_x2_) && (tmp >= target_x1_)) {
				velx_.push_back(i);
				break;
			}
		}
	}

	for (size_t i = 0; i < velx_.size(); i++) {
		for (int32_t j = std::min(target_y1_, target_y2_); j <= std::max(abs(target_y2_), abs(target_y1_)); j++) {
			if (check_probe_target_hit(velx_[i], j, tmp)) {
				result++;
			}
		}
	}

	return result;
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

bool AoC2021_day17::check_probe_target_hit(int32_t x_vel, int32_t y_vel, int32_t& highest_y_position) const {
	int32_t x = 0, y = 0;
	bool result = false;

	highest_y_position = INT32_MIN;

	while ((x <= target_x2_) && (y >= target_y1_)) {
		x += x_vel;
		y += y_vel;

		if ((target_x1_ <= x) && (target_x2_ >= x) && (target_y1_ <= y) && (target_y2_ >= y)) {
			result = true;
		}

		if (y > highest_y_position) {
			highest_y_position = y;
		}

		x_vel = get_next_x_velocity(x_vel);
		y_vel = get_next_y_velocity(y_vel);
	}

	return result;
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
		result = get_possible_hit_count(); // 112
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

	result = get_possible_hit_count();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day17 day17;

	return day17.main_execution();
}

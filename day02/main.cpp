#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>

const std::regex C_REGEX_FORWARD("^forward (\\d+)$");
const std::regex C_REGEX_UP("^up (\\d+)$");
const std::regex C_REGEX_DOWN("^down (\\d+)$");

enum direction_t { DIR_FWD, DIR_DWN, DIR_UP };

typedef std::pair<direction_t, uint16_t> course_command;

class AoC2021_day02 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	int64_t navigate_course();
	int64_t navigate_course_with_aim();
	std::vector<course_command> course_;
};

bool AoC2021_day02::init(const std::vector<std::string> lines) {
	std::smatch sm;
	course_command cc{};

	course_.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, C_REGEX_DOWN)) {
			cc.first = direction_t::DIR_DWN;
			cc.second = static_cast<uint16_t>(std::stoi(sm.str(1)));
		} else if (std::regex_match(lines[i], sm, C_REGEX_UP)) {
			cc.first = direction_t::DIR_UP;
			cc.second = static_cast<uint16_t>(std::stoi(sm.str(1)));
		} else if (std::regex_match(lines[i], sm, C_REGEX_FORWARD)) {
			cc.first = direction_t::DIR_FWD;
			cc.second = static_cast<uint16_t>(std::stoi(sm.str(1)));
		} else {
			std::cout << "Invalid course command at line " << i + 1 << std::endl;
			return false;
		}

		course_.push_back(cc);
	}

	return true;
}

int64_t AoC2021_day02::navigate_course() {
	COORD2D pos{};
	int64_t result;

	for (size_t i = 0; i < course_.size(); i++) {
		switch (course_[i].first) {
			case direction_t::DIR_DWN:
				pos.y += course_[i].second;
				break;
			case direction_t::DIR_UP:
				pos.y -= course_[i].second;
				break;
			case direction_t::DIR_FWD:
				pos.x += course_[i].second;
				break;
			default:
				return -1;
				break;
		}
	}

	result = pos.x * pos.y;

	return result;
}

int64_t AoC2021_day02::navigate_course_with_aim() {
	COORD3D pos{};
	int64_t result;

	for (size_t i = 0; i < course_.size(); i++) {
		switch (course_[i].first) {
			case direction_t::DIR_DWN:
				pos.z += course_[i].second;
				break;
			case direction_t::DIR_UP:
				pos.z -= course_[i].second;
				break;
			case direction_t::DIR_FWD:
				pos.x += course_[i].second;
				pos.y += course_[i].second * pos.z;
				break;
			default:
				return -1;
				break;
		}
	}

	result = pos.x * pos.y;

	return result;
}

int32_t AoC2021_day02::get_aoc_day() {
	return 2;
}

int32_t AoC2021_day02::get_aoc_year() {
	return 2021;
}

void AoC2021_day02::tests() {
	int64_t result;

	if (init({"forward 5", "down 5", "forward 8", "up 3", "down 8", "forward 2"})) {
		result = navigate_course();			 // 150
		result = navigate_course_with_aim(); // 900
	}
}

bool AoC2021_day02::part1() {
	int64_t result = 0;

	result = navigate_course();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day02::part2() {
	int64_t result = 0;

	result = navigate_course_with_aim();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day02 day02;

	return day02.main_execution();
}

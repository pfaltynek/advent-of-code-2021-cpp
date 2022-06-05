#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>
#include <set>

const std::regex C_REBOOT_TEMPLATE("^(on|off) x=(-?\\d+)..(-?\\d+),y=(-?\\d+)..(-?\\d+),z=(-?\\d+)..(-?\\d+)$");

class AoC2021_day22 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<std::tuple<std::tuple<coord_str, coord_str, coord_str>, bool>> reboots_;
	void print() const;
	size_t execute_reboot_steps() const;
};

bool AoC2021_day22::init(const std::vector<std::string> lines) {
	std::smatch sm;
	coord_str cx, cy, cz;
	bool on;

	reboots_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, C_REBOOT_TEMPLATE)) {
			on = sm.str(1) == "on";
			cx.x = std::stoi(sm.str(2));
			cx.y = std::stoi(sm.str(3));
			cy.x = std::stoi(sm.str(4));
			cy.y = std::stoi(sm.str(5));
			cz.x = std::stoi(sm.str(6));
			cz.y = std::stoi(sm.str(7));
			reboots_.push_back({{cx, cy, cz}, on});
		} else {
			std::cout << "Invalid reboot record on line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

size_t AoC2021_day22::execute_reboot_steps() const {
	std::set<coord_3d_str> cubes = {};
	std::tuple<coord_str, coord_str, coord_str> cuboid;
	coord_str cx, cy, cz;
	bool on;

	for (const auto& item : reboots_) {
		std::tie(cuboid, on) = item;
		std::tie(cx, cy, cz) = cuboid;

		if ((cx.x <= 50) && (cx.x >= -50) && (cx.y <= 50) && (cx.y >= -50) && (cy.x <= 50) && (cy.x >= -50) && (cy.y <= 50) && (cy.y >= -50) && (cz.x <= 50) &&
			(cz.x >= -50) && (cz.y <= 50) && (cz.y >= -50)) {
			for (int32_t x = cx.x; x <= cx.y; x++) {
				for (int32_t y = cy.x; y <= cy.y; y++) {
					for (int32_t z = cz.x; z <= cz.y; z++) {
						if (on) {
							cubes.insert({x, y, z});
						} else {
							cubes.erase({x, y, z});
						}
					}
				}
			}
		}
	}

	return cubes.size();
}

int32_t AoC2021_day22::get_aoc_day() {
	return 22;
}

int32_t AoC2021_day22::get_aoc_year() {
	return 2021;
}

void AoC2021_day22::tests() {
	int64_t result;

	if (init({"on x=10..12,y=10..12,z=10..12", "on x=11..13,y=11..13,z=11..13", "off x=9..11,y=9..11,z=9..11", "on x=10..10,y=10..10,z=10..10"})) {
		result = execute_reboot_steps(); // 39
	}
	if (init({"on x=-20..26,y=-36..17,z=-47..7",
			  "on x=-20..33,y=-21..23,z=-26..28",
			  "on x=-22..28,y=-29..23,z=-38..16",
			  "on x=-46..7,y=-6..46,z=-50..-1",
			  "on x=-49..1,y=-3..46,z=-24..28",
			  "on x=2..47,y=-22..22,z=-23..27",
			  "on x=-27..23,y=-28..26,z=-21..29",
			  "on x=-39..5,y=-6..47,z=-3..44",
			  "on x=-30..21,y=-8..43,z=-13..34",
			  "on x=-22..26,y=-27..20,z=-29..19",
			  "off x=-48..-32,y=26..41,z=-47..-37",
			  "on x=-12..35,y=6..50,z=-50..-2",
			  "off x=-48..-32,y=-32..-16,z=-15..-5",
			  "on x=-18..26,y=-33..15,z=-7..46",
			  "off x=-40..-22,y=-38..-28,z=23..41",
			  "on x=-16..35,y=-41..10,z=-47..6",
			  "off x=-32..-23,y=11..30,z=-14..3",
			  "on x=-49..-5,y=-3..45,z=-29..18",
			  "off x=18..30,y=-20..-8,z=-3..13",
			  "on x=-41..9,y=-7..43,z=-33..15",
			  "on x=-54112..-39298,y=-85059..-49293,z=-27449..7877",
			  "on x=967..23432,y=45373..81175,z=27513..53682"})) {
		result = execute_reboot_steps(); // 590784
	}
}

bool AoC2021_day22::part1() {
	int64_t result = 0;

	result = execute_reboot_steps();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day22::part2() {
	int64_t result = 0;

	// result = simulate_Dirac_dice_game();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day22 day22;

	return day22.main_execution();
}

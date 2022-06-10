#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <regex>

const std::regex C_REBOOT_TEMPLATE("^(on|off) x=(-?\\d+)..(-?\\d+),y=(-?\\d+)..(-?\\d+),z=(-?\\d+)..(-?\\d+)$");

struct cuboid_str {
	int32_t minX, maxX, minY, maxY, minZ, maxZ;

	inline cuboid_str() {
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
		minZ = 0;
		maxZ = 0;
	}

	inline cuboid_str(const int32_t minX_init, const int32_t maxX_init, const int32_t minY_init, const int32_t maxY_init, const int32_t minZ_init,
					  const int32_t maxZ_init) {
		minX = minX_init;
		maxX = maxX_init;
		minY = minY_init;
		maxY = maxY_init;
		minZ = minZ_init;
		maxZ = maxZ_init;
	}

	inline bool is_inside_of(const cuboid_str& outer) const {
		return ((minX <= outer.maxX) && (minX >= outer.minX) && (maxX <= outer.maxX) && (maxX >= outer.minX) && (minY <= outer.maxY) && (minY >= outer.minY) &&
				(maxY <= outer.maxY) && (maxY >= outer.minY) && (minZ <= outer.maxZ) && (minZ >= outer.minZ) && (maxZ <= outer.maxZ) && (maxZ >= outer.minZ));
	}

	inline bool intersecting(const cuboid_str& other) const {
		if (minX > other.maxX || maxX < other.minX || minY > other.maxY || maxY < other.minY || minZ > other.maxZ || maxZ < other.minZ) {
			return false;
		} else {
			return true;
		}
	}

	inline cuboid_str get_intersection(const cuboid_str& other) const {
		return cuboid_str(std::max(minX, other.minX), std::min(maxX, other.maxX), std::max(minY, other.minY), std::min(maxY, other.maxY),
						  std::max(minZ, other.minZ), std::min(maxZ, other.maxZ));
	}

	inline size_t size() const {
		size_t x, y, z;

		x = static_cast<size_t>(1 + maxX - minX);
		y = static_cast<size_t>(1 + maxY - minY);
		z = static_cast<size_t>(1 + maxZ - minZ);
		return x * y * z;
	}

	inline bool operator<(const cuboid_str& other) const {
		return size() < other.size();
	}
};

const cuboid_str C_PART1_LIMIT = {-50, 50, -50, 50, -50, 50};

struct reboot_record_str {
	cuboid_str cuboid;
	bool on;
};

class AoC2021_day22 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<reboot_record_str> reboots_;
	void print() const;
	size_t execute_reboot_steps(const bool part1) const;
};

bool AoC2021_day22::init(const std::vector<std::string> lines) {
	std::smatch sm;
	cuboid_str cb;
	bool on;

	reboots_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (std::regex_match(lines[i], sm, C_REBOOT_TEMPLATE)) {
			on = sm.str(1) == "on";

			cb.minX = std::stoi(sm.str(2));
			cb.maxX = std::stoi(sm.str(3));
			cb.minY = std::stoi(sm.str(4));
			cb.maxY = std::stoi(sm.str(5));
			cb.minZ = std::stoi(sm.str(6));
			cb.maxZ = std::stoi(sm.str(7));

			if ((cb.minX > cb.maxX) || (cb.minY > cb.maxY) || (cb.minZ > cb.maxZ)) {
				std::cout << "Invalid reboot record parameters order on line " << i + 1 << std::endl;
				return false;
			}

			reboots_.push_back({cb, on});
		} else {
			std::cout << "Invalid reboot record on line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

/*
 *	main function redesigned because this cubes counting is not able to finish part 2 in reasonable time
 *	replace by implementation of sets union and intersection theory:
		size of A and B union = size of A + size of B - size of intersection A and B
		thats why for each cuboid before added into set of unions is checked the intersection with any
		previous cuboid; when found it is put into intersections set
		result is sum of all cuboid sizes of unions set minus sum of all cuboid sizes in intersections set
		original idea from reddit solutions thread https://www.reddit.com/r/adventofcode/comments/rlxhmg/2021_day_22_solutions/
 */
size_t AoC2021_day22::execute_reboot_steps(const bool part1) const {
	std::vector<reboot_record_str> results, to_add;
	cuboid_str intersection;
	size_t result = 0;

	results.clear();

	for (const auto& reboot : reboots_) {
		if (part1) {
			if (!reboot.cuboid.is_inside_of(C_PART1_LIMIT)) {
				continue;
			}
		}

		to_add.clear();

		for (const auto& done : results) {
			if (reboot.cuboid.intersecting(done.cuboid)) {
				intersection = reboot.cuboid.get_intersection(done.cuboid);
				to_add.push_back({intersection, !done.on});
			}
		}

		results.insert(results.end(), to_add.begin(), to_add.end());

		if (reboot.on) {
			results.push_back(reboot);
		}
	}

	for (const auto& item : results) {
		if (item.on) {
			result += item.cuboid.size();
		} else {
			result -= item.cuboid.size();
		}
	}

	return result;
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
		result = execute_reboot_steps(true); // 39
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
		result = execute_reboot_steps(true); // 590784
	}

	if (init({"on x=-5..47,y=-31..22,z=-19..33",
			  "on x=-44..5,y=-27..21,z=-14..35",
			  "on x=-49..-1,y=-11..42,z=-10..38",
			  "on x=-20..34,y=-40..6,z=-44..1",
			  "off x=26..39,y=40..50,z=-2..11",
			  "on x=-41..5,y=-41..6,z=-36..8",
			  "off x=-43..-33,y=-45..-28,z=7..25",
			  "on x=-33..15,y=-32..19,z=-34..11",
			  "off x=35..47,y=-46..-34,z=-11..5",
			  "on x=-14..36,y=-6..44,z=-16..29",
			  "on x=-57795..-6158,y=29564..72030,z=20435..90618",
			  "on x=36731..105352,y=-21140..28532,z=16094..90401",
			  "on x=30999..107136,y=-53464..15513,z=8553..71215",
			  "on x=13528..83982,y=-99403..-27377,z=-24141..23996",
			  "on x=-72682..-12347,y=18159..111354,z=7391..80950",
			  "on x=-1060..80757,y=-65301..-20884,z=-103788..-16709",
			  "on x=-83015..-9461,y=-72160..-8347,z=-81239..-26856",
			  "on x=-52752..22273,y=-49450..9096,z=54442..119054",
			  "on x=-29982..40483,y=-108474..-28371,z=-24328..38471",
			  "on x=-4958..62750,y=40422..118853,z=-7672..65583",
			  "on x=55694..108686,y=-43367..46958,z=-26781..48729",
			  "on x=-98497..-18186,y=-63569..3412,z=1232..88485",
			  "on x=-726..56291,y=-62629..13224,z=18033..85226",
			  "on x=-110886..-34664,y=-81338..-8658,z=8914..63723",
			  "on x=-55829..24974,y=-16897..54165,z=-121762..-28058",
			  "on x=-65152..-11147,y=22489..91432,z=-58782..1780",
			  "on x=-120100..-32970,y=-46592..27473,z=-11695..61039",
			  "on x=-18631..37533,y=-124565..-50804,z=-35667..28308",
			  "on x=-57817..18248,y=49321..117703,z=5745..55881",
			  "on x=14781..98692,y=-1341..70827,z=15753..70151",
			  "on x=-34419..55919,y=-19626..40991,z=39015..114138",
			  "on x=-60785..11593,y=-56135..2999,z=-95368..-26915",
			  "on x=-32178..58085,y=17647..101866,z=-91405..-8878",
			  "on x=-53655..12091,y=50097..105568,z=-75335..-4862",
			  "on x=-111166..-40997,y=-71714..2688,z=5609..50954",
			  "on x=-16602..70118,y=-98693..-44401,z=5197..76897",
			  "on x=16383..101554,y=4615..83635,z=-44907..18747",
			  "off x=-95822..-15171,y=-19987..48940,z=10804..104439",
			  "on x=-89813..-14614,y=16069..88491,z=-3297..45228",
			  "on x=41075..99376,y=-20427..49978,z=-52012..13762",
			  "on x=-21330..50085,y=-17944..62733,z=-112280..-30197",
			  "on x=-16478..35915,y=36008..118594,z=-7885..47086",
			  "off x=-98156..-27851,y=-49952..43171,z=-99005..-8456",
			  "off x=2032..69770,y=-71013..4824,z=7471..94418",
			  "on x=43670..120875,y=-42068..12382,z=-24787..38892",
			  "off x=37514..111226,y=-45862..25743,z=-16714..54663",
			  "off x=25699..97951,y=-30668..59918,z=-15349..69697",
			  "off x=-44271..17935,y=-9516..60759,z=49131..112598",
			  "on x=-61695..-5813,y=40978..94975,z=8655..80240",
			  "off x=-101086..-9439,y=-7088..67543,z=33935..83858",
			  "off x=18020..114017,y=-48931..32606,z=21474..89843",
			  "off x=-77139..10506,y=-89994..-18797,z=-80..59318",
			  "off x=8476..79288,y=-75520..11602,z=-96624..-24783",
			  "on x=-47488..-1262,y=24338..100707,z=16292..72967",
			  "off x=-84341..13987,y=2429..92914,z=-90671..-1318",
			  "off x=-37810..49457,y=-71013..-7894,z=-105357..-13188",
			  "off x=-27365..46395,y=31009..98017,z=15428..76570",
			  "off x=-70369..-16548,y=22648..78696,z=-1892..86821",
			  "on x=-53470..21291,y=-120233..-33476,z=-44150..38147",
			  "off x=-93533..-4276,y=-16170..68771,z=-104985..-24507"})) {
		result = execute_reboot_steps(true);  // 474140
		result = execute_reboot_steps(false); // 2758514936282235
	}
}

bool AoC2021_day22::part1() {
	int64_t result = 0;

	result = execute_reboot_steps(true);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day22::part2() {
	int64_t result = 0;

	result = execute_reboot_steps(false);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day22 day22;

	return day22.main_execution();
}

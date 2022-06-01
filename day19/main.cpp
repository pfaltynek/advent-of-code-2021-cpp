#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <map>
#include <regex>
#include <set>

const std::regex C_REGEX_SCANNER_ID("^--- scanner (\\d+) ---$");
const std::regex C_REGEX_BEACON_POSITION("^(-?\\d+),(-?\\d+),(-?\\d+)$");
const size_t C_ROTATIONS_COUNT = 24;

#define DEBUG_PRINT 0

typedef std::vector<std::vector<coord_3d_str>> distances_t;
typedef std::vector<distances_t> distances_rotated_t;
typedef std::map<int32_t, distances_t> distances_all_t;
typedef std::map<int32_t, distances_rotated_t> distances_rotated_all_t;
typedef std::vector<coord_3d_str> beacons_t;

#pragma region test scanners data
const std::vector<std::string> C_TEST_SCANNERS_DATA = {"--- scanner 0 ---",
													   "404,-588,-901",
													   "528,-643,409",
													   "-838,591,734",
													   "390,-675,-793",
													   "-537,-823,-458",
													   "-485,-357,347",
													   "-345,-311,381",
													   "-661,-816,-575",
													   "-876,649,763",
													   "-618,-824,-621",
													   "553,345,-567",
													   "474,580,667",
													   "-447,-329,318",
													   "-584,868,-557",
													   "544,-627,-890",
													   "564,392,-477",
													   "455,729,728",
													   "-892,524,684",
													   "-689,845,-530",
													   "423,-701,434",
													   "7,-33,-71",
													   "630,319,-379",
													   "443,580,662",
													   "-789,900,-551",
													   "459,-707,401",
													   "",
													   "--- scanner 1 ---",
													   "686,422,578",
													   "605,423,415",
													   "515,917,-361",
													   "-336,658,858",
													   "95,138,22",
													   "-476,619,847",
													   "-340,-569,-846",
													   "567,-361,727",
													   "-460,603,-452",
													   "669,-402,600",
													   "729,430,532",
													   "-500,-761,534",
													   "-322,571,750",
													   "-466,-666,-811",
													   "-429,-592,574",
													   "-355,545,-477",
													   "703,-491,-529",
													   "-328,-685,520",
													   "413,935,-424",
													   "-391,539,-444",
													   "586,-435,557",
													   "-364,-763,-893",
													   "807,-499,-711",
													   "755,-354,-619",
													   "553,889,-390",
													   "",
													   "--- scanner 2 ---",
													   "649,640,665",
													   "682,-795,504",
													   "-784,533,-524",
													   "-644,584,-595",
													   "-588,-843,648",
													   "-30,6,44",
													   "-674,560,763",
													   "500,723,-460",
													   "609,671,-379",
													   "-555,-800,653",
													   "-675,-892,-343",
													   "697,-426,-610",
													   "578,704,681",
													   "493,664,-388",
													   "-671,-858,530",
													   "-667,343,800",
													   "571,-461,-707",
													   "-138,-166,112",
													   "-889,563,-600",
													   "646,-828,498",
													   "640,759,510",
													   "-630,509,768",
													   "-681,-892,-333",
													   "673,-379,-804",
													   "-742,-814,-386",
													   "577,-820,562",
													   "",
													   "--- scanner 3 ---",
													   "-589,542,597",
													   "605,-692,669",
													   "-500,565,-823",
													   "-660,373,557",
													   "-458,-679,-417",
													   "-488,449,543",
													   "-626,468,-788",
													   "338,-750,-386",
													   "528,-832,-391",
													   "562,-778,733",
													   "-938,-730,414",
													   "543,643,-506",
													   "-524,371,-870",
													   "407,773,750",
													   "-104,29,83",
													   "378,-903,-323",
													   "-778,-728,485",
													   "426,699,580",
													   "-438,-605,-362",
													   "-469,-447,-387",
													   "509,732,623",
													   "647,635,-688",
													   "-868,-804,481",
													   "614,-800,639",
													   "595,780,-596",
													   "",
													   "--- scanner 4 ---",
													   "727,592,562",
													   "-293,-554,779",
													   "441,611,-461",
													   "-714,465,-776",
													   "-743,427,-804",
													   "-660,-479,-426",
													   "832,-632,460",
													   "927,-485,-438",
													   "408,393,-506",
													   "466,436,-512",
													   "110,16,151",
													   "-258,-428,682",
													   "-393,719,612",
													   "-211,-452,876",
													   "808,-476,-593",
													   "-575,615,604",
													   "-485,667,467",
													   "-680,325,-822",
													   "-627,-443,-432",
													   "872,-547,-609",
													   "833,512,582",
													   "807,604,487",
													   "839,-516,451",
													   "891,-625,532",
													   "-652,-548,-490",
													   "30,-46,-14"

};
#pragma endregion

class AoC2021_day19 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<int32_t, std::vector<coord_3d_str>> data_;
	distances_rotated_all_t prepare_pt2pt_distances_rotated() const;
	bool are_neighbors(const distances_t& distances, const int32_t scanner1, const beacons_t& beacons, const distances_rotated_t& distances_rotated,
					   const int32_t scanner2, coord_3d_str& relation, int32_t& rotation_id) const;

	int32_t scanner_min_, scanner_max_;
	int32_t get_beacons_count() const;
};

bool AoC2021_day19::init(const std::vector<std::string> lines) {
	int32_t scanner = -1;
	std::smatch sm;

	scanner_min_ = INT32_MAX;
	scanner_max_ = INT32_MIN;

	data_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].empty()) {
			continue;
		}

		if (std::regex_match(lines[i], sm, C_REGEX_SCANNER_ID)) {
			scanner = std::stoi(sm.str(1));
			if (scanner > scanner_max_) {
				scanner_max_ = scanner;
			}
			if (scanner < scanner_min_) {
				scanner_min_ = scanner;
			}
		} else if (std::regex_match(lines[i], sm, C_REGEX_BEACON_POSITION)) {
			data_[scanner].push_back({std::stoi(sm.str(1)), std::stoi(sm.str(2)), std::stoi(sm.str(3))});
		} else {
			std::cout << "Invlid data at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

distances_rotated_all_t AoC2021_day19::prepare_pt2pt_distances_rotated() const {
	distances_rotated_all_t result = {};
	distances_t dists, tmp;
	coord_3d_str coord;
	std::vector<coord_3d_str> vect_dists;
	distances_rotated_t rot_dists;

	for (auto scanner1 = data_.begin(); scanner1 != data_.end(); scanner1++) {
		dists.clear();
		rot_dists.clear();

		for (size_t i = 0; i < scanner1->second.size(); i++) {
			vect_dists.clear();

			for (size_t j = 0; j < scanner1->second.size(); j++) {
				coord = (scanner1->second[j] - scanner1->second[i]);
				vect_dists.push_back(coord);
			}

			dists.push_back(vect_dists);
		}

		for (size_t k = 0; k < C_ROTATIONS_COUNT; k++) {
			tmp = dists;

			for (size_t i = 0; i < tmp.size(); i++) {
				for (size_t j = 0; j < tmp.size(); j++) {
					tmp[i][j] = dists[i][j].rotate(k);
				}
			}

			rot_dists.push_back(tmp);
		}

		result[scanner1->first] = rot_dists;
	}

	return result;
}

bool AoC2021_day19::are_neighbors(const distances_t& distances, const int32_t scanner1, const beacons_t& beacons, const distances_rotated_t& distances_rotated,
								  const int32_t scanner2, coord_3d_str& relation, int32_t& rotation_id) const {
	size_t r2;
	coord_3d_str v1l, v1r, v2l, v2r, v2lr, v2rr, v1, v2, rel, d1, d2;
	std::map<coord_3d_str, coord_3d_str> found;
	relation = {};
	rotation_id = 0;

	for (size_t l1 = 0; l1 < distances.size(); l1++) {
		for (size_t r = 0; r < distances_rotated.size(); r++) {
			found.clear();
			rotation_id = 0;
			for (size_t r1 = 0; r1 < distances[l1].size(); r1++) {
				if (l1 == r1) {
					continue;
				}

				for (size_t l2 = 0; l2 < distances_rotated[r].size(); l2++) {
					const auto& it = std::find(distances_rotated[r][l2].begin(), distances_rotated[r][l2].end(), distances[l1][r1]);
					if (it != distances_rotated[r][l2].end()) {
						r2 = it - distances_rotated[r][l2].begin();

						v1l = beacons[l1];
						v1r = beacons[r1];
						v2l = data_.at(scanner2)[l2];
						v2r = data_.at(scanner2)[r2];

						v2lr = v2l.rotate(r);
						v2rr = v2r.rotate(r);

						v1 = v2lr - v1l;
						v2 = v2rr - v1r;

						if (v1 == v2) {
							if (found.size() == 0) {
								relation = v2;
							}

							if (v1 == relation) {
								rotation_id = r;
								found[v1l] = v2l;
								found[v1r] = v2r;
							}
						}
					}
				}
			}
			if (found.size() >= 12) {
#if DEBUG_PRINT
				for (auto it = found.begin(); it != found.end(); it++) {
					coord_3d_str c1, c2;

					c1 = it->first;
					c2 = it->second;
					std::cout << c1.to_string() << " -> " << c2.to_string() << std::endl;
				}
#endif

				return true;
			}
		}
	}

	return false;
}

int32_t AoC2021_day19::get_beacons_count() const {
	std::map<int32_t, coord_3d_str> scanner_positions = {};
	std::map<int32_t, beacons_t> beacons_by_scanner = {};
	std::vector<int32_t> scanners = {};
	coord_3d_str shift_vector, tmp_coord, beacon_derotated, beacon_shifted;
	std::set<coord_3d_str> beacons;
	std::set<coord_str> history = {};
	coord_str history_item;

	int32_t rotation_id, to;
	bool relation_found = false;
	beacons.clear();

	// get scanners IDs list
	for (auto const& scanner : data_) {
		scanners.push_back(scanner.first);
	}
	// use 1st scanner data as basement

	const int32_t base = scanners.front();
	scanners.erase(scanners.begin());

	scanner_positions[base] = {};

	beacons_by_scanner[base] = data_.at(base);

	distances_rotated_all_t distances_rotated = prepare_pt2pt_distances_rotated();
	distances_all_t distances = {};

	distances[base] = distances_rotated[base][0];
	beacons.insert(beacons_by_scanner[base].begin(), beacons_by_scanner[base].end());

	relation_found = true;

	while (scanners.size() > 0) {
		if (!relation_found) {
			break;
		}

		for (auto it = distances.begin(); it != distances.end(); it++) {
			for (size_t i = 0; i < scanners.size(); i++) {

				history_item = {it->first, scanners[i]};

				if (history.count(history_item) > 0) {
					relation_found = false;
					continue;
				} else {
					history.emplace(history_item);
				}

				relation_found =
					are_neighbors(it->second, it->first, beacons_by_scanner[it->first], distances_rotated[scanners[i]], scanners[i], shift_vector, rotation_id);

				if (relation_found) {
					to = scanners[i];
#if DEBUG_PRINT
					int32_t from = it->first;
					std::cout << "[" << from << ", " << to << "] " << shift_vector.to_string() << " (" << rotation_id << ")" << std::endl;
#endif
					break;
				}
			}
			if (relation_found) {
				break;
			}
		}
		if (relation_found) {
			scanner_positions[to] = shift_vector;
			beacons_by_scanner[to] = data_.at(to);

			for (size_t i = 0; i < data_.at(to).size(); i++) {
				tmp_coord = data_.at(to)[i];
				beacon_derotated = tmp_coord.rotate(rotation_id);
				beacon_shifted = beacon_derotated - shift_vector;
				beacons_by_scanner[to][i] = beacon_shifted;
				beacons.emplace(beacon_shifted);
			}

			distances[to] = distances_rotated[to][rotation_id];

			auto itx = std::find(scanners.begin(), scanners.end(), to);
			if (itx != scanners.end()) {
				scanners.erase(itx);
			}
		}
	}

	return beacons.size();
}

int32_t AoC2021_day19::get_aoc_day() {
	return 19;
}

int32_t AoC2021_day19::get_aoc_year() {
	return 2021;
}

void AoC2021_day19::tests() {
	int64_t result;

	if (init(C_TEST_SCANNERS_DATA)) {
		result = get_beacons_count(); // 79
	}
}

bool AoC2021_day19::part1() {
	int64_t result = 0;

	result = get_beacons_count();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day19::part2() {
	int64_t result = 0;

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day19 day19;

	return day19.main_execution();
}

#include "./../common/aoc.hpp"
#include "./../common/string-utils.hpp"
#include <map>
#include <queue>
#include <set>

const std::string C_START = "start";
const std::string C_END = "end";

struct q_state {
	std::string current;
	std::string path;
	std::string twice;
};

class AoC2021_day12 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<std::string, std::vector<std::string>> edges_;
	std::set<std::string> lower_;
	bool is_lower(const std::string cave) const;
	int64_t find_all_paths(const bool part2) const;
};

bool AoC2021_day12::init(const std::vector<std::string> lines) {
	std::vector<std::string> points;

	edges_.clear();
	lower_.clear();

	for (size_t i = 0; i < lines.size(); i++) {
		points = split(lines[i], "-");
		if (points.size() != 2) {
			std::cout << "Invalid input at line " << i + 1 << std::endl;
			return false;
		}

		edges_[points[0]].push_back(points[1]);
		edges_[points[1]].push_back(points[0]);

		if (is_lower(points[0])) {
			lower_.emplace(points[0]);
		}

		if (is_lower(points[1])) {
			lower_.emplace(points[1]);
		}
	}

	if (!edges_.count(C_START) && edges_.count(C_END)) {
		std::cout << "Start not found" << std::endl;
		return false;
	} else if (!edges_.count(C_END)) {
		std::cout << "End not found" << std::endl;
		return false;
	} else {
		return true;
	}
}

bool AoC2021_day12::is_lower(const std::string cave) const {
	for (size_t i = 0; i < cave.size(); i++) {
		if (!islower(cave[i])) {
			return false;
		}
	}

	return true;
}

int64_t AoC2021_day12::find_all_paths(const bool part2) const {
	std::queue<q_state> q;
	std::vector<std::string> results;
	q_state qs, qn;

	q = {};

	qs.current = C_START;
	qs.path = "," + C_START + ",";

	q.emplace(qs);

	while (q.size()) {
		qs = q.front();
		q.pop();

		for (size_t i = 0; i < edges_.at(qs.current).size(); i++) {
			qn = qs;
			qn.current = edges_.at(qs.current)[i];
			qn.path += qn.current + ",";

			if (lower_.count(qn.current)) {
				if (qn.current == C_START) {
					continue;
				}

				if (qs.path.find("," + qn.current + ",") == std::string::npos) {
					if (qn.current == C_END) {
						results.push_back(qn.path);
					} else {
						q.emplace(qn);
					}
				} else if (part2) {
					if (qn.twice.empty()) {
						qn.twice = qn.current;
						q.emplace(qn);
					}
				}
			} else {
				q.emplace(qn);
			}
		}
	}

	return results.size();
}

int32_t AoC2021_day12::get_aoc_day() {
	return 12;
}

int32_t AoC2021_day12::get_aoc_year() {
	return 2021;
}

void AoC2021_day12::tests() {
	int64_t result;

	if (init({"start-A", "start-b", "A-c", "A-b", "b-d", "A-end", "b-end"})) {
		result = find_all_paths(false); // 10
		/*
			start,A,b,A,c,A,end
			start,A,b,A,end
			start,A,b,end
			start,A,c,A,b,A,end
			start,A,c,A,b,end
			start,A,c,A,end
			start,A,end
			start,b,A,c,A,end
			start,b,A,end
			start,b,end
		*/
		result = find_all_paths(true); // 36
	}

	if (init({"dc-end", "HN-start", "start-kj", "dc-start", "dc-HN", "LN-dc", "HN-end", "kj-sa", "kj-HN", "kj-dc"})) {
		result = find_all_paths(false); // 19
		/*
			start,HN,dc,HN,end
			start,HN,dc,HN,kj,HN,end
			start,HN,dc,end
			start,HN,dc,kj,HN,end
			start,HN,end
			start,HN,kj,HN,dc,HN,end
			start,HN,kj,HN,dc,end
			start,HN,kj,HN,end
			start,HN,kj,dc,HN,end
			start,HN,kj,dc,end
			start,dc,HN,end
			start,dc,HN,kj,HN,end
			start,dc,end
			start,dc,kj,HN,end
			start,kj,HN,dc,HN,end
			start,kj,HN,dc,end
			start,kj,HN,end
			start,kj,dc,HN,end
			start,kj,dc,end
		*/
		result = find_all_paths(true); // 103
	}

	if (init({"fs-end", "he-DX", "fs-he", "start-DX", "pj-DX", "end-zg", "zg-sl", "zg-pj", "pj-he", "RW-he", "fs-DX", "pj-RW", "zg-RW", "start-pj", "he-WI",
			  "zg-he", "pj-fs", "start-RW"})) {
		result = find_all_paths(false); // 226
		result = find_all_paths(true);	// 3509
	}
}

bool AoC2021_day12::part1() {
	int64_t result = 0;

	result = find_all_paths(false);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day12::part2() {
	int64_t result = 0;

	result = find_all_paths(true);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day12 day12;

	return day12.main_execution();
}

#include "./../common/aoc.hpp"
#include <map>

#define DEBUG_PRINT 0

struct node_str {
	int32_t parent;
	int32_t left;
	int32_t right;
};

class AoC2021_day18 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::map<int32_t, node_str> nodes_;
	std::vector<int32_t> roots_;
	int32_t idx_;
	void reduce_pair(const int32_t node_idx);
	void explode_node(const int32_t node_idx);
	void split_number(const int32_t node_idx, const bool left);
	void add_pairs();
	int32_t get_pair_magnitude(const int32_t idx) const;
	void print() const;
	void print_pair(const int32_t idx) const;
	void print_node(const int32_t idx) const;
	void print_single_pair(const std::string prefix, const int32_t idx) const;
	int32_t find_pair_to_explode_or_split(const int32_t node_idx, const bool explode) const;
};

bool AoC2021_day18::init(const std::vector<std::string> lines) {
	int32_t parent = 0, current, value;
	std::map<int32_t, bool> pair_part = {};

	nodes_.clear();
	roots_.clear();

	idx_ = -1;

	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].find_first_not_of("0123456789[],") != std::string::npos) {
			std::cout << "Invalid pair at line " << i + 1 << std::endl;
			return false;
		}

		roots_.push_back(idx_);
		current = 0;

		for (size_t j = 0; j < lines[i].size(); j++) {
			switch (lines[i][j]) {
				case '[':
					if (current != 0) {
						if (pair_part[current]) {
							nodes_[current].right = idx_;
						} else {
							nodes_[current].left = idx_;
						}
					}

					parent = current;

					current = idx_;
					nodes_[current].parent = parent;
					pair_part[current] = false;
					parent = current;
					idx_--;
					break;

				case ',':
					if (pair_part[current]) {
						std::cout << "Third part of pair at line " << i + 1 << std::endl;
						return false;
					}

					pair_part[current] = true;
					break;

				case ']':
					if (!pair_part[current]) {
						std::cout << "Missing second part of pair at line " << i + 1 << std::endl;
						return false;
					}

					current = nodes_[current].parent;
					break;

				default:
					value = lines[i][j] - '0';

					if (pair_part[current]) {
						nodes_[current].right = value;
					} else {
						nodes_[current].left = value;
					}
					break;
			}
		}
		if (current != 0) {
			std::cout << "Incomplete pair at line " << i + 1 << std::endl;
			return false;
		}
	}

	return true;
}

void AoC2021_day18::add_pairs() {
	int32_t new_root, part1;

	if (roots_.size()) {
		part1 = roots_[0];

		for (size_t i = 1; i < roots_.size(); i++) {
			new_root = idx_;
			idx_--;

			nodes_[new_root].left = part1;
			nodes_[new_root].right = roots_[i];
			nodes_[new_root].parent = 0;

			nodes_[part1].parent = new_root;
			nodes_[roots_[i]].parent = new_root;

			reduce_pair(new_root);

			part1 = new_root;
		}

		roots_.clear();
		roots_.push_back(part1);
	}
}

void AoC2021_day18::reduce_pair(const int32_t node_idx) {
	int32_t explode, split;

#if DEBUG_PRINT
	std::cout << std::endl << "==========";
#endif

	while (true) {
		print_pair(node_idx);

		explode = find_pair_to_explode_or_split(node_idx, true);

		if (explode < 0) {
			print_single_pair("E: ", explode);
			explode_node(explode);
			continue;
		}

		split = find_pair_to_explode_or_split(node_idx, false);

		if (split < 0) {
			print_single_pair("S: ", split);
			const bool left_side = (nodes_[split].left >= 10);
			split_number(split, left_side);
			continue;
		} else {
			break;
		}
	}
}

int32_t AoC2021_day18::find_pair_to_explode_or_split(const int32_t node_idx, const bool explode) const {
	int32_t nested, tmp, next;
	std::map<int32_t, uint8_t> pair_states = {};

	nested = 0;
	tmp = node_idx;

	while (tmp < 0) {
		if (explode) {
			if ((nested >= 4) && (nodes_.at(tmp).left >= 0) && (nodes_.at(tmp).right >= 0)) {
				return tmp;
			}
		}

		next = tmp;

		switch (pair_states[tmp]) {
			case 0:
				if (nodes_.at(tmp).left < 0) {
					next = nodes_.at(tmp).left;
					nested++;
				} else if ((!explode) && (nodes_.at(tmp).left >= 10)) {
					return tmp;
				}
				pair_states[tmp] = 2;
				break;
			case 2:
				if (nodes_.at(tmp).right < 0) {
					next = nodes_.at(tmp).right;
					nested++;
				} else if ((!explode) && (nodes_.at(tmp).right >= 10)) {
					return tmp;
				}
				pair_states[tmp] = 3;
				break;
			case 3:
				next = nodes_.at(tmp).parent;
				nested--;
				break;
			default:
				int z = 13;
				break;
		}

		tmp = next;
	}

	return 0;
}

void AoC2021_day18::explode_node(const int32_t node_idx) {
	int32_t tmp, parent, lval, rval, prev;

	lval = nodes_[node_idx].left;
	rval = nodes_[node_idx].right;
	parent = nodes_[node_idx].parent;
	tmp = parent;
	prev = node_idx;

	while (tmp < 0) {
		if (prev == nodes_[tmp].parent) {
			if (nodes_[tmp].right >= 0) {
				nodes_[tmp].right += lval;
				break;
			} else {
				prev = tmp;
				tmp = nodes_[tmp].right;
			}
		} else if (prev == nodes_[tmp].right) {
			if (nodes_[tmp].left >= 0) {
				nodes_[tmp].left += lval;
				break;
			} else {
				prev = tmp;
				tmp = nodes_[tmp].left;
			}
		} else {
			prev = tmp;
			tmp = nodes_[tmp].parent;
		}
	}

	tmp = parent;
	prev = node_idx;

	while (tmp < 0) {
		if (prev == nodes_[tmp].parent) {
			if (nodes_[tmp].left >= 0) {
				nodes_[tmp].left += rval;
				break;
			} else {
				prev = tmp;
				tmp = nodes_[tmp].left;
			}
		} else if (prev == nodes_[tmp].left) {
			if (nodes_[tmp].right >= 0) {
				nodes_[tmp].right += rval;
				break;
			} else {
				prev = tmp;
				tmp = nodes_[tmp].right;
			}
		} else {
			prev = tmp;
			tmp = nodes_[tmp].parent;
		}
	}

	if (nodes_[parent].left == node_idx) {
		nodes_[parent].left = 0;
	} else {
		nodes_[parent].right = 0;
	}
}

void AoC2021_day18::split_number(const int32_t node_idx, const bool left) {
	int32_t idx = idx_, value = 0;
	idx_--;

	if (left) {
		value = nodes_[node_idx].left;
		nodes_[node_idx].left = idx;
	} else {
		value = nodes_[node_idx].right;
		nodes_[node_idx].right = idx;
	}

	nodes_[idx].left = value / 2;
	nodes_[idx].right = (value + 1) / 2;
	nodes_[idx].parent = node_idx;
}

int32_t AoC2021_day18::get_pair_magnitude(const int32_t idx) const {
	int32_t result = 0;

	if (nodes_.at(idx).left < 0) {
		result += 3 * get_pair_magnitude(nodes_.at(idx).left);
	} else {
		result += 3 * nodes_.at(idx).left;
	}

	if (nodes_.at(idx).right < 0) {
		result += 2 * get_pair_magnitude(nodes_.at(idx).right);
	} else {
		result += 2 * nodes_.at(idx).right;
	}

	return result;
}

void AoC2021_day18::print() const {
#if DEBUG_PRINT
	std::cout << std::endl;

	for (size_t i = 0; i < roots_.size(); i++) {
		print_pair(roots_[i]);
	}
#endif
}

void AoC2021_day18::print_pair(const int32_t idx) const {
#if DEBUG_PRINT
	std::cout << std::endl;
	print_node(idx);
	std::cout << std::endl;
#endif
}

void AoC2021_day18::print_single_pair(const std::string prefix, const int32_t idx) const {
#if DEBUG_PRINT
	std::cout << prefix << "[";

	if (nodes_.at(idx).left >= 0) {
		std::cout << nodes_.at(idx).left;
	} else {
		std::cout << "?";
	}

	std::cout << ",";

	if (nodes_.at(idx).right >= 0) {
		std::cout << nodes_.at(idx).right;
	} else {
		std::cout << "?";
	}

	std::cout << "]" << std::endl;

#endif
}

void AoC2021_day18::print_node(const int32_t idx) const {
	std::cout << "[";

	if (nodes_.at(idx).left < 0) {
		print_node(nodes_.at(idx).left);
	} else {
		std::cout << nodes_.at(idx).left;
	}

	std::cout << ",";

	if (nodes_.at(idx).right < 0) {
		print_node(nodes_.at(idx).right);
	} else {
		std::cout << nodes_.at(idx).right;
	}

	std::cout << "]";
}

int32_t AoC2021_day18::get_aoc_day() {
	return 18;
}

int32_t AoC2021_day18::get_aoc_year() {
	return 2021;
}

void AoC2021_day18::tests() {
	int64_t result;

	if (init({"[1,2]", "[[1,2],3]", "[9,[8,7]]", "[[1,9],[8,5]]", "[[[[1,2],[3,4]],[[5,6],[7,8]]],9]", "[[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]",
			  "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]"})) {

		print();

		result = 0; // 45
	}

	if (init({"[[[[[9,8],1],2],3],4]"})) {
		reduce_pair(roots_[0]); // "[[[[0,9],2],3],4]"
	}

	if (init({"[7,[6,[5,[4,[3,2]]]]]"})) {
		reduce_pair(roots_[0]); // "[7,[6,[5,[7,0]]]]"
	}

	if (init({"[[6,[5,[4,[3,2]]]],1]"})) {
		reduce_pair(roots_[0]); // "[[6,[5,[7,0]]],3]"
	}

	if (init({"[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]"})) {
		reduce_pair(roots_[0]); // "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"
								// "[[3,[2,[8,0]]],[9,[5,[7,0]]]]"
	}

	if (init({"[[[[4,3],4],4],[7,[[8,4],9]]]", "[1,1]"})) {
		add_pairs();
		// "[[[[0,7],4],[7,[[8,4],9]]],[1,1]]"
		// "[[[[0,7],4],[15,[0,13]]],[1,1]]"
		// "[[[[0,7],4],[[7,8],[0,13]]],[1,1]]"
		// "[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]"
		// "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]"
	}

	if (init({"[1,1]", "[2,2]", "[3,3]", "[4,4]"})) {
		add_pairs();
		// "[[[[1,1],[2,2]],[3,3]],[4,4]]"
	}

	if (init({"[1,1]", "[2,2]", "[3,3]", "[4,4]", "[5,5]"})) {
		add_pairs();
		// "[[[[3,0],[5,3]],[4,4]],[5,5]]"
	}

	if (init({"[1,1]", "[2,2]", "[3,3]", "[4,4]", "[5,5]", "[6,6]"})) {
		add_pairs();
		// "[[[[5,0],[7,4]],[5,5]],[6,6]]"
	}

	if (init({"[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]", "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]", "[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]",
			  "[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]", "[7,[5,[[3,8],[1,4]]]]", "[[2,[2,2]],[8,[8,1]]]", "[2,9]",
			  "[1,[[[9,3],9],[[9,0],[0,7]]]]", "[[[5,[7,4]],7],1]", "[[[[4,2],2],6],[8,7]]"})) {
		add_pairs();
		// "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]"
	}

	if (init({"[[9,1],[1,9]]"})) {
		result = get_pair_magnitude(roots_[0]); // 129
	}

	if (init({"[[1,2],[[3,4],5]]"})) {
		result = get_pair_magnitude(roots_[0]); // 143.
	}

	if (init({"[[[[0,7],4],[[7,8],[6,0]]],[8,1]]"})) {
		result = get_pair_magnitude(roots_[0]); // 1384.
	}

	if (init({"[[[[1,1],[2,2]],[3,3]],[4,4]]"})) {
		result = get_pair_magnitude(roots_[0]); // 445.
	}

	if (init({"[[[[3,0],[5,3]],[4,4]],[5,5]]"})) {
		result = get_pair_magnitude(roots_[0]); // 791.
	}

	if (init({"[[[[5,0],[7,4]],[5,5]],[6,6]]"})) {
		result = get_pair_magnitude(roots_[0]); // 1137.
	}

	if (init({"[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]"})) {
		result = get_pair_magnitude(roots_[0]); // 3488.
	}

	if (init({"[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]", "[[[5,[2,8]],4],[5,[[9,9],0]]]", "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]",
			  "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]", "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]", "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]",
			  "[[[[5,4],[7,7]],8],[[8,3],8]]", "[[9,3],[[9,9],[6,[4,9]]]]", "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]",
			  "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]"})) {
		add_pairs(); // "[[[[6,6],[7,6]],[[7,7],[7,0]]],[[[7,7],[7,7]],[[7,8],[9,9]]]]""
		result = get_pair_magnitude(roots_[0]); // 4140.
	}
}

bool AoC2021_day18::part1() {
	int64_t result = 0;

	add_pairs();
	result = get_pair_magnitude(roots_[0]);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day18::part2() {
	int64_t result = 0;

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day18 day18;

	return day18.main_execution();
}

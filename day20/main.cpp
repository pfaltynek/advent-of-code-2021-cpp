#include "./../common/aoc.hpp"
#include "./../common/coord.hpp"
#include <set>

#define DEBUG_PRINT 0

class AoC2021_day20 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::set<uint16_t> algorithm_;
	std::set<coord_str> image_;
	int32_t left_, right_, top_, bottom_;
	bool get_output_pixel(const coord_str pixel) const;
	size_t enhance_image(const size_t rounds);
	void print() const;
};

bool AoC2021_day20::init(const std::vector<std::string> lines) {
	if (lines.size() < 3) {
		std::cout << "Input data incomplete" << std::endl;
		return false;
	}

	if (lines[0].size() != 512) {
		std::cout << "Invalid image enhancement algorithm size" << std::endl;
		return false;
	}

	algorithm_.clear();

	for (uint16_t i = 0; i < lines[0].size(); i++) {
		if (lines[0][i] == '#') {
			algorithm_.emplace(i);
		}
	}

	if (lines[1].size() > 0) {
		std::cout << "Invalid input data format" << std::endl;
		return false;
	}

	image_.clear();

	for (uint32_t y = 2; y < lines.size(); y++) {
		if (y == 2) {
			right_ = lines[y].size() - 1;
		} else {
			if (lines[y].size() != static_cast<size_t>(right_ + 1)) {
				std::cout << "Invalid image width at line " << y + 1 << std::endl;
				return false;
			}
		}

		for (uint32_t x = 0; x < lines[y].size(); x++) {
			if (lines[y][x] == '#') {
				image_.emplace(coord_str(x, y - 2));
			}
		}
	}

	left_ = 0;
	top_ = 0;
	bottom_ = lines.size() - 3;

	return true;
}

bool AoC2021_day20::get_output_pixel(const coord_str pixel) const {
	uint16_t idx = 0;

	for (int32_t y = pixel.y - 1; y <= pixel.y + 1; y++) {
		for (int32_t x = pixel.x - 1; x <= pixel.x + 1; x++) {
			idx = idx << 1;
			if (image_.count({x, y}) > 0) {
				idx |= 1;
			}
		}
	}

	idx &= (512 - 1);

	return (algorithm_.count(idx) > 0);
}

size_t AoC2021_day20::enhance_image(const size_t rounds) {
	std::set<coord_str> img;
	coord_str pos;

	left_ -= 55;
	bottom_ += 55;
	top_ -= 55;
	right_ += 55;

	print();

	for (size_t i = 0; i < rounds; i++) {

		img.clear();

		for (int32_t x = left_; x <= right_; x++) {
			for (int32_t y = top_; y <= bottom_; y++) {
				pos = {x, y};

				if (get_output_pixel(pos)) {
					img.emplace(pos);
				}
			}
		}

		image_.swap(img);

		print();

		left_++;
		bottom_--;
		top_++;
		right_--;
	}

	return image_.size();
}

void AoC2021_day20::print() const {
#if DEBUG_PRINT
	for (int32_t y = top_; y <= bottom_; y++) {
		for (int32_t x = left_; x <= right_; x++) {
			if (image_.count({x, y}) > 0) {
				std::cout << '#';
			} else {
				std::cout << '.';
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

#endif
}

int32_t AoC2021_day20::get_aoc_day() {
	return 20;
}

int32_t AoC2021_day20::get_aoc_year() {
	return 2021;
}

void AoC2021_day20::tests() {
	int64_t result;

	if (init({"..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#"
			  ".###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.#"
			  "#.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#."
			  "......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#",
			  "", "#..#.", "#....", "##..#", "..#..", "..###"})) {
		result = enhance_image(2); // 35
		result = enhance_image(48); // 3351

	}
}

bool AoC2021_day20::part1() {
	int64_t result = 0;

	result = enhance_image(2);

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day20::part2() {
	int64_t result = 0;

	result = enhance_image(48);

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day20 day20;

	return day20.main_execution();
}

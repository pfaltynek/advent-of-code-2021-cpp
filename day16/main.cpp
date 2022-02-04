#include "./../common/aoc.hpp"
#include <algorithm>
#include <numeric>
class bits_stream {
  public:
	bits_stream(const std::vector<uint8_t> message) {
		stream_ = message;
		remaining_bits_ = 8;
		index_ = 0;
		ver_sum_ = 0;
		bits_read_ = 0;
	}

	uint32_t get_version_sum() const {
		return ver_sum_;
	}

	uint64_t decode_stream() {
		return decode_packet();
	}

  private:
	uint64_t decode_packet() {
		uint32_t version, type_id, length_type_id, count, bits_count;
		uint64_t packet_result, literal_value;
		std::vector<uint64_t> params = {};

		version = get_bits(3);
		ver_sum_ += version;
		type_id = get_bits(3);

		switch (type_id) {
			case 4: // literal value
				literal_value = decode_literal();
				packet_result = literal_value;
				break;
			default:
				length_type_id = get_bits(1);

				if (length_type_id == 0) {
					count = get_bits(15);
					bits_count = bits_read_;

					while (bits_count + count > bits_read_) {
						params.push_back(decode_packet());
					}
				} else {
					count = get_bits(11);

					for (size_t i = 0; i < count; i++) {
						params.push_back(decode_packet());
					}
				}

				switch (type_id) {
					case 0: // sum
						packet_result = std::accumulate(params.begin(), params.end(), 0llu);
						break;
					case 1: // product
						packet_result = 1;
						for (size_t i = 0; i < params.size(); i++) {
							packet_result *= params[i];
						}
						break;
					case 2: // minimum
						packet_result = *std::min_element(params.begin(), params.end());
						break;
					case 3: // maximum
						packet_result = *std::max_element(params.begin(), params.end());
						break;
					case 5: // greater than
						packet_result = (params[0] > params[1]) ? 1 : 0;
						break;
					case 6: // less than
						packet_result = (params[0] < params[1]) ? 1 : 0;
						break;
					case 7: // equal to
						packet_result = (params[0] == params[1]) ? 1 : 0;
						break;
				}
				break;
		}

		return packet_result;
	}

	uint64_t decode_literal() {
		uint32_t start_bit = 0, value;
		uint64_t result = 0;

		do {
			start_bit = get_bits(1);
			value = get_bits(4);
			result = result << 4;
			result |= static_cast<uint64_t>(value);
		} while (start_bit != 0);

		return result;
	}

	bits_stream() {
	}

	static void mask_bits(const uint8_t bits_count, uint32_t& data) {
		uint32_t mask = (1 << bits_count) - 1;
		data &= mask;
	}

	uint32_t get_bits(const uint8_t bits_count) {
		uint8_t cnt = bits_count % 32;
		uint32_t result = 0, tmp, bits_read = 0;

		while ((cnt > 0) && (index_ < stream_.size())) {
			if (cnt < remaining_bits_) {
				result = result << cnt;
				tmp = stream_[index_];
				tmp = tmp >> (remaining_bits_ - cnt);
				mask_bits(cnt, tmp);
				result |= tmp;

				remaining_bits_ -= cnt;
				bits_read += cnt;
				cnt = 0;
			} else {
				result = result << remaining_bits_;
				tmp = stream_[index_];
				mask_bits(remaining_bits_, tmp);
				result |= tmp;

				cnt -= remaining_bits_;
				bits_read += remaining_bits_;
				remaining_bits_ = 8;
				index_++;
			}
		}

		bits_read_ += bits_read;

		return result;
	}

	std::vector<uint8_t> stream_;
	uint8_t remaining_bits_;
	size_t index_, ver_sum_, bits_read_;
};

class AoC2021_day16 : public AoC {
  protected:
	bool init(const std::vector<std::string> lines);
	bool part1();
	bool part2();
	void tests();
	int32_t get_aoc_day();
	int32_t get_aoc_year();

  private:
	std::vector<uint8_t> message_;
};

bool AoC2021_day16::init(const std::vector<std::string> lines) {
	size_t index = 0;
	std::string byte, msg;
	unsigned long hexa;

	message_.clear();
	msg.clear();

	for (uint32_t i = 0; i < lines.size(); i++) {
		msg.append(lines[i]);
	}

	while (index < msg.size()) {
		byte = msg.substr(index, 2);
		index += 2;

		if (byte.size() < 2) {
			byte.append("0");
		}

		hexa = std::stoul(byte, nullptr, 16);
		message_.push_back(static_cast<uint8_t>(hexa));
	}

	return true;
}

int32_t AoC2021_day16::get_aoc_day() {
	return 16;
}

int32_t AoC2021_day16::get_aoc_year() {
	return 2021;
}

void AoC2021_day16::tests() {
	int64_t result;

	if (init({"D2FE28"})) {
		bits_stream bs(message_);
		bs.decode_stream();
		result = bs.get_version_sum(); // ?
	}

	if (init({"38006F45291200"})) {
		bits_stream bs(message_);
		bs.decode_stream();
		result = bs.get_version_sum(); // ?
	}

	if (init({"EE00D40C823060"})) {
		bits_stream bs(message_);
		bs.decode_stream();
		result = bs.get_version_sum(); // ?
	}

	if (init({"8A004A801A8002F478"})) {
		bits_stream bs(message_);
		bs.decode_stream();
		result = bs.get_version_sum(); // 16
	}

	if (init({"620080001611562C8802118E34"})) {
		bits_stream bs(message_);
		bs.decode_stream();
		result = bs.get_version_sum(); // 12
	}

	if (init({"C0015000016115A2E0802F182340"})) {
		bits_stream bs(message_);
		bs.decode_stream();
		result = bs.get_version_sum(); // 23
	}

	if (init({"A0016C880162017C3686B18A3D4780"})) {
		bits_stream bs(message_);
		bs.decode_stream();
		result = bs.get_version_sum(); // 31
	}

	if (init({"C200B40A82"})) {
		bits_stream bs(message_);
		result = bs.decode_stream(); // finds the sum of 1 and 2, resulting in the value 3.
	}

	if (init({"04005AC33890"})) {
		bits_stream bs(message_);
		result = bs.decode_stream(); // finds the product of 6 and 9, resulting in the value 54.
	}

	if (init({"880086C3E88112"})) {
		bits_stream bs(message_);
		result = bs.decode_stream(); // finds the minimum of 7, 8, and 9, resulting in the value 7.
	}

	if (init({"CE00C43D881120"})) {
		bits_stream bs(message_);
		result = bs.decode_stream(); // finds the maximum of 7, 8, and 9, resulting in the value 9.
	}

	if (init({"D8005AC2A8F0"})) {
		bits_stream bs(message_);
		result = bs.decode_stream(); // produces 1, because 5 is less than 15.
	}

	if (init({"F600BC2D8F"})) {
		bits_stream bs(message_);
		result = bs.decode_stream(); // produces 0, because 5 is not greater than 15.
	}

	if (init({"9C005AC2F8F0"})) {
		bits_stream bs(message_);
		result = bs.decode_stream(); // produces 0, because 5 is not equal to 15.
	}

	if (init({"9C0141080250320F1802104A08"})) {
		bits_stream bs(message_);
		result = bs.decode_stream(); // produces 1, because 1 + 3 = 2 * 2.
	}
}

bool AoC2021_day16::part1() {
	int64_t result = 0;

	bits_stream bs(message_);
	bs.decode_stream();
	result = bs.get_version_sum();

	result1_ = std::to_string(result);

	return true;
}

bool AoC2021_day16::part2() {
	int64_t result = 0;

	bits_stream bs(message_);
	result = bs.decode_stream();

	result2_ = std::to_string(result);

	return true;
}

int main(void) {
	AoC2021_day16 day16;

	return day16.main_execution();
}

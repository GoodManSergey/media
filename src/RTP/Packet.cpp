#include "Packet.h"

namespace RTP {

	Packet::Packet() :
		m_version(2),
		m_padding_present(false),
		m_extension_present(false),
		m_csrc_count(0),
		m_marker(false),
		m_payload_type(0),
		m_seq(0),
		m_timestamp(0),
		m_ssrc(4),
		m_extension_header(0)
	{}

	std::unique_ptr<Packet> Packet::Parse(const uint8_t* data, size_t size) {
		if (data == nullptr || size < 12) {
			return {};
		}

		std::unique_ptr<Packet> packet = std::make_unique<Packet>();

		packet->m_version = (data[0] & 192u) >> 6u;
		packet->m_padding_present = (data[0] & 32u) >> 5u;
		packet->m_extension_present = (data[0] & 16u) >> 4u;
		packet->m_csrc_count = (data[0] & 15u);
		packet->m_marker = (data[1] & 128u) >> 7u;
		packet->m_payload_type = (data[1] & 127u);
		packet->m_seq = (((uint16_t)data[2]) << 8u) + data[3];
		packet->m_timestamp = (((uint32_t)data[4]) << 24u) + (((uint32_t)data[5]) << 16u) + (((uint32_t)data[6]) << 8u) + data[7];
		packet->m_ssrc = (((uint32_t)data[8]) << 24u) + (((uint32_t)data[9]) << 16u) + (((uint32_t)data[10]) << 8u) + data[11];

		unsigned offset = 12;
		if (packet->m_csrc_count > 0) {
			if (size < (offset + packet->m_csrc_count * 4)) {
				return {};
			}
			for (int i = 0; i < packet->m_csrc_count; i++) {
				uint32_t csrc = (((uint32_t)data[offset++]) << 24u) + (((uint32_t)data[offset++]) << 16u)
						+ (((uint32_t)data[offset++]) << 8u) + data[offset++];
				packet->m_csrc.push_back(csrc);
			}
		}

		if (packet->m_extension_present) {
			if (size < (offset + 4)) {
				return {};
			}
			packet->m_extension_header = (((uint16_t)data[offset++]) << 8u) + data[offset++];
			uint16_t extension_size = ((((uint16_t)data[offset++]) << 8u) + data[offset++]) * 4;

			if (size < (offset + extension_size)) {
				return {};
			}

			packet->m_extension_data = std::vector<uint8_t>(data + offset, data + offset + extension_size);
			offset += extension_size;
		}

		if (size > offset) {
			packet->m_payload = std::vector<uint8_t>(data + offset, data + size);
		}

		return std::move(packet);
	}
}
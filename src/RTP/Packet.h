#ifndef MEDIA_PACKET_H
#define MEDIA_PACKET_H

#include <memory>
#include <vector>

namespace RTP {
	class Packet {
	public:
		static std::unique_ptr<Packet> Parse(const uint8_t* data, size_t size);

		Packet();

		void get_raw(uint8_t* data, size_t& size);

		uint8_t m_version;
		bool m_padding_present;
		bool m_extension_present;
		uint8_t m_csrc_count;
		bool m_marker;
		uint8_t m_payload_type;
		uint16_t m_seq;
		uint32_t m_timestamp;
		uint32_t m_ssrc;
		std::vector<uint32_t> m_csrc;
		uint16_t m_extension_header;
		std::vector<uint8_t> m_extension_data;
		std::vector<uint8_t> m_payload;

	};

}


#endif //MEDIA_PACKET_H

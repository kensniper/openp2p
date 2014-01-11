#include <stdint.h>
#include <string.h>

#include <algorithm>
#include <stdexcept>

#include <OpenP2P/BinaryStream.hpp>
#include <OpenP2P/Stream.hpp>

namespace OpenP2P {

	void BinaryIStream::readAll(uint8_t* data, size_t size) {
		if (size == 0) return;
		
		size_t pos = 0;
		
		// TODO: Replace polling with wait() calls.
		while (stream_.isValid()) {
			pos += stream_.read(data + pos, size - pos);
			if (pos == size) return;
		}
		
		throw std::runtime_error("readAll() failed; stream is invalid.");
	}
	
	size_t BinaryIStream::readSome(uint8_t* data, size_t size) {
		if (size == 0) return 0;
		
		// TODO: Replace polling with wait() calls.
		while (stream_.isValid()) {
			const size_t readSize = stream_.read(data, size);
			if (readSize > 0) return readSize;
		}
		
		throw std::runtime_error("readSome() failed; stream is invalid.");
	}
	
	void BinaryOStream::writeAll(const uint8_t* data, size_t size) {
		if (size == 0) return;
		
		size_t pos = 0;
		
		// TODO: Replace polling with wait() calls.
		while (stream_.isValid()) {
			pos += stream_.write(data + pos, size - pos);
			if (pos == size) return;
		}
		
		throw std::runtime_error("writeAll() failed; stream is invalid.");
	}
	
	size_t BinaryOStream::writeSome(const uint8_t* data, size_t size) {
		if (size == 0) return 0;
		
		// TODO: Replace polling with wait() calls.
		while (stream_.isValid()) {
			const size_t writeSize = stream_.write(data, size);
			if (writeSize > 0) return writeSize;
		}
		
		throw std::runtime_error("writeSome() failed; stream is invalid.");
	}
	
	namespace Binary {
	
		uint8_t ReadUint8(BinaryIStream& stream) {
			uint8_t value = 0;
			stream.readAll(&value, sizeof(value));
			return value;
		}
		
		int8_t ReadInt8(BinaryIStream& stream) {
			return (int8_t) ReadUint8(stream);
		}
		
		uint16_t ReadUint16(BinaryIStream& stream) {
			uint8_t data[2];
			stream.readAll(data, sizeof(data));
			return (uint16_t(data[0]) << 8) |
				uint16_t(data[1]);
		}
		
		int16_t ReadInt16(BinaryIStream& stream) {
			return (int16_t) ReadUint16(stream);
		}
		
		uint32_t ReadUint32(BinaryIStream& stream) {
			uint8_t data[4];
			stream.readAll(data, sizeof(data));
			return (uint32_t(data[0]) << 24) |
				(uint32_t(data[1]) << 16) |
				(uint32_t(data[2]) << 8) |
				uint32_t(data[3]);
		}
		
		int32_t ReadInt32(BinaryIStream& stream) {
			return (int32_t) ReadUint32(stream);
		}
		
		uint64_t ReadUint64(BinaryIStream& stream) {
			uint8_t data[8];
			stream.readAll(data, sizeof(data));
			return (uint64_t(data[0]) << 56) |
				(uint64_t(data[1]) << 48) |
				(uint64_t(data[2]) << 40) |
				(uint64_t(data[3]) << 32) |
				(uint64_t(data[4]) << 24) |
				(uint64_t(data[5]) << 16) |
				(uint64_t(data[6]) << 8) |
				uint64_t(data[7]);
		}
		
		int64_t ReadInt64(BinaryIStream& stream) {
			return (int64_t) ReadUint64(stream);
		}
		
		
		void WriteUint8(BinaryOStream& stream, uint8_t value) {
			stream.writeAll(&value, 1);
		}
		
		void WriteInt8(BinaryOStream& stream, int8_t value) {
			WriteUint8(stream, value);
		}
		
		void WriteUint16(BinaryOStream& stream, uint16_t value) {
			uint8_t s[2];
			s[0] = (value >> 8) & 0xFF;
			s[1] = value & 0xFF;
			stream.writeAll(s, 2);
		}
		
		void WriteInt16(BinaryOStream& stream, int16_t value) {
			WriteUint16(stream, value);
		}
		
		void WriteUint32(BinaryOStream& stream, uint32_t value) {
			uint8_t s[4];
			s[0] = (value >> 24) & 0xFF;
			s[1] = (value >> 16) & 0xFF;
			s[2] = (value >> 8) & 0xFF;
			s[3] = value & 0xFF;
			stream.writeAll(s, 4);
		}
		
		void WriteInt32(BinaryOStream& stream, int32_t value) {
			WriteUint32(stream, value);
		}
		
		void WriteUint64(BinaryOStream& stream, uint64_t value) {
			uint8_t s[8];
			s[0] = (value >> 56) & 0xFF;
			s[1] = (value >> 48) & 0xFF;
			s[2] = (value >> 40) & 0xFF;
			s[3] = (value >> 32) & 0xFF;
			s[4] = (value >> 24) & 0xFF;
			s[5] = (value >> 16) & 0xFF;
			s[6] = (value >> 8) & 0xFF;
			s[7] = value & 0xFF;
			stream.writeAll(s, 8);
		}
		
		void WriteInt64(BinaryOStream& stream, int64_t value) {
			WriteUint64(stream, value);
		}
		
		static const size_t MOVE_BUFFER_SIZE = 256;
		
		void MoveData(IStream& source, OStream& destination) {
			uint8_t buffer[MOVE_BUFFER_SIZE];
			size_t pos = 0, bufferSize = 0;
			
			while (source.isValid() && destination.isValid()) {
				if (pos == bufferSize) {
					pos = 0;
					bufferSize = source.read(&buffer[0], MOVE_BUFFER_SIZE);
				} else {
					pos += destination.write(&buffer[pos], bufferSize - pos);
				}
			}
		}
		
	}
	
}


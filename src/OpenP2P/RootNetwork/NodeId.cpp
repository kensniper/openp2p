#include <array>

#include <boost/functional/hash.hpp>

#include <OpenP2P/Crypt/Hash/SHA256Stream.hpp>

#include <OpenP2P/Stream/BinaryStream.hpp>

#include <OpenP2P/RootNetwork/NodeId.hpp>
#include <OpenP2P/RootNetwork/Parameters.hpp>

namespace OpenP2P {

	namespace RootNetwork {
	
		NodeId::NodeId() {
			data_.fill(0x00);
		}
		
		NodeId NodeId::Zero() {
			return NodeId();
		}
		
		NodeId NodeId::Max() {
			return ~(NodeId::Zero());
		}
		
		NodeId NodeId::FromReader(BlockingReader& reader) {
			NodeId id;
			reader.readAll(id.data_.data(), id.data_.size());
			return id;
		}
		
		NodeId NodeId::Generate(const PublicKey& publicKey) {
			const auto encodedKey = publicKey.toBuffer();
			
			// Perform a hash on the key to
			// generate the node ID.
			Crypt::Hash::SHA256Stream hashStream;
			BinaryOStream blockingStream(hashStream);
			blockingStream.writeAll(encodedKey.data(), encodedKey.size());
			const auto digest = hashStream.calculateDigest();
			assert(digest.size() == NODE_ID_SIZE_BYTES);
			
			NodeId id;
			memcpy(id.data_.data(), digest.data(), digest.size());
			return id;
		}
		
		void NodeId::writeTo(BlockingWriter& writer) const {
			writer.writeAll(data_.data(), data_.size());
		}
		
		bool NodeId::bitAt(size_t index) const {
			constexpr auto bitsInByte = 8;
			assert(index < NodeId::SIZE_IN_BITS);
			const auto byte = data_.at(index / bitsInByte);
			const auto bitIndex = bitsInByte - 1 - (index % bitsInByte);
			return ((byte >> bitIndex) & 1) == 1;
		}
		
		bool NodeId::operator==(const NodeId& other) const {
			return data_ == other.data_;
		}
		
		bool NodeId::operator!=(const NodeId& other) const {
			return !(*this == other);
		}
		
		bool NodeId::operator<(const NodeId& other) const {
			return data_ < other.data_;
		}
		
		NodeId NodeId::operator^(const NodeId& other) const {
			NodeId result;
			
			for (size_t i = 0; i < data_.size(); i++) {
				result.data_.at(i) = data_.at(i) ^ other.data_.at(i);
			}
			
			return result;
		}
		
		NodeId NodeId::operator~() const {
			NodeId reverse;
			
			for (size_t i = 0; i < data_.size(); i++) {
				reverse.data_.at(i) = ~(data_.at(i));
			}
			
			return reverse;
		}
		
		std::size_t NodeId::hash() const {
			return boost::hash_range(data_.begin(), data_.end());
		}
		
		namespace {
			
			char getHexDigit(uint8_t value) {
				assert(value < 16);
				if (value < 10) {
					return '0' + value;
				} else {
					return 'A' + (value - 10);
				}
			}
			
		}
		
		std::string NodeId::hexString() const {
			std::array<char, NODE_ID_SIZE_BYTES*2> hexData;
			for (size_t i = 0; i < NODE_ID_SIZE_BYTES; i++) {
				const uint8_t value = data_.at(i);
				hexData.at(i * 2 + 0) = getHexDigit((value >> 4) & 15);
				hexData.at(i * 2 + 1) = getHexDigit((value >> 0) & 15);
			}
			return std::string(hexData.data(), hexData.size());
		}
		
	}
	
}


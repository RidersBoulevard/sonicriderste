#pragma once

#include <optional>

#include "lib/sound.hpp"
#include "riders/player.hpp"

namespace Sound {
	inline void PlaySound(const Player &player, u8 soundID) {
		if(player.aiControl) { return; }
		if(player.characterVoiceID != 0) [[likely]] {
			PlaySound(player.characterVoiceID, soundID);
			return;
		}
		// Most likely not needed but available as a fallback
		//auto exloadInfo = FetchEnabledEXLoadIDs(player);
		//ID::CharacterSoundIDs charID{};
		//if(isSuperCharacter(player, MetalSonic)){
		//	charID = ID::NeoMetalSonic;
		//}
		//switch(exloadInfo.gearExLoadID) {
		//	case HyperSonic:
		//		charID = ID::HyperSonic;
		//		break;
		//	default: break;
		//}
		//switch(exloadInfo.characterExLoadID) {
		//	case HatsuneMiku:
		//		charID = ID::Miku;
		//		break;
		//	case Reala:
		//		charID = ID::Reala;
		//		break;
		//	case GonGon:
		//		charID = ID::GonGon;
		//		break;
		//	case NeoMetal:
		//		charID = ID::NeoMetalSonic;
		//		break;
		//	default: break;
		//}
		//if(charID != 0){
		//	PlaySound(charID, soundID);
		//} else {
		//	PlaySound(player.character, soundID);
		//}
	}

	inline void PlaySound(const Player &player, std::optional<u8> soundID){
		if(!soundID) { return; }
		PlaySound(player, *soundID);
	}
}// namespace Sound

struct VoiceLineList {
	static constexpr const std::optional<u8> defaultValue = std::nullopt;
};

struct TrickLandVoiceLineList : public VoiceLineList {
	std::optional<u8> cRank = defaultValue;
	std::optional<u8> bRank = defaultValue;
	std::optional<u8> aRank = defaultValue;
	std::optional<u8> aPlusRank = defaultValue;
	std::optional<u8> sRank = defaultValue;
	std::optional<u8> sPlusRank = defaultValue;
	std::optional<u8> xRank = defaultValue;
	std::optional<u8> tenPlus = defaultValue;

	/**
	 * Combine one VoiceLineList with another, while skipping any default values
	 * @param overlay The other VoiceLineList that you want to override with. Most likely an Exload list
	 * @return The combined VoiceLineList
	 */
	[[nodiscard]] constexpr TrickLandVoiceLineList combine(const TrickLandVoiceLineList &overlay) const {
		TrickLandVoiceLineList ret = *this;

		for(u8 i = 0; i <= TenPlus; i++) {
			const auto rank = static_cast<TrickRanks>(i);
			if(!overlay[rank]) { continue; }
			ret[rank] = overlay[rank];
		}
		return ret;
	}

	constexpr std::optional<u8> &operator[](TrickRanks rank) {
		switch(rank) {
			case TenPlus:
				if(tenPlus) {
					return tenPlus;
				}
				[[fallthrough]];
			case XRank:
				if(xRank) {
					return xRank;
				}
				[[fallthrough]];
			case SPlusRank:
				if(sPlusRank) {
					return sPlusRank;
				}
				[[fallthrough]];
			case SRank:
				if(sRank) {
					return sRank;
				}
				[[fallthrough]];
			case APlusRank:
				if(aPlusRank) {
					return aPlusRank;
				}
				[[fallthrough]];
			case ARank:
				if(aRank) {
					return aRank;
				}
				[[fallthrough]];
			case BRank: return bRank;
			case CRank: return cRank;
			default: break;
		}
		std::unreachable();
	}

	constexpr const std::optional<u8> &operator[](TrickRanks rank) const {
		// Cast away the const-ness of `this`, so we can call the non-const version of the overload. Ugly hack but necessary to avoid duplicate code
		TrickLandVoiceLineList &self = *const_cast<TrickLandVoiceLineList *>(this);// NOLINT(cppcoreguidelines-pro-type-const-cast)
		return self[rank];
	}
};

struct AttackingVoiceLinePair {
	static constexpr auto sfxCount = 3;
	std::optional<u8> voice = VoiceLineList::defaultValue;
	std::array<std::optional<u8>, sfxCount> sfx = {};

	constexpr AttackingVoiceLinePair() = default;

	constexpr AttackingVoiceLinePair(const std::optional<u8> &voice = VoiceLineList::defaultValue, const std::initializer_list<u8> sfxInit = {}) : voice(voice) {
		if consteval {
			if(sfxInit.size() > sfxCount) {
				throw std::out_of_range("Number of SFX is too big! "
										"If this many is needed, change \"sfxCount\" to the desired value");
			}
		}
		std::copy(sfxInit.begin(), sfxInit.end(), sfx.begin());
	}

	constexpr AttackingVoiceLinePair(const std::initializer_list<u8> sfxInit = {})
		: AttackingVoiceLinePair(VoiceLineList::defaultValue, sfxInit) {}

	constexpr bool operator==(const AttackingVoiceLinePair &other) const {
		if(voice != other.voice) { return false; }
		for(size_t i = 0; i < sfx.size(); i++) {
			if(sfx[i] != other.sfx[i]) { return false; }
		}
		return true;
	}

	constexpr bool operator!=(const AttackingVoiceLinePair &other) const {
		return !(*this == other);
	}
};

struct AttackingVoiceLineList : public VoiceLineList {
	static constexpr std::optional<AttackingVoiceLinePair> defaultPair = std::nullopt;
	std::optional<AttackingVoiceLinePair> lvl1 = defaultPair;
	std::optional<AttackingVoiceLinePair> lvl2 = defaultPair;
	std::optional<AttackingVoiceLinePair> lvl3 = defaultPair;
	// std::optional<AttackingVoiceLinePair> lvl4 = defaultPair;

	/**
	 * Combine one VoiceLineList with another, while skipping any default values
	 * @param overlay The other VoiceLineList that you want to override with. Most likely an Exload list
	 * @return The combined VoiceLineList
	 */
	[[nodiscard]] constexpr AttackingVoiceLineList combine(const AttackingVoiceLineList &overlay) const {
		AttackingVoiceLineList ret = *this;

		for(u8 i = 0; i < 3; i++) {
			if(!overlay[i]) { continue; }
			auto &currentVal = ret[i];
			const auto &overlayVal = overlay[i];

			for(size_t sfxIndex = 0; sfxIndex < AttackingVoiceLinePair::sfxCount; sfxIndex++) {
				if(!currentVal->sfx[sfxIndex]) {
					currentVal->sfx[sfxIndex] = overlayVal->sfx[sfxIndex];
				}
			}
			if(!currentVal->voice) {
				currentVal->voice = overlayVal->voice;
			}
		}
		return ret;
	}

	constexpr std::optional<AttackingVoiceLinePair> &operator[](size_t index) {
		switch(index) {
			//case 3:
			//	if(lvl4 != defaultValue){
			//		return lvl4;
			//	}
			//	[[fallthrough]];
			case 2:
				if(lvl3) {
					return lvl3;
				}
				[[fallthrough]];
			case 1:
				if(lvl2) {
					return lvl2;
				}
				[[fallthrough]];
			case 0:
				if(lvl1) {
					return lvl1;
				}
				[[fallthrough]];
			default: break;
		}
		if consteval {
			throw std::out_of_range("Level outside valid range");
		}
		// For some reason, having a throw here causes a try/catch to get generated which still breaks
		std::unreachable();
	}

	constexpr const std::optional<AttackingVoiceLinePair> &operator[](size_t index) const {
		// Cast away the const-ness of `this`, so we can call the non-const version of the overload. Ugly hack but necessary to avoid duplicate code
		AttackingVoiceLineList &self = *const_cast<AttackingVoiceLineList *>(this);// NOLINT(cppcoreguidelines-pro-type-const-cast)
		return self[index];
	}
};

ASMUsed void PlayPassingCharacterVoice(const Player &player);
void PlayGettingAttackedVoice(const Player &player);
ASMUsed void PlayTrickJumpVoice(const Player &player);
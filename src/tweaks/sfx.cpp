#include "sfx.hpp"
#include "lib/sound.hpp"

ASMUsed void *SFX_Magnet(MagnetObject1 *object1, void *object) {
	const Player &player = players[object1->playerIndex];
	if (!player.playerType) {
		PlayAudioFromDAT(Sound::SFX::MagnetShield);
	}
	return object;
}

ASMUsed void SFX_Death(Player *player) {
	if (player->playerType) return;
	PlayAudioFromDAT(Sound::SFX::FallingIntoAPit);
}

ASMUsed void SFX_TenOrMoreTricks(Player *player, u32 isTrickNotFailed) {
	if (player->playerType) return;
	if (isTrickNotFailed != 1) return;
	if (player->trickCount < 10) return;

	PlayAudioFromDAT(Sound::SFX::TenOrMoreTricks);
}

ASMUsed void SFX_HitByTornado() {
	PlayAudioFromDAT(Sound::SFX::TornadoHit);
}
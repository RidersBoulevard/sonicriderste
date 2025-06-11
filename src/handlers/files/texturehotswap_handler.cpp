#include "cosmetics/player/exloads.hpp"
#include "lib/files.hpp"
#include "riders/player.hpp"
#include "lib/files.hpp"
#include "cosmetics/player/exloads.hpp"

ASMDefined void* tex_HotswapTextures;

ASMUsed void* TextureHotswapHandler(Player *player, void* gvrTexture, u32 textureID) {
	if (player->character != Character::E10R) { return gvrTexture; }
    void* customTexture = tex_HotswapTextures;
    if (customTexture == nullptr) { return gvrTexture; }

    if (player->characterExload().exLoadID == EXLoad::E10R) {
        customTexture = static_cast<u8*>(customTexture) + 0x2E40; // skip to e10r textures
    }
    else if (player->characterExload().exLoadID == EXLoad::E10Y) {
        customTexture = static_cast<u8*>(customTexture) + 0x5C80; // SYB: skip to e10y textures
    }

    if (textureID == 2) {
        gvrTexture = customTexture;
    } else if (textureID == 1) {
        gvrTexture = static_cast<u8*>(customTexture) + 0x2B20;
    }

    return gvrTexture;
}

ASMUsed void InitializeTextureHotswap() {
    tex_HotswapTextures = DumpFile("POT", true);
}
#include "context.hpp"
#include "cosmetics/player/exloads.hpp"
#include "filehandler_dat.hpp"

global void* tex_HotswapTextures;

ASMUsed void* TextureHotswapHandler(Player *player, void* gvrTexture, u32 textureID) {
    void* customTexture = tex_HotswapTextures;
    u32 exLoadIndex;
    if (player->character != E10R) return gvrTexture;
    if (!customTexture) return gvrTexture;

    exLoadIndex = Player_EXLoadData[player->input->port][CharacterEXLoadMode].exLoadIndex;

    if (CharacterEXLoadDataSlots[exLoadIndex].exLoadID == E10REXLoad) {
        customTexture = (u8*)customTexture + 0x2E40; // skip to e10r textures
    }

    if (textureID == 2) {
        gvrTexture = customTexture;
    } else if (textureID == 1) {
        gvrTexture = (u8*)customTexture + 0x2B20;
    }

    return gvrTexture;
}

ASMUsed void InitializeTextureHotswap() {
    tex_HotswapTextures = DumpFile("POT", 1);
}
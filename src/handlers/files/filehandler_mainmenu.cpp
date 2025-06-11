#include "lib/files.hpp"

void* tex_HotswapTextures;

ASMUsed void MainMenuFileHandler() {
    tex_HotswapTextures = DumpFile("POT", true);
    // LoadDATFileDynamic("MSFX.DAT");
}
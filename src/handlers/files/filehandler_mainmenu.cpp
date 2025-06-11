#include "filehandler_dat.hpp"

void* tex_HotswapTextures;

ASMUsed void MainMenuFileHandler() {
    tex_HotswapTextures = DumpFile("POT");
    // LoadDATFileDynamic("MSFX.DAT");
}
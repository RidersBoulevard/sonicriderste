#include "filehandler_dat.hpp"

void* tex_HotswapTextures;

ASMUsed void MainMenuFileHandler() {
    tex_HotswapTextures = DumpFile("POT", 1);
    // LoadDATFileDynamic("MSFX.DAT");
}
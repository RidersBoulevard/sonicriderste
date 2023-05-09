#include "context.hpp"

// Declared in ASM
global {
    void lbl_0008C528(Player*);
    void lbl_001A3594(); // sega illusion qte update func
}

struct SegaIllusionQTEObject1 {
    std::array<char, 0x18> filler;
    Player *player;
};

ASMUsed void FixSegaIllusionFastQTE(Player *player) {
    auto lastEight = getObjectList().last<8>(); // check the 8 latest objects
	for(auto &object : lastEight){
		if(lbl_001A3594 != object.update) { continue; }
		if(static_cast<SegaIllusionQTEObject1 *>(object.object)->player != player) { continue; }
		return;
	}

    lbl_0008C528(player);
}
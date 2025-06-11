#include "hypersonic_lighting.hpp"
#include "exloads.hpp"

std::array<HyperSonic_ColorState, MaxPlayerCount> HyperSonic_ColorState;

static constexpr RGBModule RED{1, 0, 0};
static constexpr RGBModule BLUE{0, 0, 1};
static constexpr RGBModule GREEN{0, 1, 0};
static constexpr RGBModule YELLOW{1, 1, 0};
static constexpr RGBModule WHITE{1, 1, 1};
static constexpr RGBModule VIOLET{0.5647058823529412f, 0, 1};

constexpr RGBModule TransitionColorValues(const RGBModule &fromValues, const RGBModule &toValues) {
	// calculate differences
	constexpr auto transitionValue = 0.25F;
	return RGBModule{
		(toValues.R - fromValues.R) * transitionValue,
		(toValues.G - fromValues.G) * transitionValue,
		(toValues.B - fromValues.B) * transitionValue
	};
}

void HyperSonic_RainbowLighting(Player *player) {
	if(player->gearExload().exLoadID != EXLoad::HyperSonic) [[likely]] { // Func is called for all players so most of the time its not hyper sonic
		return;
	}
	if(player->character != Character::SuperSonic) [[unlikely]] { // Todo: band-aid fix. Figure out why eggman's gear exload gets set to hyper
		return;
	}

	const u8 &index          = player->index;
	void **modeldataptr      = &gpsaObject_Player + index;
	void *lightingptr        = static_cast<u32*>(*modeldataptr) + 5;
	void **temp2             = static_cast<void**>(lightingptr);
	void *generallightingptr = static_cast<u32*>(*temp2) + 7;
	void **temp              = static_cast<void**>(generallightingptr);
	auto *lighting           = static_cast<Lighting*>(*temp);
	auto &colorState = HyperSonic_ColorState[index];

	if(player->hyperSonicInit == 0u) {
		lighting->rgb_modules[0] = VIOLET;
		//lighting->rgb_modules[1] = RED;
		//lighting->red1 = 0.5647058823529412f;
		//lighting->red2 = 1;
		//lighting->green1 = 0;
		//lighting->green2 = 0;
		//lighting->blue1 = 1;
		//lighting->blue2 = 0;
		player->hyperSonicInit                        = 1;
		colorState.state            = 0;
		colorState.transitionFrames = 0;
	}

	if(gu32GameCnt % 8 == 0) { // every 8 frames
		colorState.state += 1;
		colorState.transitionFrames = 0;
		if(colorState.state >= 6) {
			colorState.state = 0;
		}
	}

	if(colorState.transitionFrames >= 4) {
		return;
	}
	colorState.transitionFrames += 1;

	switch(colorState.state) {
		case 0: {
			// red
			constexpr RGBModule finalValues = TransitionColorValues(VIOLET, RED);

			lighting->rgb_modules[0] += finalValues;
			//lighting->red1 += finalValues.R;
			//lighting->green1 += finalValues.G;
			//lighting->blue1 += finalValues.B;

			//lighting->red1 = 1;
			//lighting->red2 = 1;
			//lighting->green1 = 0;
			//lighting->green2 = 0;
			//lighting->blue1 = 0;
			//lighting->blue2 = 0;
			break;
		}
		case 1: {
			// blue
			constexpr RGBModule finalValues = TransitionColorValues(RED, BLUE);

			lighting->rgb_modules[0] += finalValues;
			//lighting->red1 += finalValues.R;
			//lighting->green1 += finalValues.G;
			//lighting->blue1 += finalValues.B;

			//lighting->red1 = 0;
			//lighting->red2 = 0;
			//lighting->green1 = 0;
			//lighting->green2 = 0;
			//lighting->blue1 = 1;
			//lighting->blue2 = 1;
			break;
		}
		case 2: {
			// green
			constexpr RGBModule finalValues = TransitionColorValues(BLUE, GREEN);

			lighting->rgb_modules[0] += finalValues;
			//lighting->red1 += finalValues.R;
			//lighting->green1 += finalValues.G;
			//lighting->blue1 += finalValues.B;

			//lighting->red1 = 0;
			//lighting->red2 = 0;
			//lighting->green1 = 1;
			//lighting->green2 = 1;
			//lighting->blue1 = 0;
			//lighting->blue2 = 0;
			break;
		}
		case 3: {
			// yellow
			constexpr RGBModule finalValues = TransitionColorValues(GREEN, YELLOW);

			lighting->rgb_modules[0] += finalValues;
			//lighting->red1 += finalValues.R;
			//lighting->green1 += finalValues.G;
			//lighting->blue1 += finalValues.B;

			//lighting->red1 = 1;
			//lighting->red2 = 1;
			//lighting->green1 = 1;
			//lighting->green2 = 1;
			//lighting->blue1 = 0;
			//lighting->blue2 = 0;
			break;
		}
		case 4: {
			// white
			constexpr RGBModule finalValues = TransitionColorValues(YELLOW, WHITE);

			lighting->rgb_modules[0] += finalValues;
			//lighting->red1 += finalValues.R;
			//lighting->green1 += finalValues.G;
			//lighting->blue1 += finalValues.B;

			//lighting->red1 = 1;
			//lighting->red2 = 1;
			//lighting->green1 = 1;
			//lighting->green2 = 1;
			//lighting->blue1 = 1;
			//lighting->blue2 = 1;
			break;
		}
		case 5: {
			// violet
			constexpr RGBModule finalValues = TransitionColorValues(WHITE, VIOLET);

			lighting->rgb_modules[0] += finalValues;
			//lighting->red1 += finalValues.R;
			//lighting->green1 += finalValues.G;
			//lighting->blue1 += finalValues.B;

			//lighting->red1 = 0.5647058823529412;
			//lighting->red2 = 0.5647058823529412;
			//lighting->green1 = 0;
			//lighting->green2 = 0;
			//lighting->blue1 = 1;
			//lighting->blue2 = 1;
			break;
		}
		default: break;
	}
}

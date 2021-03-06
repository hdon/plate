#ifndef __PLATECOMMON_H__
#define __PLATECOMMON_H__
/*
	PLAtform Engine

	Requires:
		SDL2
		SDL2_Image
		OpenGL
*/
#include <SDL2/SDL.h>
#include "glad/glad.h"
#include <SDL2/SDL_image.h>
#define PLATE_RETURN_SDL_FATAL 127
#define PLATE_RETURN_OTHER_FATAL 63
namespace PLATE
{
	class Display;
	class Plate;
	class Config;
	class Tileset;
	class ColorTileset;
	class TextureTileset;
	class TileLayer;
	class Texture;
	struct color4f
	{
		float r;
		float g;
		float b;
		float a;
	};
}
#include "vec2.h"
#endif //__PLATECOMMON_H__

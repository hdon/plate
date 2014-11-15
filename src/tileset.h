#ifndef __TILESET_H__
#define __TILESET_H__
#include "platecommon.h"
#include "vec2.h"
#include <map>
namespace PLATE
{
	class Tileset
	{
		public:
			virtual void renderBegin(Display * d, Vec2 scroll, Vec2 parallax, Vec2 scale)=0;
			virtual void renderTile(Display * d, int index, int x, int y)=0; 
			virtual void renderEnd(Display * d)=0;
	};
	class ColorTileset : public Tileset
	{
		public:
			ColorTileset(int tw, int th);

			void setTile(int index, SDL_Color c);
			void setTile(int index, int r, int g, int b, int a=255);
			void setTile(int index, float r, float g, float b, float a=1);
			void setTile(int index, color4f c);
			virtual void renderBegin(Display * d, Vec2 scroll, Vec2 parallax, Vec2 scale);
			virtual void renderTile(Display * d, int index, int x, int y);
			virtual void renderEnd(Display * d);

		private:
			std::map<int,color4f> tiles;
			int tw,th;
	};
};
#endif //__TILESET_H__

#include "texture.h"
#include "plate.h"
#include "display.h"
#include <string>
#include <vector>

using namespace PLATE;

Texture::Texture(Display * dpy, const char * basename)
{
	std::string name(basename);
	std::string path;
	std::vector<std::string> searchPath=dpy->getPlate()->getTextureSearchPath();
	for(std::vector<std::string>::iterator search=searchPath.begin();search!=searchPath.end();++search)
	{
		path=name+*search;
		tex=IMG_LoadTexture(dpy->getRenderer(),path.c_str());
		if (tex!=NULL) break;
	}
	SDL_QueryTexture(tex,&format,&access,&w,&h);
}
void Texture::Bind(float * texw, float * texh)
{
	SDL_GL_BindTexture(tex,texw,texh);
}
Texture::~Texture(void)
{
	SDL_DestroyTexture(tex);
}

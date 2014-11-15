#include "display.h"
#include "plate.h"
#include "tileset.h"
#include "tilelayer.h"
#include "texture.h"
#include <random>
using namespace PLATE;
Display::Display(Plate * p, int w, int h, const char * t)
{
	int x,y;
	SDL_RendererInfo rendererInfo;
	plate=p;
	SDL_CreateWindowAndRenderer(w,h,SDL_WINDOW_OPENGL,&win,&rrr);
	if (win==NULL || rrr==NULL)
	{
		p->fatalSDLError("PLATE::Display::Display");
	}
	SDL_GetRendererInfo(rrr, &rendererInfo);
	if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
			(rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
	{
		p->fatalError("PLATE::Display::Display","Could not set up accelerated render context / GL context");
	}
	glctx=SDL_GL_CreateContext(win);
	width=w;
	height=h;
	scroll=Vec2(0,0);
	speed=Vec2(0,0);
	
	ctset=new ColorTileset(16,16);
	tl=new TileLayer((Tileset *)ctset,100,100);
	tl2=new TileLayer((Tileset *)ctset,100,100,Vec2(2,2));

	ctset->setTile(0,1.0,0.0,0.0,0.5);
	ctset->setTile(1,0.0,0.0,1.0,0.5);
	ctset->setTile(2,1.0,0.0,1.0,0.5);
	ctset->setTile(3,0.0,1.0,0.0,0.5);	
	ctset->setTile(4,0.0,0.0,0.0,0.0);

	for (x=0;x<100;x++)
	{
		tl->setTile(x,0,0);
		tl2->setTile(x,0,0);
		tl->setTile(x,1,1);
		tl2->setTile(x,1,1);
		tl->setTile(x,2,2);
		tl2->setTile(x,2,2);
		tl->setTile(x,3,3);
		tl2->setTile(x,3,3);
		tl->setTile(x,4,4);
		tl2->setTile(x,4,4);
	}
	std::default_random_engine generator;
	std::uniform_int_distribution<int> tiledist(0,4);
	for (y=5;y<100;y++)
		for (x=0;x<100;x++)
		{
			int tile=tiledist(generator);
			tl->setTile(x,y,tile);
			tl2->setTile(x,y,tile);
		}
	
	tex=new Texture(this,"marioishBG16x16.png");

	SDL_SetWindowTitle(win,t);
	resetGL();
}
Plate * Display::getPlate(void)
{
	return plate;
}
SDL_Window * Display::getWindow(void)
{
	return win;
}
SDL_Renderer * Display::getRenderer(void)
{
	return rrr;
}
void Display::resetGL(void)
{
	SDL_GL_MakeCurrent(win,glctx);
	orthoGL();
	clearGL();
}
void Display::orthoGL(void)
{
	SDL_GL_MakeCurrent(win,glctx);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho (0,width,height,0,-1,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	clearGL();
}
void Display::clearGL(void)
{
	SDL_GL_MakeCurrent(win,glctx);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void Display::render(void)
{
	float w,h;
	scroll=scroll+speed;
	SDL_GL_MakeCurrent(win,glctx);
	resetGL();
	tl2->render(this,scroll);
	tl->render(this,scroll);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	tex->Bind(&w,&h);
	glColor4f(1.0,1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3d(0,0,0);
	glTexCoord2f(w,0); glVertex3d(width,0,0);
	glTexCoord2f(w,h); glVertex3d(width,height,0);
	glTexCoord2f(0,h); glVertex3d(0,height,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	SDL_GL_SwapWindow(win);
}
void Display::setSize(int w, int h)
{
	//XXX this is not finished
	height=h;
	width=w;
	SDL_SetWindowSize(win,w,h);
	resetGL();
}
void Display::setTitle(const char * t)
{
	SDL_SetWindowTitle(win,t);
}
void Display::handleKey(SDL_KeyboardEvent k)
{
	if (k.type==SDL_KEYUP)
	{
		if (k.keysym.scancode==SDL_SCANCODE_LEFT ||
			k.keysym.scancode==SDL_SCANCODE_RIGHT)
			speed.x=0;
		if (k.keysym.scancode==SDL_SCANCODE_UP ||
			k.keysym.scancode==SDL_SCANCODE_DOWN)
			speed.y=0;
	}
	else
	{
		switch(k.keysym.scancode)
		{
			case SDL_SCANCODE_LEFT:
				speed.x=1;
				break;
			case SDL_SCANCODE_RIGHT:
				speed.x=-1;
				break;
			case SDL_SCANCODE_UP:
				speed.y=1;
				break;
			case SDL_SCANCODE_DOWN:
				speed.y=-1;
				break;
			default:
				break;
		}
	}
}
bool Display::GLok(const char* context, bool term_on_err)
{
	GLenum err=glGetError();
	if (err!=GL_NO_ERROR)
	{
		if (term_on_err)
			plate->fatalError(context,(const char *)gluErrorString(err));
		else
			return false;
	}
	return true;
}
Display::~Display(void)
{
	if (win)
	{
		SDL_GL_DeleteContext(glctx);
		SDL_DestroyWindow(win);
	}
}

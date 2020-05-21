#include "image.h"


ImageData loadTexture(char *path, int num, int hn, int wn, bool ckEnable, Uint8 r, Uint8 g, Uint8 b)
{
	ImageData img;


	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("IMG_Load failed: %s\n", IMG_GetError());
	}
	else
	{
		// Set the color key (transparent pixel) in a surface.
		// https://wiki.libsdl.org/SDL_SetColorKey
		// The color key defines a pixel value that will be treated as transparent in a blit. 
		// It is a pixel of the format used by the surface, as generated by SDL_MapRGB().
		// Use SDL_MapRGB() to map an RGB triple to an opaque pixel value for a given pixel format.
		// https://wiki.libsdl.org/SDL_MapRGB
		SDL_SetColorKey(loadedSurface, ckEnable, SDL_MapRGB(loadedSurface->format, r, g, b));

		// Create texture from surface pixels
		img.texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (img.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		/******************************************************************/
		//Get image dimensions and information
		img.width = loadedSurface->w;
		img.height = loadedSurface->h;
		img.num = num;
		img.wn = wn;
		img.hn = hn;
		/******************************************************************/

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//return newTexture;
	return img;
}

void imgRender(SDL_Renderer *renderer, ImageData img, int posX, int posY, int frame)
{
	int wc = frame % img.wn;
	int hc = frame / img.wn;

	SDL_Rect src, dst;
	src.x = img.width / img.wn * wc;
	src.y = img.height / img.hn * hc;
	src.w = img.width / img.wn;
	src.h = img.height / img.hn;
	dst.x = posX;
	dst.y = posY;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(renderer, img.texture, &src, &dst);
}


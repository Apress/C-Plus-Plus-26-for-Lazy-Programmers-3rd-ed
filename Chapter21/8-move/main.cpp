// Program to show an image on the screen
//      -- from _C++26 for Lazy Programmers_

#include "SSDL.h"
#include "captioned_image.h"

CaptionedImage makeImage (const char* filename, const char* caption)
{
	return CaptionedImage(SSDL_LoadImage(filename), caption);
}

int main(int argc, char **argv)
{
	CaptionedImage img1 = makeImage("media/flamingo.png", "flamingo");
	CaptionedImage img2; img2 = makeImage("media/pupdog.png", "Puppy");

	img1.draw(0, 0);
	img2.draw(200, 0);

	SSDL_WaitKey();

	return 0;
}

//file: texturePaletter.h
#ifndef _texturePaletter_H_
#define _texturePaletter_H_

#include <vector>
#include <FreeImage.h>

/**/
struct texturePaletter
{
	// typedefs
private:
	typedef std::vector<unsigned char> TArrayByte;
	typedef std::vector<RGBQUAD> TArrayRGBQUAD;

	// disabled
private:
	~texturePaletter();

	// static public methods
public:
	static void Run(
		TArrayRGBQUAD& out_palette,
		TArrayByte& out_pixelIndex,
		FIBITMAP& in_image,
		const int in_palletColourCount 
		);
};

#endif // _texturePaletter_H_

#ifndef TGA_HEADER_H
#define TGA_HEADER_H

struct TGAHEADER
{
	char	IDLength;
	char	ColorMapType;
	char	DataTypeCode;
	short int ColorMapOrigin;
	short int ColorMapLength;
	char	ColorMapDepth;
	short int x_origin;
	short int y_origin;
	short width;
	short height;
	char	BitsPerPixel;
	char	ImageDescriptor;

	TGAHEADER()
		: IDLength(0), ColorMapType(0), DataTypeCode(0), ColorMapOrigin(0), ColorMapLength(0), ColorMapDepth(0),
			x_origin(0), y_origin(0), width(0), height(0), BitsPerPixel(0), ImageDescriptor(0)
	{}
};

GLbyte* myReadTGABits(const void * lTexture, GLint & nWidth, GLint & nHeight, GLint & nComponents, GLenum & eFormat);
unsigned char* getTGAFile(const char * const fileName);

#endif
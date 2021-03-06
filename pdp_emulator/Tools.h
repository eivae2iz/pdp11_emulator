#pragma once
#include "inttypes.h"
#include "BITMAP.h"
using namespace System::Drawing;
using namespace System;
using namespace System::Drawing::Imaging;

class Tools
{
public:
	static bool getBiteByPosition(int16_t* buffer, int position);
	static void setBiteByPosition(int16_t* buffer, int position, bool bite);
	static BITMAP* readBMP(const char *);
	static bool isBlack(int r, int g, int b);
	Tools();
};


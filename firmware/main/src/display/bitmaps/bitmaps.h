#pragma once

#include "kodalogo.h"
#include "egg1.h"
#include "egg2.h"
#include "egg3.h"
#include "egg4.h"

// Array of all bitmaps for convenience.
const int epd_bitmap_allArray_LEN = 5;
const unsigned char* epd_bitmap_allArray[5] = {
	logo_bitmap,
	egg1_bitmap,
	egg2_bitmap,
	egg3_bitmap,
	egg4_bitmap,
};
// CPPFLAGS="-I/var/tmp/niluje/leptonica/src"
#include <allheaders.h>

#include "ffi-cdecl.h"

cdecl_type(l_int8)
cdecl_type(l_uint8)
cdecl_type(l_int16)
cdecl_type(l_uint16)
cdecl_type(l_int32)
cdecl_type(l_uint32)
cdecl_type(l_float32)
cdecl_type(l_float64)

cdecl_type(BOX)
cdecl_type(BOXA)
cdecl_type(NUMA)
cdecl_type(PIX)
cdecl_type(PIXA)

cdecl_const(L_NOCOPY)
cdecl_const(L_COPY)
cdecl_const(L_CLONE)

cdecl_func(boxAdjustSides)
cdecl_func(boxCreate)
cdecl_func(boxDestroy)
cdecl_func(boxGetGeometry)
cdecl_func(boxOverlapRegion)
cdecl_func(boxaAddBox)
cdecl_func(boxaClipToBox)
cdecl_func(boxaCombineOverlaps)
cdecl_func(boxaCreate)
cdecl_func(boxaDestroy)
cdecl_func(boxaGetBox)
cdecl_func(boxaGetBoxGeometry)
cdecl_func(boxaGetCount)
cdecl_func(numaCreateFromFArray)
cdecl_func(numaDestroy)
cdecl_func(numaGetCount)
cdecl_func(numaGetFArray)
cdecl_func(numaGetIValue)
cdecl_func(pixClipRectangle)
cdecl_func(pixClone)
cdecl_func(pixConnCompBB)
cdecl_func(pixConvertRGBToGrayFast)
cdecl_func(pixConvertTo32)
cdecl_func(pixDestroy)
cdecl_func(pixDrawBoxaRandom)
cdecl_func(pixGetDepth)
cdecl_func(pixGetHeight)
cdecl_func(pixGetRegionsBinary)
cdecl_func(pixGetWidth)
cdecl_func(pixInvert)
cdecl_func(pixMultiplyByColor)
cdecl_func(pixSplitIntoBoxa)
cdecl_func(pixThresholdToBinary)
cdecl_func(pixWriteMemPng)
cdecl_func(pixWritePng)
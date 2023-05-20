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

cdecl_struct(Box)
cdecl_type(BOX)
cdecl_struct(Sel)
cdecl_type(Sel)

cdecl_struct(Boxa)
cdecl_type(BOXA)
cdecl_struct(Numa)
cdecl_type(NUMA)

cdecl_const(L_INSERT)
cdecl_const(L_COPY)
cdecl_const(L_CLONE)
cdecl_const(L_COPY_CLONE)

cdecl_struct(PixColormap)
cdecl_struct(Pix)
cdecl_type(PIX)

cdecl_func(boxCreate)
cdecl_func(boxaCreate)
cdecl_func(boxaGetBox)
cdecl_func(numaCreate)
cdecl_func(numaCreateFromFArray)
cdecl_func(boxaCombineOverlaps)
cdecl_func(boxaClipToBox)
cdecl_func(pixConnCompBB)
cdecl_func(boxCopy)
cdecl_func(boxClone)
cdecl_func(boxOverlapRegion)
cdecl_func(boxAdjustSides)
cdecl_func(boxaAddBox)
cdecl_func(numaGetMax)
cdecl_func(numaGetIValue)
cdecl_func(boxaGetCount)
cdecl_func(numaGetCount)
cdecl_func(boxaWrite)
cdecl_func(boxDestroy)
cdecl_func(boxaDestroy)
cdecl_func(numaDestroy)
cdecl_func(pixDestroy)
cdecl_func(pixWritePng)
cdecl_func(pixWriteMemPng)
cdecl_func(pixGetWidth)
cdecl_func(pixGetHeight)
cdecl_func(pixGetDepth)
cdecl_func(pixGetWpl)
cdecl_func(pixSetPixel)
cdecl_func(pixGetData)
cdecl_func(pixCreate)
cdecl_func(pixClone)
cdecl_func(pixConvertTo1)
cdecl_func(pixThresholdToBinary)
cdecl_func(pixConvertRGBToGrayFast)
cdecl_func(pixConvertTo32)
cdecl_func(pixDrawBoxaRandom)
cdecl_func(pixMultiplyByColor)
cdecl_func(pixBlendBackgroundToColor)
cdecl_func(pixBlockconv)
cdecl_func(pixRenderContours)
cdecl_func(pixInvert)
cdecl_func(pixClipRectangle)
cdecl_func(pixOpen)
cdecl_func(pixClose)
cdecl_func(pixErode)
cdecl_func(pixGetRegionsBinary)
cdecl_func(pixSplitIntoBoxa)
cdecl_func(pixReduceRankBinaryCascade)
cdecl_func(selCreate)
cdecl_func(selSetElement)
cdecl_func(selPrintToString)
cdecl_func(selDestroy)
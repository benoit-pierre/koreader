#ifndef WILLUS_CONFIG_H
#define WILLUS_CONFIG_H

#mesondefine HAVE_Z_LIB
#mesondefine HAVE_PNG_LIB
#mesondefine HAVE_JPEG_LIB
#mesondefine HAVE_MUPDF_LIB
#mesondefine HAVE_GHOSTSCRIPT
#mesondefine HAVE_DJVU_LIB
#mesondefine HAVE_GOCR_LIB
#mesondefine HAVE_LEPTONICA_LIB
#mesondefine HAVE_TESSERACT_LIB
#mesondefine HAVE_JASPER_LIB
#mesondefine HAVE_GSL_LIB

#ifndef K2PDFOPT_EXPORT
#define K2PDFOPT_EXPORT
#endif

#define K2PDFOPT_KINDLEPDFVIEWER

#endif

/*******************************************************************/
//
// GRADIENT.CPP
//
// Object to draw a gradient within an area
//
// Copyright (C) 1998 JulMar Technology, Inc.
// All rights reserved
//
// TSP++ Version 3.00 PBX/ACD Emulator Projects
// Internal Source Code - Do Not Release
//
// Modification History
// --------------------
// 1998/09/05 MCS@JulMar	Initial revision
//
/*******************************************************************/

/*---------------------------------------------------------------*/
// INCLUDE FILES
/*---------------------------------------------------------------*/
#include "stdafx.h"
#include "gradient.h"
using namespace CGradient;

#pragma warning(disable:4244)

/*---------------------------------------------------------------*/
// DEBUG INFORMATION
/*---------------------------------------------------------------*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*---------------------------------------------------------------*/
// CONSTANTS
/*---------------------------------------------------------------*/
const UINT HLSMAX = 240;                    // Range for HLS
const UINT RGBMAX = 255;                    // Max for RGB value
const UINT UNDEFINED_HUE = (HLSMAX*2/3);    // Hue for gray 
const UINT clrStart = 192;					// Hi-end of the color
const UINT clrEnd = 64;						// Lo-end of the color

/*****************************************************************************
** Procedure:  HueToRGB
**
** Arguments:  
**
** Returns:    RGB value
**
** Description:  Utility function to convert between HLSmax units and 
**               RGBmax units
**
*****************************************************************************/
static WORD HueToRGB(WORD n1, WORD n2, WORD hue)
{
    if (hue >= HLSMAX)
        hue -= HLSMAX;

    // return r,g, or b value from this tridrant
    if (hue < (HLSMAX/6))
        return static_cast<WORD>(( n1 + (((n2-n1)*hue+(HLSMAX/12))/(HLSMAX/6)) ));
    if (hue < (HLSMAX/2))
        return ( n2 );
    if (hue < ((HLSMAX*2)/3))
        return static_cast<WORD>(( n1 + (((n2-n1)*(((HLSMAX*2)/3)-hue)+(HLSMAX/12)) / (HLSMAX/6)) ));
    else
        return ( n1 );
        
}// HueToRGB

/*****************************************************************************
** Procedure:  RGBtoHLS
**
** Arguments:  'clrRGB' - RGB color to translate
**             'H' - returning hue
**             'L' - returning luminosity
**             'S' - returning saturation
**
** Returns:    void
**
** Description:  This function takes a 24-bit RGB value and translates it
**               into a relative HLS value
**
*****************************************************************************/
static void RGBtoHLS(COLORREF lRGBColor, WORD& H, WORD& L, WORD& S)
{
    WORD R,G,B;                   // input RGB values
    WORD cMax,cMin;               // max and min RGB values
    WORD cSum,cDif;
    short  Rdelta,Gdelta,Bdelta;  // intermediate value: % of spread from max

    // get R, G, and B out of DWORD
    R = GetRValue(lRGBColor);
    G = GetGValue(lRGBColor);
    B = GetBValue(lRGBColor);

    // calculate lightness
    cMax = max( max(R,G), B);
    cMin = min( min(R,G), B);
    cSum = cMax + cMin;
    L = (WORD)(((cSum * (DWORD)HLSMAX) + RGBMAX )/(2*RGBMAX));

    cDif = cMax - cMin;
    if (!cDif)                   // r=g=b --> achromatic case
    {
        S = 0;                   // saturation
        H = UNDEFINED_HUE;           // hue
    }
    else                         // chromatic case
    {
        // Saturation
        // Note: division by cSum is not a problem, as cSum can only be 0 if the 
        // RGB value is 0L, and that is achromatic.
        if (L <= (HLSMAX/2))
            S = (WORD)(((cDif * (DWORD) HLSMAX) + (cSum / 2) ) / cSum);
        else
            S = (WORD)((DWORD)((cDif*(DWORD)HLSMAX) + (DWORD)((2*RGBMAX-cSum)/2)) / (2*RGBMAX-cSum));
            
        // Hue            
        Rdelta = (short) (( ((cMax-R)*(DWORD)(HLSMAX/6)) + (cDif / 2) ) / cDif);
        Gdelta = (short) (( ((cMax-G)*(DWORD)(HLSMAX/6)) + (cDif / 2) ) / cDif);
        Bdelta = (short) (( ((cMax-B)*(DWORD)(HLSMAX/6)) + (cDif / 2) ) / cDif);

        if (R == cMax)
            H = Bdelta - Gdelta;
        else if (G == cMax)
            H = (HLSMAX/3) + Rdelta - Bdelta;
        else // B == cMax
            H = ((2*HLSMAX)/3) + Gdelta - Rdelta;

        if ((short)H < 0)  // This can occur when R == cMax and G is > B
            H += HLSMAX;
        if (H >= HLSMAX)
            H -= HLSMAX;
    }       
    
}// RGBtoHLS

/*****************************************************************************
** Procedure:  HLStoRGB
**
** Arguments:  'H' - input hue
**             'L' - input luminosity
**             'S' - input saturation
**
** Returns:    RGB value
**
** Description:  This function takes HLS value and converts it into a 24-bit
**               RGB value
**
*****************************************************************************/
static COLORREF HLStoRGB(WORD hue,WORD lum,WORD sat)
{
    WORD R,G,B;                // RGB component values
    WORD  Magic1,Magic2;       // Calculated magic numbers (really!)

    if (sat == 0)              // achromatic case
    {
        R = G = B = (lum * RGBMAX) / HLSMAX;
    }
    else                       // chromatic case
    {
        // set up magic numbers
        if (lum <= (HLSMAX/2))
            Magic2 = (WORD)((lum * ((DWORD)HLSMAX + sat) + (HLSMAX/2))/HLSMAX);
        else
            Magic2 = lum + sat - (WORD)(((lum*sat) + (DWORD)(HLSMAX/2))/HLSMAX);
        Magic1 = 2*lum-Magic2;

        // get RGB, change units from HLSMAX to RGBMAX
        R = (WORD)((HueToRGB(Magic1,Magic2,hue+(HLSMAX/3))*(DWORD)RGBMAX + (HLSMAX/2))) / HLSMAX;
        G = (WORD)((HueToRGB(Magic1,Magic2,hue)*(DWORD)RGBMAX + (HLSMAX/2))) / HLSMAX;
        B = (WORD)((HueToRGB(Magic1,Magic2,hue-(HLSMAX/3))*(DWORD)RGBMAX + (HLSMAX/2))) / HLSMAX;
    }
    return(RGB(R,G,B));
    
}// HLStoRGB

/*****************************************************************************
** Procedure:  CGradient::Draw
**
** Arguments:  'pDC' - Device Context to draw into
**             'rcBox' - Rectangle to draw into
**             'clrStart' - Starting color
**             'iDir' - Direction (0 = top to bottom, 1=left to right)
**
** Returns:    void
**
** Description:  Draws a gradient into a specified box
**
*****************************************************************************/
void CGradient::Draw(CDC* pDC, CRect& rcBox, COLORREF clrBack, int iDir)
{
    WORD wHue, wSat, wLum;
    RGBtoHLS (clrBack, wHue, wLum, wSat);

   	if (iDir == 0 || iDir == 1) // Top to bottom
	{
		int iHeight = rcBox.Height();
		int iScale = iHeight / (clrStart - clrEnd);
		int cScale = 1;

		if (iScale == 0)
		{
			iScale = 1;
			cScale = (clrStart - clrEnd) / iHeight;
		}

		int x = min((clrStart - clrEnd), iHeight);
		int iColor = (iDir == 0) ? wLum + x : wLum - x;
		if (iDir == 0 && iColor > clrStart)
			iColor = clrStart;

		int iTop = rcBox.top;
		CRect rc;
		rc.left = rcBox.left;
		rc.right = rcBox.right;

		while (x--)
		{   
			COLORREF clr = HLStoRGB (wHue, (WORD)iColor, wSat);
			CBrush br (RGB (GetRValue(clr), GetGValue(clr), GetBValue(clr)));
			if (iDir == 0)
				iColor -= cScale;
			else
				iColor += cScale;
    
			rc.top = iTop;
			rc.bottom = rc.top + iScale;
			iTop += iScale;
    
			pDC->FillRect(&rc, &br);
		}   
	}
	// Left to right
	else if (iDir == 2 || iDir == 3)
	{
		int iWidth = rcBox.Width();
		int iScale = iWidth / (clrStart - clrEnd);
		int cScale = 1;

		if (iScale == 0)
		{
			iScale = 1;
			cScale = (clrStart - clrEnd) / iWidth;
		}

		int x = min((clrStart - clrEnd), iWidth);
		int iColor = (iDir == 2) ? wLum + x : wLum - x;
		if (iDir == 2 && iColor > clrStart)
			iColor = clrStart;

		int iLeft = rcBox.left;
		CRect rc;
		rc.top = rcBox.top;
		rc.bottom = rcBox.bottom;

		while (x--)
		{   
			COLORREF clr = HLStoRGB (wHue, (WORD)iColor, wSat);
			CBrush br (RGB (GetRValue(clr), GetGValue(clr), GetBValue(clr)));
			if (iDir == 2)
				iColor -= cScale;
			else
				iColor += cScale;
    
			rc.left = iLeft;
			rc.right = rc.left + iScale;
			iLeft += iScale;
    
			pDC->FillRect(&rc, &br);
		}   

		if (rc.right < rcBox.right)
		{
			COLORREF clr = HLStoRGB (wHue, (WORD)iColor, wSat);
			CBrush br (RGB (GetRValue(clr), GetGValue(clr), GetBValue(clr)));
			rc.right = rcBox.right;
			pDC->FillRect(&rc, &br);
		}
	}

}// CGradient::Draw


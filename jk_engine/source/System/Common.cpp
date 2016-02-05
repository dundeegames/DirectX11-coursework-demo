////////////////////////////////////////////////////////////////////////////////
/**
* \file       Common.cpp
* \brief      Common class -
*
* \details    Commonly used defines and static consts
*
* \author     Jiri Klic
* \version    1.0
* \date       October 2015
* \pre
* \post
* \bug        No known bugs
* \warning
* \todo
*
* \copyright  University of Abertay - Dundee.2013.
*             Intellectual Property Policy.[online].Available from:
*             http://www.abertay.ac.uk/media/Intellectual-Property-Policy-v2-01.pdf
*             [Accessed 22 April 2015].
*
*///////////////////////////////////////////////////////////////////////////////





// INCLUDES ////////////////////////////////////////////////////////////////////
#include"Common.h"

#include <stdio.h>      /* fopen, fputs, fclose, stderr */
#include <stdlib.h>     /* abort, NULL */


// FUNCTIONS ///////////////////////////////////////////////////////////////////


Common::Common()
{

}


Common::~Common()
{

}


// -----------------------------------------------------------------------------

void Common::die(const char* message)
{
  fputs(message, stderr);
  abort();
}





// 80 //////////////////////////////////////////////////////////////////////////
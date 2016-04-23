////////////////////////////////////////////////////////////////////////////////
/**
* \file       Common.h
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


#ifndef COMMON_H
#define COMMON_H


// INCLUDES ////////////////////////////////////////////////////////////////////





// DEFINES /////////////////////////////////////////////////////////////////////
#define DELETE_OBJECT(object)   if ((object) != nullptr) { delete object; object = nullptr; }
#define DELETE_OBJECTS(objects) if ((objects) != nullptr) { delete[] objects; objects = nullptr; }
#define RELEASE_OBJECT(object)  if ((object) != nullptr) { object->Release(); object = nullptr; }

#define PI 3.14159265359f
#define RADIANS(x) (x*PI/180)

#define COLOUR_DEFAULT 0.392f, 0.584f, 0.929f, 1.0f
#define COLOUR_ZERO    0.0f, 0.0f, 0.0f, 0.0f

// CLASS ///////////////////////////////////////////////////////////////////////

class Common
{

public:
  Common();
  ~Common();

  static void die(const char* message);

private:


};


#endif // COMMON_H




// 80 //////////////////////////////////////////////////////////////////////////
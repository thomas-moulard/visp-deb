/****************************************************************************
 *
 * $Id: vpImagePoint.h 2359 2009-11-24 15:09:25Z nmelchio $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2013 by INRIA. All rights reserved.
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional 
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 * 
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Write videos and sequences of images.
 *
 * Authors:
 * Nicolas Melchior
 *
 *****************************************************************************/

/*!
  \file vpVideoWriter.h
  \brief Write videos and sequences of images.
*/

#ifndef vpVideoWriter_H
#define vpVideoWriter_H

#include <string>

#include <visp/vpImageIo.h>
#include <visp/vpFFMPEG.h>

/*!
  \class vpVideoWriter

  \ingroup Video

  \brief Class that enables to write easily a video file or a sequence of images.
 
  The following example shows how this class is really easy to use. It enable to write an image sequence. The images are stored in the folder "./image" and are named "image0000.jpeg", "image0001.jpeg", "image0002.jpeg", ...
  
  \code
  #include <visp/vpConfig.h>
  #include <visp/vpVideoWriter.h>
 
  int main()
  {
  vpImage<vpRGBa> I;

  vpVideoWriter writer;
  
  //Initialize the writer.
  writer.setFileName("./image/image%04d.jpeg");
 
  writer.open(I);
 
  for ( ; ; )
  {
    //Here the code to capture or create an image and stores it in I.
  
    //Save the image
    writer.saveFrame(I);
  }
  
  writer.close();

  return 0;
  }
  \endcode
  
  The other following example explains how to use the class to write directly an mpeg file.
  
  \code
#include <visp/vpVideoWriter.h>

int main()
{
#ifdef VISP_HAVE_FFMPEG
  vpImage<vpRGBa> I;

  vpVideoWriter writer;

  // Set up the framerate to 30Hz. Default is 25Hz.
  writer.setFramerate(30);
  // Set up the bit rate
  writer.setBitRate(1000000);
  // Set up the codec to use
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(54,51,110) // libavcodec 54.51.100
  writer.setCodec(CODEC_ID_MPEG2VIDEO);
#else
  writer.setCodec(AV_CODEC_ID_MPEG2VIDEO);
#endif
  writer.setFileName("./test.mpeg");

  writer.open(I);

  for ( ; ; )
  {
    // Here the code to capture or create an image and store it in I.

    // Save the image
    writer.saveFrame(I);
  }

  writer.close();

  return 0;
#endif
}
  \endcode
*/

class VISP_EXPORT vpVideoWriter
{    
  private:   
#ifdef VISP_HAVE_FFMPEG
    //!To read video files
    vpFFMPEG *ffmpeg;
    //!The codec to use
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(54,51,110) // libavcodec 54.51.100
    CodecID codec;
#else
    AVCodecID codec;
#endif
    //!The bite rate
    unsigned int bit_rate;
    int framerate;
#endif
    //!Types of available formats
    typedef enum
    {
      FORMAT_PGM,
      FORMAT_PPM,
      FORMAT_JPEG,
      FORMAT_PNG,
      FORMAT_AVI,
      FORMAT_MPEG,
      FORMAT_MOV,
      FORMAT_UNKNOWN
    } vpVideoFormatType;
    
    //!Video's format which has to be writen
    vpVideoFormatType formatType;
    
    //!Path to the image sequence
    char fileName[FILENAME_MAX];
    
    //!Indicates if the path to the image sequence is set.
    bool initFileName;
    
    //!Indicates if the video is "open".
    bool isOpen;
    
    //!Count the frame number.
    unsigned int frameCount;
    
    //!The first frame index.
    unsigned int firstFrame;
    
    //!Size of the frame
    unsigned int width;
    unsigned int height;

  public:
    vpVideoWriter();
    ~vpVideoWriter();
    
    void close();

    /*!
      Gets the current frame index.

      \return Returns the current frame index.
    */
    inline unsigned int getCurrentFrameIndex() const {return frameCount;}

    void open (vpImage< vpRGBa > &I);
    void open (vpImage< unsigned char > &I);
    /*!
      Reset the frame counter and sets it to the first image index.

      By default the first frame index is set to 0.
    */
    inline void resetFrameCounter() {frameCount = firstFrame;}

    void saveFrame (vpImage< vpRGBa > &I);
    void saveFrame (vpImage< unsigned char > &I);

#ifdef VISP_HAVE_FFMPEG
    /*!
      Sets the bit rate of the video when encoding.

      \param bit_rate : the expected bit rate.

      By default the bit rate is set to 500 000.
    */
    inline void setBitRate(const unsigned int bit_rate) {this->bit_rate = bit_rate;}

    /*!
      Sets the codec used to encode the video.

      \param codec : the expected codec.

      By default codec is set to AV_CODEC_ID_MPEG1VIDEO. But if installed, you can use one of the
      AVCodecID proposed by ffmpeg such as : AV_CODEC_ID_MPEG2VIDEO, AV_CODEC_ID_MPEG2VIDEO_XVMC,
      AV_CODEC_ID_MPEG4, AV_CODEC_ID_H264, ... (More AVCodecID can be found in the ffmpeg documentation).

      Of course to use the codec it must be installed on your computer.
    */
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(54,51,110) // libavcodec 54.51.100
    inline void setCodec(const CodecID codec) {this->codec = codec;}
#else
    inline void setCodec(const AVCodecID codec) {this->codec = codec;}
#endif
#endif

    void setFileName(const char *filename);
    void setFileName(const std::string &filename);
    /*!
      Enables to set the first frame index.
      
      \param firstFrame : The first frame index.
    */
    inline void setFirstFrameIndex(const unsigned int firstFrame) {this->firstFrame = firstFrame;}
#ifdef VISP_HAVE_FFMPEG
    /*!
      Sets the framerate in Hz of the video when encoding.

      \param framerate : the expected framerate.

      By default the framerate is set to 25Hz.
    */
    inline void setFramerate(const int framerate) {
      this->framerate = framerate;
    }
#endif

    private:
      vpVideoFormatType getFormat(const char *filename);
      static std::string getExtension(const std::string &filename);
};

#endif

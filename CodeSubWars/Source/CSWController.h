// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)

#pragma once


//ControlCommand (deckt ab: move rotation ... eben alles was mit steuern zu tun hat)
//-braucht zur initialisierung:
//  -ControlCenter
//  -spezialisierten Controller, der dann rotation etc. ausführt
//
//
//
//relative rotation controller, die das uboot um lokale achsen drehen. als orientierung muss dann aber ein lokales referenzsystem benutzt werden.
//gegenüber diesem system werden dann die winkel um x,y,z bestimmt. 
//
//absolute rotation controller benutzen als referenzsystem immer das absolute (für orientierung und geschwindigkeit). sie erhalten die kontrolle über
//alle 3 maschinen.
// -orientierung: die absolute orientierung muss ermittelt werden. anhand der aktuellen oritenierung und der zielorientierung wird die ansteuerung 
// der einzelnen maschienen vorgenommen 
// -geschwindigkeit: die absolute geschwindigkeit muss ermittelt werden. anhand dieser geschwindigkeit und der zielgeschwindigkeit wird die ansteuerung 
// der einzelnen maschienen vorgenommen

namespace CodeSubWars
{

  class CSWController
  {
    public:
      typedef std::shared_ptr<CSWController> PtrType;
  
      virtual ~CSWController() {}

      virtual void initialize() = 0;

      virtual void update() = 0;
    
      virtual void finalize() = 0;

      virtual bool hasFinished() = 0;

      virtual const std::string& getName() const = 0;
    
      virtual std::string getDetails() const { return std::string(); }
    
      virtual double getProgress() const = 0;
  };

}
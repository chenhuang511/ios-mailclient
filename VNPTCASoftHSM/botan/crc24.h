/*
* CRC24
* (C) 1999-2007 Jack Lloyd
*
* Distributed under the terms of the Botan license
*/

#ifndef BOTAN_CRC24_H__
#define BOTAN_CRC24_H__

#include "hash.h"

namespace Botan {

/**
* 24-bit cyclic redundancy check
*/
class  CRC24 : public HashFunction
   {
   public:
      std::string name() const { return "CRC24"; }
      size_t output_length() const { return 3; }
      HashFunction* clone() const { return new CRC24; }

      void clear() { crc = 0xB704CE; }

      CRC24() { clear(); }
      ~CRC24() { clear(); }
   private:
      void add_data(const byte[], size_t);
      void final_result(byte[]);
      u32bit crc;
   };

}

#endif

/*
* Data Store
* (C) 1999-2007 Jack Lloyd
*
* Distributed under the terms of the Botan license
*/

#ifndef BOTAN_DATA_STORE_H__
#define BOTAN_DATA_STORE_H__

#include "secmem.h"
#include <utility>
#include <string>
#include <vector>
#include <map>

namespace Botan {

/**
* Data Store
*/
class  Data_Store
   {
   public:
      /**
      * A search function
      */
      class  Matcher
         {
         public:
            virtual bool operator()(const std::string&,
                                    const std::string&) const = 0;

            virtual std::pair<std::string, std::string>
               transform(const std::string&, const std::string&) const;

            virtual ~Matcher() {}
         };

      bool operator==(const Data_Store&) const;

      std::multimap<std::string, std::string>
         search_with(const Matcher&) const;

      std::vector<std::string> get(const std::string&) const;

      std::string get1(const std::string&) const;

      MemoryVector<byte> get1_memvec(const std::string&) const;
      u32bit get1_u32bit(const std::string&, u32bit = 0) const;

      bool has_value(const std::string&) const;

      void add(const std::multimap<std::string, std::string>&);
      void add(const std::string&, const std::string&);
      void add(const std::string&, u32bit);
      void add(const std::string&, const MemoryRegion<byte>&);
   private:
      std::multimap<std::string, std::string> contents;
   };

}

#endif

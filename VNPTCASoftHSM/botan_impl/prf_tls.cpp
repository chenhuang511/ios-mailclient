/*
* TLS v1.0 and v1.2 PRFs
* (C) 2004-2010 Jack Lloyd
*
* Distributed under the terms of the Botan license
*/

#include "../botan/prf_tls.h"
#include "../botan/xor_buf.h"
#include "../botan/hmac.h"
#include "../botan/md5.h"
#include "../botan/sha160.h"

namespace Botan {

namespace {

/*
* TLS PRF P_hash function
*/
void P_hash(MemoryRegion<byte>& output,
            MessageAuthenticationCode* mac,
            const byte secret[], size_t secret_len,
            const byte seed[], size_t seed_len)
   {
   mac->set_key(secret, secret_len);

   SecureVector<byte> A(seed, seed_len);

   size_t offset = 0;

   while(offset != output.size())
      {
      const size_t this_block_len =
         std::min<size_t>(mac->output_length(), output.size() - offset);

      A = mac->process(A);

      mac->update(A);
      mac->update(seed, seed_len);
      SecureVector<byte> block = mac->final();

      xor_buf(&output[offset], &block[0], this_block_len);
      offset += this_block_len;
      }
   }

}

/*
* TLS PRF Constructor and Destructor
*/
TLS_PRF::TLS_PRF()
   {
   hmac_md5 = new HMAC(new MD5);
   hmac_sha1 = new HMAC(new SHA_160);
   }

TLS_PRF::~TLS_PRF()
   {
   delete hmac_md5;
   delete hmac_sha1;
   }

/*
* TLS PRF
*/
SecureVector<byte> TLS_PRF::derive(size_t key_len,
                                   const byte secret[], size_t secret_len,
                                   const byte seed[], size_t seed_len) const
   {
   SecureVector<byte> output(key_len);

   size_t S1_len = (secret_len + 1) / 2,
          S2_len = (secret_len + 1) / 2;
   const byte* S1 = secret;
   const byte* S2 = secret + (secret_len - S2_len);

   P_hash(output, hmac_md5,  S1, S1_len, seed, seed_len);
   P_hash(output, hmac_sha1, S2, S2_len, seed, seed_len);

   return output;
   }

/*
* TLS v1.2 PRF Constructor and Destructor
*/
TLS_12_PRF::TLS_12_PRF(MessageAuthenticationCode* mac) : hmac(mac)
   {
   }

TLS_12_PRF::~TLS_12_PRF()
   {
   delete hmac;
   }

SecureVector<byte> TLS_12_PRF::derive(size_t key_len,
                                      const byte secret[], size_t secret_len,
                                      const byte seed[], size_t seed_len) const
   {
   SecureVector<byte> output(key_len);

   P_hash(output, hmac, secret, secret_len, seed, seed_len);

   return output;
   }

}

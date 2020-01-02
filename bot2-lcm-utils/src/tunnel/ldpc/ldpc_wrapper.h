/*
 * This file is part of bot2-lcm-utils.
 *
 * bot2-lcm-utils is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-lcm-utils is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-lcm-utils. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_WRAPPER_H_
#define BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_WRAPPER_H_

/*
 * ldpc_wrapper.h
 *
 *  Created on: Mar 5, 2009
 *      Author: abachrac
 */

#include <stdint.h>

#include "ldpc_scheme.h"
// IWYU pragma: no_forward_declare LDPCFecScheme

class ldpc_wrapper {
public:
  ldpc_wrapper()
  {
  }
  ~ldpc_wrapper();

  inline int getNumPackets()
  {
    return nbPKT;
  }

  //used for both
  int getObject(uint8_t * pktBuf);

protected:
  int init(int objSize_, int pktSize_, double fec_rate_, int typeFlag);
  int typeFlag;

  int nbDATA; /* k parameter */
  int nbFEC; /* n - k parameter */
  int nbSYMBOLS; /* n parameter */

  int pktSize; /* packet size */
  int symbolSize; /* symbol size */

  int nbSymbolsPerPkt;
  int nbDATAPkts;
  int nbFECPkts;
  int nbPKT;

  int objSize;
  double fec_ratio;
  LDPCFecScheme * MyFecScheme;

  uint8_t **data; /* filled with original data symbols AND  built FEC symbols */
  int packetNum; /* number of packets sent/received */
};

class ldpc_enc_wrapper: public ldpc_wrapper {
public:
  //encoder stuff:
  ldpc_enc_wrapper(uint8_t * data_to_send, int objSize, int packetSize, double fec_rate); //initializer for encoder
  int getNextPacket(uint8_t * pktBuf, int16_t * ESI);
  int encodeData(uint8_t * data_to_send);
};

class ldpc_dec_wrapper: public ldpc_wrapper {
public:
  //decoder stuff:
  ldpc_dec_wrapper(int objSize, int packetSize, double fec_rate); //initializer for decoder
  int processPacket(uint8_t * newPkt, int16_t ESI);
};

#endif  // BOT2_LCM_UTILS_TUNNEL_LDPC_LDPC_WRAPPER_H_

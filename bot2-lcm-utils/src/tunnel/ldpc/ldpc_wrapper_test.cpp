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

// ldpc_wrapper_test.cpp
//
//  Created on: Mar 9, 2009
//      Author: abachrac

#include "ldpc_wrapper.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

static inline double getTime(void) {
  struct timeval tv;
  double t;

  if (gettimeofday(&tv, NULL) < 0) {
    printf("carmen_get_time encountered error in gettimeofday : %s\n",
           strerror(errno));
  }
  t = tv.tv_sec + tv.tv_usec / 1000000.0;
  return t;
}

int main(int argc, char* argv[]) {
  unsigned int seed = time(NULL);
  int numRuns = 1000;
  int numSuccess = 0;

  ldpc_enc_wrapper* ldpc_enc;
  ldpc_dec_wrapper* ldpc_dec;
  double t0 = 0;
  double t1 = 0;
  double t2 = 0;

  int packetSize = 1024;
  double fec_rate = 1.3;
  double dropfrac = .1;
  if (argc >= 2) {
    fec_rate = atof(argv[1]);
  }
  if (argc >= 3) {
    dropfrac = atof(argv[2]);
    if (dropfrac > 1) {
      dropfrac /= 100;
    }
  }
  unsigned int messageSize = 142230;  // average stereo image size

  for (int r = 0; r < numRuns; r++) {
    uint8_t* message = (uint8_t*)calloc(messageSize, 1);
    for (unsigned int i = 0; i < messageSize; i++) {
      message[i] = (uint8_t)rand_r(&seed);
    }

    t0 += getTime();
    ldpc_enc = new ldpc_enc_wrapper(message, messageSize, packetSize, fec_rate);
    t1 += getTime();
    ldpc_dec = new ldpc_dec_wrapper(messageSize, packetSize, fec_rate);
    t2 += getTime();

    uint8_t* pkt = (uint8_t*)calloc(packetSize, 1);
    int sentCount = 0;
    int recCount = 0;
    bool decoded = false;
    while (true) {
      sentCount++;
      int16_t ESI;
      int enc_done = ldpc_enc->getNextPacket(pkt, &ESI);
      if (rand_r(&seed) % 10000 < 10000 * dropfrac) {
        if (enc_done) {
          break;
        }
        continue;
      }
      recCount++;
      int dec_done = ldpc_dec->processPacket(pkt, ESI);
      if (dec_done == 1) {
        decoded = true;
      }
      if (dec_done || enc_done) {
        break;
      }
    }

    uint8_t* dataD = (uint8_t*)calloc(messageSize, 1);

    if (decoded) {
      ldpc_dec->getObject(dataD);
      if (memcmp(message, dataD, messageSize) == 0) {
        numSuccess++;

        printf("%d) success\n", r);
      } else {
        printf("%d) failed -didn't match\n", r);
        for (unsigned int i = 0; i < messageSize; i++) {
          if (dataD[i] != message[i]) {
            printf("data doesn't match at position %d\n", i);
            break;
          }
        }
      }
    } else {
      printf("%d) failed - couldn't decode\n", r);
    }

    delete ldpc_enc;
    delete ldpc_dec;
    free(message);
    free(pkt);
    free(dataD);
  }
  printf(
      "succeeded %.2f%% of the time with fec_rate of %f and drop Pct of %f%% "
      "\n",
      (double)numSuccess / (double)numRuns * 100.0, fec_rate, dropfrac * 100);
  printf("dec_init time=%f, enc_init = %f\n", (t2 - t1) / numRuns,
         (t1 - t0) / numRuns);
}

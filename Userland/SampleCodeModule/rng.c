/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/*                 A modified "maximally equidistributed" implementation         */
/*                 by Shin Harase, Hiroshima University.                         */
/* ***************************************************************************** */

#include <lib.h>
#include <libasm.h>
#include <rng.h>
#define W 32
#define R 624
#define DISCARD 31
#define MASKU (0xffffffffU>>(W-DISCARD))
#define MASKL (~MASKU)
#define M1 70
#define M2 179
#define M3 449

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define MAT1(v) v
#define MAT3POS(t,v) (v>>t)

#define V0            STATE[state_i]
#define VM1Over       STATE[state_i+M1-R]
#define VM1           STATE[state_i+M1]
#define VM2Over       STATE[state_i+M2-R]
#define VM2           STATE[state_i+M2]
#define VM3Over       STATE[state_i+M3-R]
#define VM3           STATE[state_i+M3]
#define VRm1          STATE[state_i-1]
#define VRm1Under     STATE[state_i+R-1]
#define VRm2          STATE[state_i-2]
#define VRm2Under     STATE[state_i+R-2]

#define newV0         STATE[state_i-1]
#define newV0Under    STATE[state_i-1+R]
#define newV1         STATE[state_i]
#define newVRm1       STATE[state_i-2]
#define newVRm1Under  STATE[state_i-2+R]

#define newVM2Over    STATE[state_i+M2-R+1]
#define newVM2        STATE[state_i+M2+1]

/*tempering paramater*/
#define BITMASK 0x41180000

static int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;
static unsigned int case_1 (void);
static unsigned int case_2 (void);
static unsigned int case_3 (void);
static unsigned int case_4 (void);
static unsigned int case_5 (void);
static unsigned int case_6 (void);
       unsigned int (*WELLRNG19937)(void);

void InitWELLRNG19937 (){

   int j;
   state_i = 0;
   WELLRNG19937 = case_1;
   for (j = 0; j < R; j++)
     STATE[j] = getTicks() + j;
}

unsigned int case_1 (void){
   // state_i == 0
   z0 = (VRm1Under & MASKL) | (VRm2Under & MASKU);
   z1 = MAT0NEG (-25, V0) ^ MAT0POS (27, VM1);
   z2 = MAT3POS (9, VM2) ^ MAT0POS (1, VM3);
   newV1      = z1 ^ z2;
   newV0Under = MAT1 (z0) ^ MAT0NEG (-9, z1) ^ MAT0NEG (-21, z2) ^ MAT0POS (21, newV1);
   state_i = R - 1;
   WELLRNG19937 = case_3;
   return (STATE[state_i] ^ (newVM2Over & BITMASK));
}

static unsigned int case_2 (void){
   // state_i == 1
   z0 = (VRm1 & MASKL) | (VRm2Under & MASKU);
   z1 = MAT0NEG (-25, V0) ^ MAT0POS (27, VM1);
   z2 = MAT3POS (9, VM2) ^ MAT0POS (1, VM3);
   newV1 = z1 ^ z2;
   newV0 = MAT1 (z0) ^ MAT0NEG (-9, z1) ^ MAT0NEG (-21, z2) ^ MAT0POS (21, newV1);
   state_i = 0;
   WELLRNG19937 = case_1;
   return (STATE[state_i] ^ (newVM2 & BITMASK));
}

static unsigned int case_3 (void){
   // state_i+M1 >= R
   z0 = (VRm1 & MASKL) | (VRm2 & MASKU);
   z1 = MAT0NEG (-25, V0) ^ MAT0POS (27, VM1Over);
   z2 = MAT3POS (9, VM2Over) ^ MAT0POS (1, VM3Over);
   newV1 = z1 ^ z2;
   newV0 = MAT1 (z0) ^ MAT0NEG (-9, z1) ^ MAT0NEG (-21, z2) ^ MAT0POS (21, newV1);
   state_i--;
   if (state_i + M1 < R)
      WELLRNG19937 = case_5;
   return (STATE[state_i] ^ (newVM2Over & BITMASK));
}

static unsigned int case_4 (void){
   // state_i+M3 >= R
   z0 = (VRm1 & MASKL) | (VRm2 & MASKU);
   z1 = MAT0NEG (-25, V0) ^ MAT0POS (27, VM1);
   z2 = MAT3POS (9, VM2) ^ MAT0POS (1, VM3Over);
   newV1 = z1 ^ z2;
   newV0 = MAT1 (z0) ^ MAT0NEG (-9, z1) ^ MAT0NEG (-21, z2) ^ MAT0POS (21, newV1);
   state_i--;
   if (state_i + M3 < R)
      WELLRNG19937 = case_6;
   return (STATE[state_i] ^ (newVM2 & BITMASK));
}

static unsigned int case_5 (void){
   // state_i+M2 >= R
   z0 = (VRm1 & MASKL) | (VRm2 & MASKU);
   z1 = MAT0NEG (-25, V0) ^ MAT0POS (27, VM1);
   z2 = MAT3POS (9, VM2Over) ^ MAT0POS (1, VM3Over);
   newV1 = z1 ^ z2;
   newV0 = MAT1 (z0) ^ MAT0NEG (-9, z1) ^ MAT0NEG (-21, z2) ^ MAT0POS (21, newV1);
   state_i--;
   if (state_i + M2 < R)
      WELLRNG19937 = case_4;
	  return (STATE[state_i] ^ (newVM2Over & BITMASK));
}

static unsigned int case_6 (void){
   // 2 <= state_i <= (R - M3 - 1)
   z0 = (VRm1 & MASKL) | (VRm2 & MASKU);
   z1 = MAT0NEG (-25, V0) ^ MAT0POS (27, VM1);
   z2 = MAT3POS (9, VM2) ^ MAT0POS (1, VM3);
   newV1 = z1 ^ z2;
   newV0 = MAT1 (z0) ^ MAT0NEG (-9, z1) ^ MAT0NEG (-21, z2) ^ MAT0POS (21, newV1);
   state_i--;
   if (state_i == 1)
      WELLRNG19937 = case_2;
   return (STATE[state_i] ^ (newVM2 & BITMASK));
}

int random(){
   return (getTicks() % 8) + 1;
}

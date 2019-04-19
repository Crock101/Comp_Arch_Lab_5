#ifndef MEMORY_HH_
#define MEMORY_HH_
#include <stdio.h>

/*
  This file contains basic cache and block structures.
  It also defines some global constants
  for use in the classes and main program.
*/

extern unsigned int clockX;
extern unsigned int numMisses;
extern int cache_org;

enum cache_org {DIRECT = 0, FULLY, TWOWAY}; // FULLY=1, TWOWAY=2

void printCacheOrg (int org); // print cache org
void resetClock(void); // set timer to zero

//=================
//==  Constants
//=================
#define WORDS_PER_BLOCK 4
#define BLOCKS_IN_MEMORY 512
#define BLOCKS_IN_CACHE 8
#define NUM_OF_SET 4
//-------------------------------------


//=================
//==  Block
//=================
class Block{
 public:
  Block()//constructor
  {
    valid = 0;
  }
  ~Block(){ }//destructor
  
  int tag;
  int data[WORDS_PER_BLOCK];
  int last_used;
  int valid;
};//class block
//-------------------------------------


//=================
//==  MainMem
//=================
class MainMem{
 public:
  Block blocks[BLOCKS_IN_MEMORY];
  MainMem(){ }  //constructor
  ~MainMem(){ }  //destructor
  int getData(int address);
  void putData(int address, int value);
};//class MainMem
//-------------------------------------


//=================
//==  Cache
//=================
class Cache {
 public:
  Block cblocks[BLOCKS_IN_CACHE];
  MainMem MainMemory;

  Cache(){ }  //constructor
  ~Cache(){ }  //destructor
  //=====================================
  int getData(int address);
  void putData( int address, int value );
};//class Cache
//-------------------------------------


class Memory
{
private:
  Cache myCache;
  
public:
  
  Memory (){}                                   // constructor
  
  int  getData ( int address )                // load
  {
    return myCache.getData(address);
  }
  void putData ( int address, int value )     // store
  {
    myCache.putData(address, value);
  }
  
  void showCacheAddress () // show the cache contents
  {
      for (int j = 0; j < BLOCKS_IN_CACHE; j++)
      {
          if (myCache.cblocks[j].valid == 1)
          {
              myCache.cblocks[j].valid = 0;

              printf("Address in block %d:\n", j);
              for (int k = 0; k < WORDS_PER_BLOCK; k++)
              {
                  if (cache_org == DIRECT)
                  {
                      //Print the reconstructed address
                      printf("Word %d: %d\n", k, (myCache.cblocks[j].tag << 5) | (j << 2) | k);
                  }
                  else if (cache_org == TWOWAY)
                  {
                      //Print the reconstructed address
                      printf("Word %d: %d\n", k, (myCache.cblocks[j].tag << 4) | ((j / 2) << 2) | k);
                  }
                  else if (cache_org == FULLY)
                  {
                      //Print the reconstructed address
                      printf("Word %d: %d\n", k, (myCache.cblocks[j].tag << 2) | k);
                  }
              }
              printf("\n");
          }
          else
          {
              printf("Block %d is not valid.\n", j);
          }
      }
  }
};

#endif

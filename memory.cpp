#include "memory.hh"

unsigned int clockX;
unsigned int numMisses;
int cache_org;

void resetClock()
{
  clockX = 0;
  numMisses = 0;
}

void printCacheOrg(int org)
{
  printf("Cache Organization: ");
  if (org == DIRECT)
    printf ("DIRECT MAPPED\n");
  else if (org == TWOWAY)
    printf ("TWO-WAY SET ASSOCIATIVE\n");
  else if (org == FULLY)
    printf ("FULLY ASSOCIATIVE\n");
  else
    printf ("ERROR: WRONG CACHE ORG\n");
}

int Cache::getData(int address)
{
  if (cache_org == DIRECT)
  {
      //The tag is the upper 27 bits of the address
      int tag = (address >> 5);

      //The index is the lower three bits of the address above the byte offset
      int index = (address >> 2) & 0x0007;

      //The byte offset is the lower two bits of the address
      int byteOffest = address & 0x0003;

      //If the block at the index has the correct tag and is valid,
      //and thus is in the cache
      if (cblocks[index].tag == tag && cblocks[index].valid == 1)
      {
          //Update the last used field of the block
          cblocks[index].last_used = clockX;

          //Increament the clock by 2
          clockX += 2;

          //Return the correct word from the block in the cache
          return cblocks[index].data[byteOffest];
      }

      //The requested block isn't in cache
      else
      {
          //Increase the number of cache misses
          numMisses++;

          //Evict the block at that address and load in the 
          //block from main memory into the cache

          //Load in all four words from memory
          cblocks[index].data[0] = MainMemory.getData((tag << 5) | (index << 2) | 0);
          cblocks[index].data[1] = MainMemory.getData((tag << 5) | (index << 2) | 1);
          cblocks[index].data[2] = MainMemory.getData((tag << 5) | (index << 2) | 2);
          cblocks[index].data[3] = MainMemory.getData((tag << 5) | (index << 2) | 3);

          //Mark the block as valid
          cblocks[index].valid = 1;

          //Update the last used field of the block
          cblocks[index].last_used = clockX;

          //Update the tag of the block
          cblocks[index].tag = tag;

          //Increament the clock by 100
          clockX += 100;
      }

      return directGetData(address);
  }
  else if (cache_org == TWOWAY)
  {
      return twoWayGetData(address);
  }
  else if (cache_org == FULLY)
  {
      return fullyGetData(address);
  }
}

void Cache::putData(int address, int value)
{
    if (cache_org == DIRECT)
    {
        //The tag is the upper 27 bits of the address
        int tag = (address >> 5);

        //The index is the lower three bits of the address above the byte offset
        int index = (address >> 2) & 0x0007;

        //The byte offset is the lower two bits of the address
        int byteOffest = address & 0x0003;

        //If the block at the index doesn't have the correct tag or isn't valid 
        if (!(cblocks[index].tag == tag && cblocks[index].valid == 1))
        {
            //Increase the number of misses
            numMisses++;
        }

        //Write the word to the cache
        cblocks[index].data[byteOffest] = value;

        //Mark the block as valid
        cblocks[index].valid = 1;

        //Update the last used field of the block
        cblocks[index].last_used = clockX;

        //Update the tag of the block
        cblocks[index].tag = tag;

        //Increament the clock by 102
        clockX += 102;

        //Write the word to the main memory
        MainMemory.putData(address, value);

    }
    else if (cache_org == TWOWAY)
    {
        twoWayPutData(address, value);
    }
    else if (cache_org == FULLY)
    {
        fullyPutData(address, value);
    }
}

int MainMem::getData(int address)
{
    //The address in memory is the upper 30 bits of the address
    int memoryAddress = (address >> 2);

    //The byte offset is the lower two bits of the address
    int byteOffest = address & 0x0003;

    //Return the correct word from the correct block in memory
    return blocks[memoryAddress].data[byteOffest];
}

void MainMem::putData(int address, int value)
{
    //The address in memory is the upper 30 bits of the address
    int memoryAddress = (address >> 2);

    //The byte offset is the lower two bits of the address
    int byteOffest = address & 0x0003;

    //Store the value in the the correct word in the correct block in memory
    blocks[memoryAddress].data[byteOffest] = value;
}



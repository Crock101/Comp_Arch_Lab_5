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

int directGetData(int address)
{

}

void directPutData(int address, int value)
{

}

int twoWayGetData(int address)
{

}

void twoWayPutData(int address, int value)
{

}

int fullyGetData(int address)
{

}

void fullyPutData(int address, int value)
{

}

int Cache::getData(int address)
{
  int data;
  // implement your code here
  return data;
}

void Cache::putData(int address, int value)
{
  // implement your code here
}

int MainMem::getData(int address)
{
    //The address in memory is the upper 30 bits of the address
    int memoryAddress = (address >> 2);

    //The byte offset is the lower two bits of the address
    int byteOffest = address & 0x0002;

    //Return the correct word from the correct block in memory
    return blocks[memoryAddress].data[byteOffest];
}

void MainMem::putData(int address, int value)
{
    //The address in memory is the upper 30 bits of the address
    int memoryAddress = (address >> 2);

    //The byte offset is the lower two bits of the address
    int byteOffest = address & 0x0002;

    //Store the value in the the correct word in the correct block in memory
    blocks[memoryAddress].data[byteOffest] = value;
}



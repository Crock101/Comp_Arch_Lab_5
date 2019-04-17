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
    int data;
    // implement your code here
    return data;
}

void MainMem::putData(int address, int value)
{
    // implement your code here
}



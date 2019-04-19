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

        //Return the correct word from the block in the cache
        return cblocks[index].data[byteOffest];
    }
    else if (cache_org == TWOWAY)
    {
        //The tag is the upper 28 bits of the address
        int tag = (address >> 4);

        //The index is the lower two bits of the address above the byte offset
        int set = (address >> 2) & 0x0003;

        //The byte offset is the lower two bits of the address
        int byteOffest = address & 0x0003;

        int  leastReacentlyUsedIndex;

        int  leastReacentlyUsedTime = clockX;

        //Search through the set for a block with an invalid bit,
        //the correct tag number, or the lowest last_used field
        for (int i = 2 * set; i < 2 * (set + 1); i++)
        {
            //If the current block is valid and has the correct sequence number
            if (cblocks[i].valid == 1 && cblocks[i].tag == tag)
            {
                //Update the last used field of the block
                cblocks[i].last_used = clockX;

                //Increament the clock by 2
                clockX += 2;

                //Return the correct word from the block
                return cblocks[i].data[byteOffest];

            }

            //If the block is invalid
            else if (cblocks[i].valid == 0)
            {
                //Increase the number of cache misses
                numMisses++;

                //Mark the block as valid
                cblocks[i].valid = 1;

                //Load in all four words from memory
                cblocks[i].data[0] = MainMemory.getData((tag << 4) | ((i / 2) << 2) | 0);
                cblocks[i].data[1] = MainMemory.getData((tag << 4) | ((i / 2) << 2) | 1);
                cblocks[i].data[2] = MainMemory.getData((tag << 4) | ((i / 2) << 2) | 2);
                cblocks[i].data[3] = MainMemory.getData((tag << 4) | ((i / 2) << 2) | 3);

                //Update the tag of the block
                cblocks[i].tag = tag;

                //Update the last used field of the block
                cblocks[i].last_used = clockX;

                //Increament the clock by 102
                clockX += 102;

                //Return the correct word from the block
                return cblocks[i].data[byteOffest];

            }

            //The block is valid with an incorrect tag,
            //and is the oldest one
            else if (cblocks[i].last_used < leastReacentlyUsedTime)
            {
                //Store the access time of the block
                leastReacentlyUsedTime = cblocks[i].last_used;

                //Store the index of the block
                leastReacentlyUsedIndex = i;
            }
        }

        //Increase the number of cache misses
        numMisses++;

        //Evict the least recently used block

        //Load in all four words from memory
        cblocks[leastReacentlyUsedIndex].data[0] = MainMemory.getData((tag << 4) | ((leastReacentlyUsedIndex / 2) << 2) | 0);
        cblocks[leastReacentlyUsedIndex].data[1] = MainMemory.getData((tag << 4) | ((leastReacentlyUsedIndex / 2) << 2) | 1);
        cblocks[leastReacentlyUsedIndex].data[2] = MainMemory.getData((tag << 4) | ((leastReacentlyUsedIndex / 2) << 2) | 2);
        cblocks[leastReacentlyUsedIndex].data[3] = MainMemory.getData((tag << 4) | ((leastReacentlyUsedIndex / 2) << 2) | 3);

        //Update the tag of the block
        cblocks[leastReacentlyUsedIndex].tag = tag;

        //Update the last used field of the block
        cblocks[leastReacentlyUsedIndex].last_used = clockX;

        //Increament the clock by 102
        clockX += 102;

        //Return the correct word from the block
        return cblocks[leastReacentlyUsedIndex].data[byteOffest];
    }
    else if (cache_org == FULLY)
    {
        //The tag is the upper 30 bits of the address
        int tag = (address >> 2);

        //The byte offset is the lower two bits of the address
        int byteOffest = address & 0x0003;

        int leastReacentlyUsedIndex;

        int leastReacentlyUsedTime = clockX;

        //Search through entire cache for a block with an invalid bit,
        //the correct tag number, or the lowest last_used field
        for (int i = 0; i < BLOCKS_IN_CACHE; i++)
        {
            //If the current block is valid and has the correct sequence number
            if (cblocks[i].valid == 1 && cblocks[i].tag == tag)
            {
                //Update the last used field of the block
                cblocks[i].last_used = clockX;

                //Increament the clock by 2
                clockX += 2;

                //Return the correct word from the block
                return cblocks[i].data[byteOffest];

            }

            //If the block is invalid
            else if (cblocks[i].valid == 0)
            {
                //Increase the number of cache misses
                numMisses++;

                //Mark the block as valid
                cblocks[i].valid = 1;

                //Load in all four words from memory
                cblocks[i].data[0] = MainMemory.getData((tag << 2) | 0);
                cblocks[i].data[1] = MainMemory.getData((tag << 2) | 1);
                cblocks[i].data[2] = MainMemory.getData((tag << 2) | 2);
                cblocks[i].data[3] = MainMemory.getData((tag << 2) | 3);

                //Update the tag of the block
                cblocks[i].tag = tag;

                //Update the last used field of the block
                cblocks[i].last_used = clockX;

                //Increament the clock by 102
                clockX += 102;

                //Return the correct word from the block
                return cblocks[i].data[byteOffest];

            }

            //The block is valid with an incorrect tag,
            //and is the oldest one
            else if (cblocks[i].last_used < leastReacentlyUsedTime)
            {
                //Store the access time of the block
                leastReacentlyUsedTime = cblocks[i].last_used;

                //Store the index of the block
                leastReacentlyUsedIndex = i;
            }
        }

        //Increase the number of cache misses
        numMisses++;

        //Evict the least recently used block

        //Load in all four words from memory
        cblocks[leastReacentlyUsedIndex].data[0] = MainMemory.getData((tag << 2) | 0);
        cblocks[leastReacentlyUsedIndex].data[1] = MainMemory.getData((tag << 2) | 1);
        cblocks[leastReacentlyUsedIndex].data[2] = MainMemory.getData((tag << 2) | 2);
        cblocks[leastReacentlyUsedIndex].data[3] = MainMemory.getData((tag << 2) | 3);

        //Update the tag of the block
        cblocks[leastReacentlyUsedIndex].tag = tag;

        //Update the last used field of the block
        cblocks[leastReacentlyUsedIndex].last_used = clockX;

        //Increament the clock by 102
        clockX += 102;

        //Return the correct word from the block
        return cblocks[leastReacentlyUsedIndex].data[byteOffest];
    }
    else
    {
        printf("ERROR: WRONG CACHE ORG\n");
        return 0;
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

            //Write the word to the main memory
            MainMemory.putData(address, value);

            //Load in all four words from memory
            cblocks[index].data[0] = MainMemory.getData((tag << 5) | (index << 2) | 0);
            cblocks[index].data[1] = MainMemory.getData((tag << 5) | (index << 2) | 1);
            cblocks[index].data[2] = MainMemory.getData((tag << 5) | (index << 2) | 2);
            cblocks[index].data[3] = MainMemory.getData((tag << 5) | (index << 2) | 3);

        }

        else
        {
            //Write the word to the cache
            cblocks[index].data[byteOffest] = value;

            //Write the word to the main memory
            MainMemory.putData(address, value);
        }

        //Mark the block as valid
        cblocks[index].valid = 1;

        //Update the last used field of the block
        cblocks[index].last_used = clockX;

        //Update the tag of the block
        cblocks[index].tag = tag;

        //Increament the clock by 102
        clockX += 102;

    }
    else if (cache_org == TWOWAY)
    {
        //The tag is the upper 28 bits of the address
        int tag = (address >> 4);

        //The index is the lower two bits of the address above the byte offset
        int set = (address >> 2) & 0x0003;

        //The byte offset is the lower two bits of the address
        int byteOffest = address & 0x0003;

        int  leastReacentlyUsedIndex;

        int  leastReacentlyUsedTime = clockX;

        //Search through the set for a block with an invalid bit,
        //the correct tag number, or the lowest last_used field
        for (int i = 2 * set; i < 2 * (set + 1); i++)
        {
            //If the current block is valid and has the correct sequence number
            if (cblocks[i].valid == 1 && cblocks[i].tag == tag)
            {
                //Write the word to the cache
                cblocks[i].data[byteOffest] = value;

                //Write the word to the main memory
                MainMemory.putData(address, value);

                //Update the last used field of the block
                cblocks[i].last_used = clockX;

                //Increament the clock by 102
                clockX += 102;

                return;
            }

            //If the block is invalid
            else if (cblocks[i].valid == 0)
            {
                //Increase the number of misses
                numMisses++;

                //Write the word to the main memory
                MainMemory.putData(address, value);

                //Mark the block as valid
                cblocks[i].valid = 1;

                //Load in all four words from memory
                cblocks[i].data[0] = MainMemory.getData((tag << 4) | (i << 2) | 0);
                cblocks[i].data[1] = MainMemory.getData((tag << 4) | (i << 2) | 1);
                cblocks[i].data[2] = MainMemory.getData((tag << 4) | (i << 2) | 2);
                cblocks[i].data[3] = MainMemory.getData((tag << 4) | (i << 2) | 3);

                //Update the tag of the block
                cblocks[i].tag = tag;

                //Update the last used field of the block
                cblocks[i].last_used = clockX;

                //Increament the clock by 102
                clockX += 102;

                return;
            }

            //The block is valid with an incorrect tag,
            //and is the oldest one
            else if (cblocks[i].last_used < leastReacentlyUsedTime)
            {
                //Store the access time of the block
                leastReacentlyUsedTime = cblocks[i].last_used;

                //Store the index of the block
                leastReacentlyUsedIndex = i;
            }
        }

        //Increase the number of misses
        numMisses++;

        //Evict the least recently used block

        //Write the word to the main memory
        MainMemory.putData(address, value);

        //Load in all four words from memory
        cblocks[leastReacentlyUsedIndex].data[0] = MainMemory.getData((tag << 4) | (leastReacentlyUsedIndex << 2) | 0);
        cblocks[leastReacentlyUsedIndex].data[1] = MainMemory.getData((tag << 4) | (leastReacentlyUsedIndex << 2) | 1);
        cblocks[leastReacentlyUsedIndex].data[2] = MainMemory.getData((tag << 4) | (leastReacentlyUsedIndex << 2) | 2);
        cblocks[leastReacentlyUsedIndex].data[3] = MainMemory.getData((tag << 4) | (leastReacentlyUsedIndex << 2) | 3);

        //Update the tag of the block
        cblocks[leastReacentlyUsedIndex].tag = tag;

        //Update the last used field of the block
        cblocks[leastReacentlyUsedIndex].last_used = clockX;

        //Increament the clock by 102
        clockX += 102;
    }
    else if (cache_org == FULLY)
    {
    //The tag is the upper 30 bits of the address
    int tag = (address >> 2);

    //The byte offset is the lower two bits of the address
    int byteOffest = address & 0x0003;

    int  leastReacentlyUsedIndex;

    int  leastReacentlyUsedTime = clockX;

    //Search through entire cache for a block with an invalid bit,
    //the correct tag number, or the lowest last_used field
    for (int i = 0; i < BLOCKS_IN_CACHE; i++)
    {
        //If the current block is valid and has the correct sequence number
        if (cblocks[i].valid == 1 && cblocks[i].tag == tag)
        {
            //Write the word to the cache
            cblocks[i].data[byteOffest] = value;

            //Write the word to the main memory
            MainMemory.putData(address, value);

            //Update the last used field of the block
            cblocks[i].last_used = clockX;

            //Increament the clock by 102
            clockX += 102;

            return;
        }

        //If the block is invalid
        else if (cblocks[i].valid == 0)
        {
            //Increase the number of misses
            numMisses++;

            //Write the word to the main memory
            MainMemory.putData(address, value);

            //Mark the block as valid
            cblocks[i].valid = 1;

            //Load in all four words from memory
            cblocks[i].data[0] = MainMemory.getData((tag << 2) | 0);
            cblocks[i].data[1] = MainMemory.getData((tag << 2) | 1);
            cblocks[i].data[2] = MainMemory.getData((tag << 2) | 2);
            cblocks[i].data[3] = MainMemory.getData((tag << 2) | 3);

            //Update the tag of the block
            cblocks[i].tag = tag;

            //Update the last used field of the block
            cblocks[i].last_used = clockX;

            //Increament the clock by 102
            clockX += 102;

            return;
        }

        //The block is valid with an incorrect tag,
        //and is the oldest one thus far
        else if (cblocks[i].last_used < leastReacentlyUsedTime)
        {
            //Store the access time of the block
            leastReacentlyUsedTime = cblocks[i].last_used;

            //Store the index of the block
            leastReacentlyUsedIndex = i;
        }
    }

    //Increase the number of misses
    numMisses++;

    //Evict the least recently used block

    //Write the word to the main memory
    MainMemory.putData(address, value);

    //Load in all four words from memory
    cblocks[leastReacentlyUsedIndex].data[0] = MainMemory.getData((tag << 2) | 0);
    cblocks[leastReacentlyUsedIndex].data[1] = MainMemory.getData((tag << 2) | 1);
    cblocks[leastReacentlyUsedIndex].data[2] = MainMemory.getData((tag << 2) | 2);
    cblocks[leastReacentlyUsedIndex].data[3] = MainMemory.getData((tag << 2) | 3);

    //Update the tag of the block
    cblocks[leastReacentlyUsedIndex].tag = tag;

    //Update the last used field of the block
    cblocks[leastReacentlyUsedIndex].last_used = clockX;

    //Increament the clock by 102
    clockX += 102;
    }
    else
    {
        printf("ERROR: WRONG CACHE ORG\n");
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



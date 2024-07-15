//Program by Jack Wilcox

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct Block Block;

struct Block
{
    bool validBit;
    int tag;
    int numHits;
    int lastReference;
};

int main()
{
    const int QUIT = -1;

    int S = 0;
    int s = 0;
    int E = 0;
    int B = 0;
    int b = 0;
    int m = 0;

    char repPolicy[3];
    int hitTime = 0;
    int penalty = 0;

    unsigned int addressInput = 0;
    unsigned int* curAddress = NULL;
    unsigned int addressList[50];
    int numAddresses = 0;

    int tag     = 0;
    int setLine = 0;

    int numLines = 0;
    int numSets  = 0;

    double numMisses     = 0;
    int missRate         = 0;
    double numReferences = 0;
    int numCycles        = 0;

    /**************************************************************************************************************************************************/

    printf("Input the Cache Parameters:\n");

    printf("S: ");
    scanf("%d", &S);

    s = (int) (log2(S));

    printf("E: ");
    scanf("%d", &E);

    printf("B: ");
    scanf("%d", &B);

    b = (int) (log2(B));

    printf("m: ");
    scanf("%d", &m);

    Block cache[S][E];
    numSets  = S;
    numLines = E;

    for(int i = 0; i < numSets; i++)
    {
        for(int j = 0; j < numLines; j++)
        {
            cache[i][j].validBit = false;
            cache[i][j].numHits = 0;
            cache[i][j].lastReference = 0;
        }
    }

    printf("Replacement Policy: ");
    scanf("%s", repPolicy);

    printf("Hit Time: ");
    scanf("%d", &hitTime);

    printf("Penalty: ");
    scanf("%d", &penalty);

    while(addressInput != QUIT)
    {
        scanf("%x", &addressInput);

        addressList[numAddresses] = addressInput;
        numAddresses++;
    }

    for(int i = 0; i < numAddresses; i++)
    {
        curAddress = &addressList[i];

        if(*curAddress == QUIT)
        {
            missRate = (int) round((numMisses / numReferences) * 100);

            printf("%d %d\n", missRate, numCycles);

            break;
        }

        tag     = (int) (*curAddress >> (s + b));
        setLine = (int) ((*curAddress >> b) & ((1 << s) - 1));

        numCycles += hitTime;
        numReferences++;

        for(int j = 0; j < numLines; j++)
        {
            if(cache[setLine][j].validBit == true)
            {
                if(cache[setLine][j].tag == tag)
                {
                    cache[setLine][j].lastReference = numReferences;

                    printf("%x H\n", *curAddress);

                    cache[setLine][j].numHits++;

                    break;
                }

                else
                {
                    if(numLines != 1)
                    {
                        if(j == numLines - 1)
                        {
                            if(strcmp(repPolicy, "LFU") == 0)
                            {
                                int leastFrequentHits = 0;
                                int lfuSetIndex = 0;
                                int lfuLineIndex = 0;

                                for(int k = 0; k < numLines; k++)
                                {
                                    if(k == 0)
                                    {
                                        leastFrequentHits = cache[setLine][k].numHits;
                                    }

                                    if(cache[setLine][k].numHits <= leastFrequentHits)
                                    {
                                        lfuLineIndex = k;
                                        lfuSetIndex = setLine;
                                    }
                                }

                                cache[lfuSetIndex][lfuLineIndex].tag = tag;
                                cache[lfuSetIndex][lfuLineIndex].numHits = 0;
                            }

                            else if(strcmp(repPolicy, "LRU") == 0)
                            {
                                int leastRecentRefs = 0;
                                int lruSetIndex = 0;
                                int lruLineIndex = 0;

                                for(int k = 0; k < numLines; k++)
                                {
                                    if(k == 0)
                                    {
                                        leastRecentRefs = cache[setLine][k].lastReference;
                                    }

                                    if(cache[setLine][k].lastReference <= leastRecentRefs)
                                    {
                                        lruLineIndex = k;
                                        lruSetIndex = setLine;
                                    }
                                }

                                cache[lruSetIndex][lruLineIndex].tag = tag;
                                cache[lruSetIndex][lruLineIndex].numHits = 0;
                                cache[lruSetIndex][lruLineIndex].lastReference = numReferences;
                            }

                            printf("%x M\n", *curAddress);

                            numMisses++;
                            numCycles += penalty;

                            break;
                        }
                    }

                    else
                    {
                        cache[setLine][j].tag = tag;
                        cache[setLine][j].lastReference = numReferences;

                        printf("%x M\n", *curAddress);

                        numMisses++;
                        numCycles += penalty;

                        break;
                    }
                }
            }

            else
            {
                cache[setLine][j].validBit = true;
                cache[setLine][j].tag = tag;
                cache[setLine][j].lastReference = numReferences;

                printf("%x M\n", *curAddress);

                numMisses++;
                numCycles += penalty;

                break;
            }
        }
    }

    return 0;
}

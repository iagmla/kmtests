#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include "stats.c"
#include "tests.c"

/* kmtests v0.0.1 */
/* by Karl Zander */

void usage() {
    printf("kmtests v0.0.1\n");
    printf("Usage: kmtests <input file>\n");
}

int main(int arc, char *argv[]) {
    if (arc != 2) {
        usage();
        return 0;
    }
    char *inFilename;
    inFilename = argv[1];
    if (access(inFilename, F_OK) == -1 ) {
        printf("%s not found\n", inFilename);
        exit(1);
    }

    struct stats s;
    s.inFilename = inFilename;
    statsInit(&s);
    calcStats(&s, inFilename);
    calcFractions(&s);
    calcProbabilities(&s);
    calcIC(&s);
    calcEntropy(&s);
    calcAverage(&s);
    calcChiSQ(&s);
    calcSerialCorrelation(&s);
    printStats(&s);
 
    struct tests t;
    testsInit(&t);
    runBiGraph(&t, inFilename);
    runTriGraph(&t, inFilename);
    runSums(&t, inFilename);
    runPeriod(&t, inFilename);
    return 0;
}

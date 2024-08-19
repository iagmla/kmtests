uint8_t bigraphs[65536][2];
uint8_t trigraphs[16777216][3];
int bigraphOccurrences[65536];
int trigraphOccurrences[16777216];
int bigraphResults[65536];
int trigraphResults[16777216];

struct tests {
    int period;
    int sums;
    uint32_t avgSums;
    uint32_t datalen;
};

void testsInit(struct tests *t) {
    memset(bigraphResults, 0, 65536*sizeof(int));
    memset(bigraphOccurrences, 0, 65536*sizeof(int));
    memset(trigraphResults, 0, 16777216*sizeof(int));
    memset(trigraphOccurrences, 0, 16777216*sizeof(int));
};

void genBiGraphs(struct tests *t) {
    uint8_t bi[2] = {0};
    int tmp[2] = {0};
    int c = 0;
    int p = 0;
    int l;
    int s = 0;
    for (int i = 0; i < 65536; i++) {
        if ((c % 256) == 0) {
            p = (p - 1) % 2;
            if (p < 0) {
                p += 2;
            }
        }
        tmp[0] = s;
        tmp[1] = c;
        bi[0] = tmp[0];
        bi[1] = tmp[1];
        bigraphs[i][0] = bi[0];
        bigraphs[i][1] = bi[1];
        c = (c + 1) % 256;
        if (c < 0) {
            c += 256;
        }
        if ((c % 256) == 0) {
            s = (s + 1) % 256;
        }
    }
}

int getBiGraphIndex(struct tests *t, uint8_t *bi) {
    int i = 0;
    for (i = 0; i < 65536; i++) {
        if (memcmp(bigraphs[i], bi, 2*sizeof(uint8_t)) == 0) {
            break;
        }
    }
    return i;
}

void testBiGraph(struct tests *t, char *inFilename) {
    FILE *infile;
    infile = fopen(inFilename, "rb");
    fseek(infile, 0, SEEK_END);
    uint32_t datalen = ftell(infile);
    t->datalen = datalen;
    fseek(infile, 0, SEEK_SET);
    uint8_t bi[2];
    int index;
    uint32_t blocks = datalen / 2;
    for (int i = 0; i < blocks; i++) {
        fread(bi, 1, 2, infile);
        index = getBiGraphIndex(t, bi);
        bigraphOccurrences[index] += 1;
    }
    fclose(infile);
}

void printBiGraph(struct tests *t) {
    uint8_t bi[2] = {0};
    for (int i = 0; i < 65536; i++) {
        bi[0] = bigraphs[i][0];
        bi[1] = bigraphs[i][1];
        if (bigraphResults[i] == 1) {
            printf("Bi Graph {%d, %d} - %d FAIL ------\n", bi[0], bi[1], bigraphOccurrences[i]);
        }
        else {
            printf("Bi Graph {%d, %d} - %d PASS ------\n", bi[0], bi[1], bigraphOccurrences[i]);
        }
    }
}

void resultsBiGraph(struct tests *t) {
    int expected_max = ((t->datalen + (65536 * 16)) / 65536 / 4) * 2;
    int expected_min = ((t->datalen + (65536 * 16)) / 4 / ((65536 / 16) + 65536)) / 2;
    printf("bi min %d\n", expected_min);
    printf("bi max %d\n", expected_max);
    for (int i = 0; i < 65536; i++) {
        if ((bigraphOccurrences[i] >= expected_min) && (bigraphOccurrences[i] <= expected_max)) {
            bigraphResults[i] = 0;
        }
        else {
            bigraphResults[i] = 1;
        }
    }   
}

void runBiGraph(struct tests *t, char *inFilename) {
    genBiGraphs(t);
    testBiGraph(t, inFilename);
    resultsBiGraph(t);
    printBiGraph(t);
}

void genTriGraphs(struct tests *t) {
    uint8_t tri[3] = {0};
    int tmp[3] = {0};
    int c = 0;
    int p = 0;
    int l;
    int s = 0;
    int v = 0;
    for (int i = 0; i < 16777216; i++) {
        if (((i % 65536) == 0) && (i != 0))  {
            v = (v + 1) % 256;
            if (v < 0) {
                v += 256;
            }
        }
        tmp[0] = v;
        tmp[1] = s;
        tmp[2] = c;
        tri[0] = tmp[0];
        tri[1] = tmp[1];
        tri[2] = tmp[2];
        trigraphs[i][0] = tri[0];
        trigraphs[i][1] = tri[1];
        trigraphs[i][2] = tri[2];
        c = (c + 1) % 256;
        if (c < 0) {
            c += 256;
        }
        if ((c % 256) == 0) {
            s = (s + 1) % 256;
            if (s < 0) {
                s += 256;
            }
        }
    }
}

int getTriGraphIndex(struct tests *t, uint8_t *tri) {
    int i = 0;
    for (i = 0; i < 16777216; i++) {
        if (memcmp(trigraphs[i], tri, 3*sizeof(uint8_t)) == 0) {
            break;
        }
    }
    return i;
}

void testTriGraph(struct tests *t, char *inFilename) {
    FILE *infile;
    infile = fopen(inFilename, "rb");
    fseek(infile, 0, SEEK_END);
    uint32_t datalen = ftell(infile);
    t->datalen = datalen;
    fseek(infile, 0, SEEK_SET);
    uint8_t tri[3];
    int index;
    uint32_t blocks = datalen / 3;
    for (int i = 0; i < blocks; i++) {
        fread(tri, 1, 3, infile);
        index = getTriGraphIndex(t, tri);
        trigraphOccurrences[index] += 1;
    }
    fclose(infile);
}

void printTriGraphs(struct tests *t) {
    uint8_t tri[3] = {0};
    for (int i = 0; i < 16777216; i++) {
        tri[0] = trigraphs[i][0];
        tri[1] = trigraphs[i][1];
        tri[2] = trigraphs[i][2];
        if (trigraphResults[i] == 1) {
            printf("Tri Graph {%d, %d, %d} - %d FAIL ------\n", tri[0], tri[1], tri[2], trigraphOccurrences[i]);
        }
        else {
            printf("Tri Graph {%d, %d, %d} - %d PASS ------\n", tri[0], tri[1], tri[2], trigraphOccurrences[i]);
        }
    }
}

void resultsTriGraph(struct tests *t) {
    int expected_max = (t->datalen + (16777216 * 256 * 2)) / 16777216 / 3;
    int expected_min = (t->datalen + (16777216 * 1 * 1)) / 3 / ((16777216 / 2) + 16777216);
    printf("tri min %d\n", expected_min);
    printf("tri max %d\n", expected_max);
    for (int i = 0; i < 65536; i++) {
        if ((trigraphOccurrences[i] >= expected_min) && (trigraphOccurrences[i] <= expected_max)) {
            trigraphResults[i] = 0;
        }
        else {
            trigraphResults[i] = 1;
        }
    }
}

void runTriGraph(struct tests *t, char *inFilename) {
    genTriGraphs(t);
    testTriGraph(t, inFilename);
    resultsTriGraph(t);
    printTriGraphs(t);
}

void testPeriod(struct tests *t, char *inFilename, int runLength) {
    FILE *infile;
    infile = fopen(inFilename, "rb");
    fseek(infile, 0, SEEK_END);
    uint32_t datalen = ftell(infile);
    t->datalen = datalen;
    fseek(infile, 0, SEEK_SET);
    uint32_t runs = datalen / runLength;
    uint32_t offset = 0;
    for (uint32_t x = 0; x < runs; x++) {
        fseek(infile, offset, 0);
        uint8_t run[runLength];
        uint8_t sample[runLength];
        fread(run, 1, runLength, infile);
        for (uint32_t y = 0; y < runs; y++) {
            fread(sample, 1, runLength, infile);
            if (memcmp(run, sample, runLength*sizeof(uint8_t)) == 0) {
                t->period += 1;
            }
        }
        offset += runLength;
        runs -= runLength;
    }
    fclose(infile);
}

void printPeriod(struct tests *t) {
    int threshold = sqrt(sqrt(sqrt(t->datalen)));
    if (t->period <= threshold) {
        printf("Period %d PASS ------\n", t->period);
    }
    else {
        printf("Period %d FAIL ------\n", t->period);
    }
}

void runPeriod(struct tests *t, char *inFilename) {
    int maxRun = 13;
    int minRun = 8;
    for (int i = maxRun; i != minRun - 1; i--) {
        testPeriod(t, inFilename, i);
    }
    printPeriod(t);
}

void testSums(int *sums, char *inFilename, int blockLength) {
    FILE *infile;
    infile = fopen(inFilename, "rb");
    fseek(infile, 0, SEEK_END);
    uint32_t datalen = ftell(infile);
    fseek(infile, 0, SEEK_SET);
    uint8_t block[256];
    int index;
    uint32_t blocks = datalen / blockLength;
    for (int i = 0; i < blocks; i++) {
        fread(block, 1, blockLength, infile);
        int blockSum = 0;
        for (int x = 0; x < blockLength; x++) {
            blockSum += block[x];
        }
        sums[i] = blockSum;
    }
    fclose(infile);
}

void printSums(struct tests *t) {
    int sqrtSums = sqrt(sqrt(t->datalen));
    int threshold = sqrtSums + 256;
    if (t->sums <= threshold) {
        printf("Sums %d PASS ------\n", t->sums);
    }
    else {
        printf("Sums %d FAIL ------\n", t->sums);
    }
}

void printavgSums(struct tests *t) {
    int max = ((256 * 256) / 2) - 13;
    int min = (((256 * 256) / 2) - 13) - 5;
    if ((t->avgSums <= max) || (t->avgSums >= min)) {
        printf("AvgSums %d PASS ------\n", t->avgSums);
    }
    else {
        printf("AvgSums %d FAIL ------\n", t->avgSums);
    }
}

void runSums(struct tests *t, char *inFilename) {
    int blockLength = 256;
    int max = ((256 * 256) / 2) + (256 * 16);
    int min = (((256 * 256) / 2) - (256 * 16));
    int sums[t->datalen / blockLength];
    t->sums = 0;
    t->avgSums = 0;
    testSums(sums, inFilename, blockLength);
    for (int i = 0; i < t->datalen / blockLength; i++) {
        if ((sums[i] > max) || (sums[i] < min)) {
            t->sums += 1;
        }
        t->avgSums += sums[i];
    }
    t->avgSums = t->avgSums / (t->datalen / blockLength);
    printSums(t);
    printavgSums(t);
}

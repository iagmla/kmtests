/* kmstats */
/* by Karl Zander */

struct stats {
    int occurrences[256];
    double fractions[256];
    double probabilities[256];
    double means[256];
    double ic;
    double entropy;
    double avg;
    double serialCorrelation;
    double chiSQ;
    uint32_t datalen;
    uint32_t sum;
    uint32_t totalCount;
    char *inFilename;
};

void statsInit(struct stats *s) {
    memset(s->occurrences, 0, 256 * sizeof(int));
    memset(s->means, 0, 256 * sizeof(double));
    s->ic = 0.0;
    s->avg = 0.0;
    s->entropy = 0.0;
    s->chiSQ = 0.0;
    s->totalCount = 0;
    s->serialCorrelation = 0.0;
}

void calcStats(struct stats *s, char *inFilename) {
    FILE *infile;
    infile = fopen(inFilename, "rb");
    fseek(infile, 0, SEEK_END);
    uint32_t datalen = ftell(infile);
    s->datalen = datalen;
    fseek(infile, 0, SEEK_SET);
    uint8_t data;
    for (int i = 0; i < datalen; i++) {
        fread(&data, 1, 1, infile);
        s->occurrences[data] += 1;
    }
    fclose(infile);
}

void printStats(struct stats *s) {
    printf("Value  Occurrences  Fractions  Probability\n");
    for (int i = 0; i < 256; i++) {
        printf("%3d    %10d  %7f   %7f\n", i, s->occurrences[i], s->fractions[i], s->probabilities[i]);
    }
    printf("Entropy %f\n", s->entropy);
    printf("Average %f\n", s->avg);
    printf("IC %f\n", s->ic);
    printf("Serial Correlation %f\n", s->serialCorrelation);
    printf("Chi-Squared Distribution %f\n", s->chiSQ);
}

void calcIC(struct stats *s) {
    float p = 0.0;
    for (int i = 0; i < 256; i++) {
        p += ((s->occurrences[i] / (s->datalen / 256.0)) * (s->occurrences[i] - 1) / ((s->datalen - 1) / 256.0));
    }
    s->ic = p;
}

void calcEntropy(struct stats *s) {
    for (int i = 0; i < 256; i++) {
        if (s->occurrences[i] != 0.0) {
            s->entropy += ((double)s->occurrences[i] / s->datalen) * log2(1 / ((double)s->occurrences[i] / s->datalen));
        }
    }
}

void calcAverage(struct stats *s) {
    for (int i = 0; i < 256; i++) {
        s->avg += s->occurrences[i] * i;
        s->totalCount += s->occurrences[i];
    }
    s->sum = s->avg;
    s->avg = s->avg / s->datalen;
}

void calcFractions(struct stats *s) {
    for (int i = 0; i < 256; i++) {
        s->fractions[i] = (double)((double)s->occurrences[i] / (double)s->datalen);
    }
}

void calcProbabilities(struct stats *s) {
    for (int i = 0; i < 256; i++) {
        s->probabilities[i] = s->fractions[i] * 256.0;
    }
}

void calcChiSQ(struct stats *s) {
    double p;
    double expectedCount = s->totalCount / 256.0;
    s->means[0] = 0.0;
    if (s->occurrences[1] != 0) {
        s->means[1] = s->occurrences[1];
    }
    for (int i = 2; i < 256; i++) {
        if (s->occurrences[i] != 0) {
            s->means[i] = s->sum / (s->occurrences[i] * i);
        }
    }
    for (int i = 0; i < 256; i++) {
        p = (((double)s->occurrences[i] - (double)s->means[i])) / expectedCount;
        s->chiSQ += (double)(p * p);
    }
}

void calcSerialCorrelation(struct stats *s) {
    double p;
    double expectedCount = ((double)s->datalen) / 256.0;
    for (int i = 0; i < 256; i++) {
        p = (((float)expectedCount - (float)s->occurrences[i]));
        if (p < 0.0) {
            p *= -1.0;
        }
        s->serialCorrelation += p;
    }
    s->serialCorrelation = s->serialCorrelation / s->totalCount / s->datalen;
    s->serialCorrelation = sqrt(s->serialCorrelation);
}

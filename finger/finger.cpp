/*
 * create by sivan.yue
 * data : 2017-05-27
 */

#include "../CAFP/CAFP.h"

#include <stdio.h>
#include <unistd.h>

void correctionData(unsigned *data, int len) {
    if(len <= 0 || data == NULL) {
        return;
    }
    int i;
    for(i = 0; i < len; ++i) {
        data[i] = data[i] << 1;
    }
}

int finger(char *pcmPath, char *afpPath) {
    int ret, mode = 2;
    int fftSize = 4096;
    double fs = 8000.0;
    double stepTime = 11.6 / 1000.0;
    double dataTime = 0.512;
    int streamBytes, fileBytes;
    unsigned char *ppcm;
    unsigned int *afpBuf;
    CAFP cafp;
    FILE *pcmFp = fopen(pcmPath, "rb");
    FILE *afpFp = fopen(afpPath, "wb");
    if (!pcmFp || !afpFp) {
        return -1;
    }
    
    ret = cafp.setParm(fs, stepTime, dataTime, fftSize, mode);
    printf("init CAFP result: %d\n", ret);

    fseek(pcmFp, 0, SEEK_END);
    fileBytes = ftell(pcmFp);
    fseek(pcmFp, 0, SEEK_SET);
    ppcm = new unsigned char[fileBytes];
    streamBytes = fread(ppcm, 1, fileBytes, pcmFp);
    fclose(pcmFp);
    if (streamBytes != fileBytes) {
        return -1;
    }

    int pcmSize = fileBytes / 2;
    short *spcm = (short *)ppcm;
    int realStep, step = pcmSize;
    int preCalAfpNum = cafp.calcuteNextSubFingerSize(pcmSize);
    afpBuf = new unsigned[preCalAfpNum + 16];
    int realAfpNum = cafp.getSubFingerprinting(spcm, pcmSize, (void*)afpBuf, preCalAfpNum+16);
    correctionData(afpBuf, realAfpNum);
    fwrite((void *)afpBuf, 1, realAfpNum*sizeof(unsigned), afpFp);
    fclose(afpFp);
    delete []ppcm;
    delete []afpBuf;
    return 0;
}

int main(int argc, char **argv) {
    char *orgFile = 0;
    char *dstFile = 0;
    int ch = -1;
    while (-1 != (ch=getopt(argc, argv, "o:d:"))) {
        switch(ch) {
        case 'o': {
            orgFile = optarg;
            break;
        }
        case 'd': {
            dstFile = optarg;
            break;
        }
        default: {
            break;
        }
        }
    }

    finger(orgFile, dstFile);
    return 0;
}

#include "pcap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pcapParserInit(PCAPParser *pParser) {
  memset(pParser, 0, sizeof(PCAPParser));
}

void pcapParserCleanup(PCAPParser *pParser) {
  if (pParser->bCleanMemory) {
    free(pParser->pPcap);
    memset(pParser, 0, sizeof(PCAPParser));
  }
}

int pcapParse(PCAPParser *pParser, uint8_t *pBuff, size_t nLen) {
  if (nLen < 24) {
    return -1;
  }

  pParser->pPcap = (PCAP *)pBuff;

  return 0;
}

int pcapLoadFromFile(PCAPParser *pParser, const char *sFilePath, size_t pLen) {
  uint8_t *pBuff = NULL;
  int nLen = 0;

  FILE *fd = fopen(sFilePath, "rb");
  if (!fd) {
    return -1;
  }

  fseek(fd, 0, SEEK_END);
  nLen = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  pBuff = malloc(nLen + 1);
  if (!pBuff) {
    return -1;
  }

  fread(pBuff, nLen, 1, fd);
  fclose(fd);

  return pcapParse(pParser, pBuff, nLen);
}

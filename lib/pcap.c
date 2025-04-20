#include "pcap.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pcapParserInit(PCAPParser *pParser) {
  memset(pParser, 0, sizeof(PCAPParser));
}

void pcapParserCleanup(PCAPParser *pParser) {
  if (pParser->bCleanMemory) {
    free(pParser->pPcapHeader);
    memset(pParser, 0, sizeof(PCAPParser));
  }
}

int pcapParse(PCAPParser *pParser, uint8_t *pBuff, size_t nLen) {
  if (nLen < 24) {
    return -1;
  }

  pParser->pPcapHeader = (PCAPHeader *)pBuff;
  pParser->pBuffer = pBuff;
  pParser->nLen = nLen;

  pcapRewind(pParser);

  return 0;
}

int pcapLoadFromFile(PCAPParser *pParser, const char *sFilePath) {
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

bool pcapNextPacket(PCAPParser *pParser, PCAPPacketHeader **pHeader,
                    uint8_t **pData) {
  if (pParser->nOffset + 16 >= pParser->nLen) {
    return false;
  }

  PCAPPacketHeader *pPacketHeader =
      (PCAPPacketHeader *)(pParser->pBuffer + pParser->nOffset);
  pParser->nOffset += 16;

  if (pHeader) {
    *pHeader = pPacketHeader;
  }
  if (pData) {
    *pData = (pParser->pBuffer + pParser->nOffset);
  }
  pParser->nOffset += pPacketHeader->nCapturedLen;

  return true;
}

void pcapRewind(PCAPParser *pParser) { pParser->nOffset = 24; }

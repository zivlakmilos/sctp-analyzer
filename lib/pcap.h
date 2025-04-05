#ifndef _PCAP_H_
#define _PCAP_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  uint32_t nMagicNumber;
  uint16_t nVersionMajor;
  uint16_t nVersionMinor;
  uint32_t nReserved1;
  uint32_t nReserved2;
  uint32_t nSnapLen;
  uint16_t nFlags;
  uint16_t nLinkType;
  uint8_t *pData;
} __attribute__((packed)) PCAP;

typedef struct {
  PCAP *pPcap;
  bool bCleanMemory;
} PCAPParser;

void pcapParserInit(PCAPParser *pParser);
void pcapParserCleanup(PCAPParser *pParser);

int pcapParse(PCAPParser *pParser, uint8_t *pBuff, size_t nLen);
int pcapLoadFromFile(PCAPParser *pParser, const char *sFilePath, size_t pLen);

#endif // !_PCAP_H_

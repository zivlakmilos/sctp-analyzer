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
} __attribute__((packed)) PCAPHeader;

typedef struct {
  uint32_t nTimestamp;
  uint32_t nTimestamp2;
  uint32_t nCapturedLen;
  uint32_t nOriginalLen;
} __attribute__((packed)) PCAPPacketHeader;

typedef struct {
  PCAPHeader *pPcapHeader;
  bool bCleanMemory;
  uint8_t *pBuffer;
  size_t nOffset;
  size_t nLen;
} PCAPParser;

void pcapParserInit(PCAPParser *pParser);
void pcapParserCleanup(PCAPParser *pParser);

int pcapParse(PCAPParser *pParser, uint8_t *pBuff, size_t nLen);
int pcapLoadFromFile(PCAPParser *pParser, const char *sFilePath);

bool pcapNextPacket(PCAPParser *pParser, PCAPPacketHeader **pHeader,
                    uint8_t **pData);
void pcapRewind(PCAPParser *pParser);

#endif // !_PCAP_H_

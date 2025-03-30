#include "app.h"

#include <assert.h>
#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SApp {
  int nArgc;
  char **psArgv;
  char *sPcapPath;
};

static void _parseArgs(App *pApp);

App *appNew(int nArgc, char **psArgv) {
  App *pApp = malloc(sizeof(App));
  memset(pApp, 0, sizeof(App));

  pApp->nArgc = nArgc;
  pApp->psArgv = psArgv;

  return pApp;
}

void appDelete(App **ppApp) {
  if (!ppApp || !*ppApp) {
    return;
  }

  free(*ppApp);
  *ppApp = NULL;
}

void appRun(App *pApp) {
  initscr();
  cbreak();
  raw();
  noecho();

  _parseArgs(pApp);
  getch(); // TODO: Implement main loop instead

  endwin();
}

void appExitWithError(App *pApp, char *fmt, ...) {
  endwin();

  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);

  exit(1);
}

static void _parseArgs(App *pApp) {
  if (pApp->nArgc < 2) {
    appExitWithError(pApp, "%s requires path to pcap file as argument\n",
                     pApp->psArgv[0]);
  }

  pApp->sPcapPath = pApp->psArgv[1];
}

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

  int nWidth;
  int nHeight;
};

static void _parseArgs(App *pApp);
static void _readNcursesState(App *pApp);
static void _handleInput(App *pApp);
static void _render(App *pApp);
static void _renderStatusBar(App *pApp);

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

  keypad(stdscr, true);

  _parseArgs(pApp);

  _readNcursesState(pApp);
  _renderStatusBar(pApp);

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

static void _readNcursesState(App *pApp) {
  getmaxyx(stdscr, pApp->nHeight, pApp->nWidth);
}

static void _handleInput(App *pApp) {}

static void _render(App *pApp) {}

static void _renderStatusBar(App *pApp) {
  int y = pApp->nHeight - 2;
  int x = 0;

  attron(WA_REVERSE);
  mvprintw(y, x, "%-*s", pApp->nWidth - 1, pApp->sPcapPath);
  attroff(WA_REVERSE);
}

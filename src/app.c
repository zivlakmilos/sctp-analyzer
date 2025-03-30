#include "app.h"
#include "utils.h"

#include <assert.h>
#include <ctype.h>
#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
  STATE_NORMAL = 0,
  STATE_COMMAND,
};

struct SApp {
  int nArgc;
  char **psArgv;
  char *sPcapPath;

  int nWidth;
  int nHeight;

  int nState;

  char sCommand[1024];
  int nCommandLen;
};

static void _parseArgs(App *pApp);
static void _readNcursesState(App *pApp);
static void _render(App *pApp);
static void _renderStatusBar(App *pApp);
static void _renderCommandBar(App *pApp);

static void _handleInput(App *pApp);
static int _handleInputCommand(App *pApp, int ch);

static stateInputHandler _stateInputHandlers[] = {
    NULL,
    _handleInputCommand,
};

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

  while (1) {
    _readNcursesState(pApp);
    _render(pApp);
    _handleInput(pApp);
  }

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

static void _render(App *pApp) {
  _renderStatusBar(pApp);
  _renderCommandBar(pApp);
}

static void _renderStatusBar(App *pApp) {
  int y = pApp->nHeight - 2;
  int x = 0;

  attron(WA_REVERSE);
  mvprintw(y, x, "%-*s", pApp->nWidth - 1, pApp->sPcapPath);
  attroff(WA_REVERSE);
}

static void _renderCommandBar(App *pApp) {
  int y = pApp->nHeight - 1;
  int x = 0;

  if (pApp->nState != STATE_COMMAND) {
    mvprintw(y, x, "%-*s", pApp->nWidth - 1, "");
    return;
  }

  mvprintw(y, x, ":%-*s", pApp->nWidth - 1, pApp->sCommand);
}

static void _handleInput(App *pApp) {
  int ch = getch();

  if (ch == ':' && pApp->nState != STATE_COMMAND) {
    pApp->nState = STATE_COMMAND;
    pApp->sCommand[0] = '\0';
    pApp->nCommandLen = 0;
    return;
  }

  if (!_stateInputHandlers[pApp->nState]) {
    return;
  }

  _stateInputHandlers[pApp->nState](pApp, ch);
}

static int _handleInputCommand(App *pApp, int ch) {
  switch (ch) {
  case KBD_ESC:
    pApp->nState = STATE_NORMAL;
    return 0;
  case KBD_BACKSPACE:
    pApp->nCommandLen--;
    pApp->sCommand[pApp->nCommandLen] = '\0';
    return 0;
  case KBD_ENTER:
    if (strcmp(pApp->sCommand, "q") == 0) {
      endwin();
      exit(0);
    }
    pApp->nCommandLen--;
    pApp->sCommand[pApp->nCommandLen] = '\0';
    pApp->nState = STATE_NORMAL;
    return 0;
  }

  if (!isalnum(ch)) {
    return 0;
  }

  pApp->sCommand[pApp->nCommandLen] = (char)ch;
  pApp->nCommandLen++;
  pApp->sCommand[pApp->nCommandLen] = '\0';

  return 0;
}

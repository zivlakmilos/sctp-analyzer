#include "app.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

struct SApp {
  int nArgc;
  char **psArgv;
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

void appRun(App *app) {
  initscr();
  cbreak();
  raw();
  noecho();

  getch(); // TODO: Implement main loop instead

  endwin();
}

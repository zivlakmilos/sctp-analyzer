#include "app.h"

int main(int argc, char *argv[]) {
  App *pApp = appNew(argc, argv);

  appRun(pApp);

  appDelete(&pApp);

  return 0;
}

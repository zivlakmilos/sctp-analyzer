#ifndef _APP_H_
#define _APP_H_

typedef struct SApp App;

App *appNew(int pArgc, char **psArgv);
void appDelete(App **ppApp);

void appRun(App *pApp);
void appExitWithError(App *pApp, char *fmt, ...);

#endif // !_APP_H_

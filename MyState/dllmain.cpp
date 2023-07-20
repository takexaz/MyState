// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"
#include <Mebius.h>
#include <StateControllerExtension.h>

int stcReg(TPFILE*, STATE_INFO*, PLAYER_CACHE*);
void stcProc(PLAYER*, STATE_INFO*);
void stcFree(STATE_INFO*);

struct MYSTATE {
    EVAL_EXP value;
    char x[256] = "";
};

// FALSE=Error
int stcReg(TPFILE* tpf, STATE_INFO* sinfo, PLAYER_CACHE* pcache) {
    MYSTATE* mystate = new MYSTATE;
    sinfo->params = mystate;
    DWORD TEMP;

    constExp(&mystate->value, 100);
    const char* value = TPGetValue(tpf, "value");
    if (value) {
        SCtrlReadExpList(value, "i", pcache, &TEMP, &mystate->value);
    }

    GetQuotedString(tpf, "text", mystate->x, sizeof(mystate->x));
    return TRUE;
}

void stcProc(PLAYER* p, STATE_INFO* sinfo) {
    MYSTATE* mystate = (MYSTATE*)sinfo->params;
    int life = EvalExpression(p,&mystate->value, 0);
    p->life = life;
    return;
}

void stcFree(STATE_INFO* sinfo) {
    MYSTATE* mystate = (MYSTATE*)sinfo->params;
    FreeExpression(&mystate->value);
    delete mystate;
    return;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        STX mystate = STX{
            "mystate",
            stcReg,
            stcProc,
            stcFree, 
        };
        addState(mystate);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


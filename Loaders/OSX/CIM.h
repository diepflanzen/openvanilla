// CIM.h: CarbonInputMethod, an IM framework for Apple Mac OS X
// Copyright (c) 2004-2005 The OpenVanilla Project

#ifndef __CIM_h
#define __CIM_h

#include <Carbon/Carbon.h>
#include "CIMConst.h"

struct CIMSessionData;
typedef CIMSessionData **CIMSessionHandle;

const int cimIBMaxLen=1024;

class CIMInputBuffer
{
public:
    CIMInputBuffer() { clear(); }
    void clear() { len=0; }
    void deletechar(); 
    int length();   // returns actual Unicode (UTF-32) chars, not UTF-16 chars
    int put(UniChar c);
    int put(CFStringRef s);
    int put(char *s, CFStringEncoding e);
    OSErr send();
    OSErr update(Boolean send=FALSE, int cursorpos=-1, int hilitefrom=-1, 
                 int hiliteto=-1, ScriptCode sc=cimScript, 
                 LangCode=cimLanguage);
    CIMInputBuffer *bind(ComponentInstance inst)
        { instance=inst; return this; }
    
protected:
    int realpos(int p);     // returns actual position in buffer (with surrogate)
    int len;
    int lastupdate;
    UniChar buffer[cimIBMaxLen];
    ComponentInstance instance;
};

struct CIMSessionData
{
    void init(ComponentInstance inst=nil)
    {
        sessionFixLock=0;
        instance=inst; buffer=new CIMInputBuffer; 
    }
    void kill() { delete buffer; }
    
    int sessionFixLock;
    ComponentInstance instance;
    CIMInputBuffer *buffer;
    void *data;             // for custom functions
};

ComponentResult CIMInitialize(ComponentInstance inst, MenuRef *mnu);
void            CIMTerminate(ComponentInstance inst);
ComponentResult CIMSessionOpen(ComponentInstance inst, CIMSessionHandle* hndl);
void            CIMSessionClose(CIMSessionHandle hndl);
ComponentResult CIMSessionActivate(CIMSessionHandle hndl);
ComponentResult CIMSessionDeactivate(CIMSessionHandle hndl);
ComponentResult CIMSessionFix(CIMSessionHandle hndl);
ComponentResult CIMSessionEvent(CIMSessionHandle hndl, EventRef evnt);
ComponentResult CIMSessionHidePalettes(CIMSessionHandle hndl);
ComponentResult CIMGetInputPosition(CIMSessionHandle hndl, Point *pnt);

// custom functions
int CIMCustomInitialize(MenuRef mnu);
void CIMCustomTerminate();
void *CIMCustomOpen();
int CIMCustomClose(void *data);
int CIMCustomActivate(void *data, CIMInputBuffer *buf);
int CIMCustomDeactivate(void *data, CIMInputBuffer *buf);
int CIMCustomSessionFix(void *data, CIMInputBuffer *buf);
int CIMCustomHandleInput(void *data, CIMInputBuffer *buf, 
    unsigned char charcode, UInt32 keycode, UInt32 modifiers, Point *pnt);
int CIMCustomMenuHandler(void *data, UInt32 command, MenuRef mnu, 
		CIMInputBuffer *buf);

#endif      // of #ifndef __CIM_h

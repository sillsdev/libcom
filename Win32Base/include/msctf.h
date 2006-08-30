#include "COM.h"

typedef	WPARAM wParam;
const	wParam MK_LBUTTON = ~0;
const	wParam MK_SHIFT = ~0;

typedef ULONG dwDynamicFlags;
typedef ULONG dwStaticFlags;
typedef struct {
  ULONG dwDynamicFlags;
  ULONG dwStaticFlags;
} TS_STATUS;
const	dwStaticFlags TS_SS_REGIONS = 0x2;
const 	dwStaticFlags TS_SS_NOHIDDENTEXT = 0x8;

typedef GUID TS_ATTRID;
typedef struct {
  TS_ATTRID idAttr;
  DWORD dwOverlapId;
  VARIANT varValue;
} TS_ATTRVAL;

typedef DWORD TsViewCookie;
typedef DWORD TfClientId;
typedef DWORD TfEditCookie;
typedef DWORD TfGuidAtom;

typedef	TfClientId	TF_CLIENTID_NULL;
typedef	TfGuidAtom	TF_INVALID_GUIDATOM;

const	ULONG	TS_E_NOSELECTION = 0x80040205;
const	ULONG	TS_E_NOLOCK = 0x80040201;
const	ULONG	TS_E_SYNCHRONOUS = 0x00040300;
const	ULONG	TS_E_INVALIDPOS = 0x80040200;
const	ULONG	TF_POPF_ALL = 0x1;
const	ULONG	TS_S_ASYNC = 0x1;
const	ULONG	TS_LF_SYNC = 0x1;
const	ULONG	TS_LF_READ = 0x2;
const	ULONG	TS_IAS_NOQUERY = 0x1;
const	ULONG	TS_IAS_QUERYONLY = 0x2;
const	ULONG	TS_ST_CORRECTION = 0x1;
const	ULONG	TS_AS_TEXT_CHANGE = 0x1;
const	ULONG	TS_AS_SEL_CHANGE = 0x2;
const	ULONG	TS_AS_LAYOUT_CHANGE = 0x4;
const	ULONG	TF_DEFAULT_SELECTION = (ULONG)-1;
const	ULONG	CONNECT_E_ADVISELIMIT = ~0;
const	ULONG	CONNECT_E_NOCONNECTION = ~0;
	
typedef enum 
{
  TS_RT_PLAIN, 
  TS_RT_HIDDEN, 
  TS_RT_OPAQUE
} TsRunType;

typedef struct {
  ULONG uCount;
  TsRunType type;
} TS_RUNINFO;

typedef struct {
  LONG acpStart;
  LONG acpOldEnd;
  LONG acpNewEnd;
} TS_TEXTCHANGE;

typedef enum {
  TS_AE_NONE = 0, 
  TS_AE_START = 1, 
  TS_AE_END = 2
} TsActiveSelEnd;

typedef struct {
  TsActiveSelEnd ase;
  BOOL fInterimChar;
} TS_SELECTIONSTYLE;

typedef struct {
  LONG acpStart;
  LONG acpEnd;
  TS_SELECTIONSTYLE style;
} TS_SELECTION_ACP;

enum { TS_LF_READWRITE, TS_LC_CHANGE };
	
typedef enum {
  TF_CT_NONE = 0, 
  TF_CT_SYSCOLOR = 1, 
  TF_CT_COLORREF = 2
} TF_DA_COLORTYPE;

typedef struct {
  TF_DA_COLORTYPE type;
  union {
    int nIndex;
    COLORREF cr;
  };
} TF_DA_COLOR;

typedef enum {
  TF_LS_NONE = 0, 
  TF_LS_SOLID = 1, 
  TF_LS_DOT = 2, 
  TF_LS_DASH = 3, 
  TF_LS_SQUIGGLE = 4
} TF_DA_LINESTYLE;

typedef enum {
  TF_ATTR_INPUT = 0, 
  TF_ATTR_TARGET_CONVERTED = 1, 
  TF_ATTR_CONVERTED = 2, 
  TF_ATTR_TARGET_NOTCONVERTED = 3, 
  TF_ATTR_INPUT_ERROR = 4, 
  TF_ATTR_FIXEDCONVERTED = 5, 
  TF_ATTR_OTHER = -1
} TF_DA_ATTR_INFO;

typedef struct {
  TF_DA_COLOR crText;
  TF_DA_COLOR crBk;
  TF_DA_LINESTYLE lsStyle;
  BOOL fBoldLine;
  TF_DA_COLOR crLine;
  TF_DA_ATTR_INFO bAttr;
} TF_DISPLAYATTRIBUTE;

class ITextStoreACP : public IUnknown
{
public:
	HRESULT GetSelection(ULONG ulIndex, ULONG, TS_SELECTION_ACP*, ULONG*);
	HRESULT GetText(LONG, LONG, WCHAR*, ULONG, ULONG*, TS_RUNINFO*, ULONG, ULONG*, LONG*);
};

class ITextStoreACPSink : public IUnknown
{
public:
	HRESULT OnLayoutChange(int, TsViewCookie&);
	HRESULT OnLockGranted(DWORD);
	HRESULT OnTextChange(DWORD, TS_TEXTCHANGE*);
	HRESULT OnSelectionChange();
};

class ITextStoreACPServices : public IUnknown
{
};

class ITfCategoryMgr : public IUnknown
{
public:
	HRESULT GetGUID(TfGuidAtom, GUID*);
};

class ITfDisplayAttributeInfo : public IUnknown
{
public:
	HRESULT GetAttributeInfo(TF_DISPLAYATTRIBUTE*);
	
};

class ITfDisplayAttributeMgr : public IUnknown
{
public:
	HRESULT GetDisplayAttributeInfo(REFGUID, ITfDisplayAttributeInfo**, CLSID*);
};

class ITfRange : public IUnknown
{
};

class IEnumTfRanges : public IUnknown
{
public:
	HRESULT Next(ULONG, ITfRange**, ULONG*);
};

class ITfRangeACP : public IUnknown
{
public:
	HRESULT GetExtent(LONG*, LONG*);
};

class ITfReadOnlyProperty : public IUnknown
{
public:
	HRESULT GetValue(TfEditCookie, ITfRange*, VARIANT*);
	HRESULT EnumRanges(TfEditCookie, IEnumTfRanges**, ITfRange*);
};

class ITfProperty : public ITfReadOnlyProperty
{
};

class ITfContext : public IUnknown
{
public:
	HRESULT GetProperty(REFGUID, ITfProperty**);
};

class ITfDocumentMgr : public IUnknown
{
public:
	HRESULT CreateContext(TfClientId, DWORD, IUnknown*, ITfContext**, TfEditCookie*);
	HRESULT Push(ITfContext*);
	HRESULT Pop(DWORD);
};

class ITfThreadMgr : public IUnknown
{
public:
	HRESULT CreateDocumentMgr(ITfDocumentMgr**);
	HRESULT SetFocus(ITfDocumentMgr*);
	HRESULT Deactivate();
};

class ITfMouseSink : public IUnknown
{
public:
	HRESULT OnMouseEvent(ULONG, ULONG, DWORD, BOOL*);	
};

class ITfContextOwnerCompositionSink : public IUnknown
{
};

class ITfMouseTrackerACP : public IUnknown
{
};

class ITfCompositionView : public IUnknown
{
};

class ITfContextOwnerCompositionServices : public IUnknown
{
public:
	HRESULT TerminateComposition(ITfCompositionView*);
};

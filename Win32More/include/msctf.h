typedef LONG TS_STATUS;
typedef LONG TS_SELECTION_ACP;
typedef LONG TS_RUNINFO;
typedef LONG TS_ATTRVAL;
typedef LONG TS_ATTRID;

typedef struct {
  LONG acpStart;
  LONG acpOldEnd;
  LONG acpNewEnd;
} TS_TEXTCHANGE;

typedef DWORD TsViewCookie;
typedef DWORD TfClientId;
typedef DWORD TfEditCookie;

enum { TS_LF_READWRITE, TS_LC_CHANGE };
	
typedef enum 
{
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

typedef enum 
{
  TF_LS_NONE = 0, 
  TF_LS_SOLID = 1, 
  TF_LS_DOT = 2, 
  TF_LS_DASH = 3, 
  TF_LS_SQUIGGLE = 4
} TF_DA_LINESTYLE;

typedef enum 
{
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

class ITfThreadMgr : public IUnknown
{
public:
	HRESULT Deactivate();
};

class ITfDocumentMgr : public IUnknown
{
};

class ITfContext : public IUnknown
{
};

class ITfCategoryMgr : public IUnknown
{
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

class ITfProperty : public IUnknown
{
};

class ITfReadOnlyProperty : public IUnknown
{
};

class IEnumTfRanges : public IUnknown
{
};

class ITfRange : public IUnknown
{
};

class ITfRangeACP : public IUnknown
{
public:
	HRESULT GetExtent(LONG*, LONG*);
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

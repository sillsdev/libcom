typedef LONG TS_STATUS;
typedef LONG TS_SELECTION_ACP;
typedef LONG TS_RUNINFO;
typedef LONG TS_TEXTCHANGE;
typedef LONG TS_TEXTCHANGE;
typedef LONG TS_ATTRVAL;
typedef LONG TS_ATTRID;
typedef LONG TS_TEXTCHANGE;
typedef LONG TS_TEXTCHANGE;

typedef DWORD TsViewCookie;
typedef DWORD TfClientId;
typedef DWORD TfEditCookie;

enum { TS_LF_READWRITE, TS_LC_CHANGE };

class ITextStoreACP : public IUnknown
{
};

class ITextStoreACPSink : public IUnknown
{
public:
	HRESULT OnLayoutChange(int, TsViewCookie&);
};

class ITextStoreACPServices : public IUnknown
{
};

class ITfThreadMgr : public IUnknown
{
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

class ITfDisplayAttributeMgr : public IUnknown
{
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
};

class ITfDisplayAttributeInfo : public IUnknown
{
};

class ITfMouseSink : public IUnknown
{
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

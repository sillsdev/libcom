/*
 *	$Id$
 *
 *	OLEDB Error or Status Codes
 *
 *	MarkS - 2008-01-14
 *
 * Win32More Compatibility Library
 * Copyright (C) 2008 SIL International
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

// This file was built by doing the following:
// * Copy the many lines from Util.cpp that start with "CASE_HRESULT(DB_" to a file.
// * sed it to produce this file's list of enumerators.
// * Adjust values of certain enumerators to match the standard.
// * Added 7 enumerators that weren't in Util.cpp.

#ifndef oledberr_h_included
#define oledberr_h_included

enum OleDbErrCodes {
	DB_E_BADACCESSORHANDLE = (HRESULT)0x80040e00L,
	DB_E_ROWLIMITEXCEEDED,
	DB_E_READONLYACCESSOR,
	DB_E_SCHEMAVIOLATION,
	DB_E_BADROWHANDLE,
	DB_E_OBJECTOPEN,
	DB_E_BADCHAPTER,
	DB_E_CANTCONVERTVALUE,
	DB_E_BADBINDINFO,
	DB_E_NOTAREFERENCECOLUMN = (HRESULT)0x80040e0aL,
	DB_E_LIMITREJECTED,
	DB_E_NOCOMMAND,
	DB_E_COSTLIMIT,
	DB_E_BADBOOKMARK,
	DB_E_BADLOCKMODE,
	DB_E_PARAMNOTOPTIONAL,
	DB_E_BADCOLUMNID,
	DB_E_BADRATIO,
	DB_E_BADVALUES,
	DB_E_ERRORSINCOMMAND,
	DB_E_CANTCANCEL,
	DB_E_DIALECTNOTSUPPORTED,
	DB_E_DUPLICATEDATASOURCE,
	DB_E_CANNOTRESTART,
	DB_E_NOTFOUND,
	DB_E_NEWLYINSERTED = (HRESULT)0x80040e1bL,
	DB_E_CANNOTFREE = (HRESULT)0x80040e1aL,
	DB_E_GOALREJECTED = (HRESULT)0x80040e1cL,
	DB_E_UNSUPPORTEDCONVERSION,
	DB_E_BADSTARTPOSITION,
	DB_E_NOQUERY,
	DB_E_NOTREENTRANT,
	DB_E_ERRORSOCCURRED,
	DB_E_NOAGGREGATION,
	DB_E_DELETEDROW,
	DB_E_CANTFETCHBACKWARDS,
	DB_E_ROWSNOTRELEASED,
	DB_E_BADSTORAGEFLAG,
	DB_E_BADCOMPAREOP,
	DB_E_BADSTATUSVALUE,
	DB_E_CANTSCROLLBACKWARDS,
	DB_E_BADREGIONHANDLE,
	DB_E_NONCONTIGUOUSRANGE,
	DB_E_INVALIDTRANSITION,
	DB_E_NOTASUBREGION,
	DB_E_MULTIPLESTATEMENTS,
	DB_E_INTEGRITYVIOLATION,
	DB_E_BADTYPENAME,
	DB_E_ABORTLIMITREACHED,
	DB_E_ROWSETINCOMMAND,
	DB_E_CANTTRANSLATE,
	DB_E_DUPLICATEINDEXID,
	DB_E_NOINDEX,
	DB_E_INDEXINUSE,
	DB_E_NOTABLE,
	DB_E_CONCURRENCYVIOLATION,
	DB_E_BADCOPY,
	DB_E_BADPRECISION,
	DB_E_BADSCALE,
	DB_E_BADTABLEID,
	DB_E_BADTYPE,
	DB_E_DUPLICATECOLUMNID,
	DB_E_DUPLICATETABLEID,
	DB_E_TABLEINUSE,
	DB_E_NOLOCALE,
	DB_E_BADRECORDNUM,
	DB_E_BOOKMARKSKIPPED,
	DB_E_BADPROPERTYVALUE,
	DB_E_INVALID,
	DB_E_BADACCESSORFLAGS,
	DB_E_BADSTORAGEFLAGS,
	DB_E_BYREFACCESSORNOTSUPPORTED,
	DB_E_NULLACCESSORNOTSUPPORTED,
	DB_E_NOTPREPARED,
	DB_E_BADACCESSORTYPE,
	DB_E_WRITEONLYACCESSOR,
	DB_E_CANCELED = (HRESULT)0x80040e4eL,
	DB_E_CHAPTERNOTRELEASED,
	DB_E_BADSOURCEHANDLE,
	DB_E_PARAMUNAVAILABLE,
	DB_E_ALREADYINITIALIZED,
	DB_E_NOTSUPPORTED,
	DB_E_MAXPENDCHANGESEXCEEDED,
	DB_E_BADORDINAL,
	DB_E_PENDINGCHANGES,
	DB_E_DATAOVERFLOW,
	DB_E_BADHRESULT,
	DB_E_BADLOOKUPID,
	DB_E_BADDYNAMICERRORID,
	DB_E_PENDINGINSERT,
	DB_E_BADCONVERTFLAG,
	DB_E_BADPARAMETERNAME,
	DB_E_MULTIPLESTORAGE,
	DB_E_CANTFILTER,
	DB_E_CANTORDER,
	DB_E_NOCOLUMN = (HRESULT)0x80040e65L,
	DB_E_COMMANDNOTPERSISTED = (HRESULT)0x80040e67L,
	DB_E_DUPLICATEID,
	DB_E_OBJECTCREATIONLIMITREACHED,
	DB_E_BADINDEXID = (HRESULT)0x80040e72L,
	DB_E_BADINITSTRING,
	DB_E_NOPROVIDERSREGISTERED,
	DB_E_MISMATCHEDPROVIDER,
	DB_E_BADCOMMANDID,
	DB_E_BADCONSTRAINTTYPE,
	DB_E_BADCONSTRAINTFORM,
	DB_E_BADDEFERRABILITY,
	DB_E_BADMATCHTYPE = (HRESULT)0x80040e80L,
	DB_E_BADUPDATEDELETERULE = (HRESULT)0x80040e8aL,
	DB_E_BADCONSTRAINTID,
	DB_E_BADCOMMANDFLAGS,
	DB_E_OBJECTMISMATCH,
	DB_E_NOSOURCEOBJECT = (HRESULT)0x80040e91L,
	DB_E_RESOURCELOCKED,
	DB_E_NOTCOLLECTION,
	DB_E_READONLY,
	DB_E_ASYNCNOTSUPPORTED,
	DB_E_CANNOTCONNECT,
	DB_E_TIMEOUT,
	DB_E_RESOURCEEXISTS,
	DB_E_RESOURCEOUTOFSCOPE = (HRESULT)0x80040e8eL,
	DB_E_DROPRESTRICTED = (HRESULT)0x80040e90L,
	DB_E_DUPLICATECONSTRAINTID = (HRESULT)0x80040e99L,
	DB_E_OUTOFSPACE,
	DB_SEC_E_PERMISSIONDENIED = (HRESULT)0x80040e09L,
	DB_SEC_E_AUTH_FAILED = (HRESULT)0x80040e4dL,
	DB_SEC_E_SAFEMODE_DENIED = (HRESULT)0x80040e9bL,
	DB_S_ROWLIMITEXCEEDED = (HRESULT)0x00040ec0L,
	DB_S_COLUMNTYPEMISMATCH,
	DB_S_TYPEINFOOVERRIDDEN,
	DB_S_BOOKMARKSKIPPED,
	DB_S_NONEXTROWSET = (HRESULT)0x00040ec5L,
	DB_S_ENDOFROWSET,
	DB_S_COMMANDREEXECUTED,
	DB_S_BUFFERFULL,
	DB_S_NORESULT,
	DB_S_CANTRELEASE,
	DB_S_GOALCHANGED,
	DB_S_UNWANTEDOPERATION,
	DB_S_DIALECTIGNORED,
	DB_S_UNWANTEDPHASE,
	DB_S_UNWANTEDREASON,
	DB_S_ASYNCHRONOUS,
	DB_S_COLUMNSCHANGED,
	DB_S_ERRORSRETURNED,
	DB_S_BADROWHANDLE,
	DB_S_DELETEDROW,
	DB_S_TOOMANYCHANGES,
	DB_S_STOPLIMITREACHED,
	DB_S_LOCKUPGRADED = (HRESULT)0x00040ed8L,
	DB_S_PROPERTIESCHANGED,
	DB_S_ERRORSOCCURRED,
	DB_S_PARAMUNAVAILABLE,
	DB_S_MULTIPLECHANGES,
	DB_S_NOTSINGLETON = (HRESULT)0x00040ed7L,
	DB_S_NOROWSPECIFICCOLUMNS = (HRESULT)0x00040eddL,

	DB_E_BOGUS = (HRESULT)0x80040effL,
	DB_E_BADID = DB_E_BADTABLEID,
	DB_E_NOSTATISTIC  = (HRESULT)0x80040e9cL,
	DB_E_ALTERRESTRICTED,
	DB_E_RESOURCENOTSUPPORTED,
	DB_E_NOCONSTRAINT,
	DB_E_COLUMNUNAVAILABLE = (HRESULT)0x80040ea0L,
};

#endif // oledberr_h_included

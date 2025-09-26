
#include "StdAfx.h"
//#include "../include/frd_Register.h"

//#include <MAPI.h>

#include "../include/FUtility.h"

#include <string>
//#include <MAPIX.h>
//#include <Mapiutil.h>
#include <Strsafe.h>
#include <winreg.h>
#include <vector>
using namespace std;

#pragma comment(lib, "Version.lib")

FS_FileRead FUtility::fsFileReader = {0};

SRWLOCK FUtility::s_srwlock;
CONDITION_VARIABLE FUtility::s_cvReadyToPrint;
//CONDITION_VARIABLE FUtility::s_cvStopResolving;
FUtility::V_RESOLVE_RESULTS FUtility::s_nameResolveResults;

LONG FUtility::s_LResolutionFailed = 0;

#define REG_PI_LOADBEHAVIOR_CONNECTED			0x00000001

FUtility::FUtility()
{
}



BOOL FUtility::GetEmailFromList(const CString& EmailList, int nIndex, CString& email)
{
	BOOL bResult = FALSE;

	if(!EmailList.IsEmpty())
	{
		CString strEmailList = EmailList;
		strEmailList.TrimRight(L" ;");
		strEmailList += L";";
		int nStart = 0;
		int nIndexEmail = 0;
		for(;;nIndexEmail++)
		{
			const int nsemiColon = strEmailList.Find(L';', nStart);
			if(-1 == nsemiColon)
				break;
			const CString strEmail = strEmailList.Mid(nStart, nsemiColon - nStart);
			nStart = nsemiColon + 1;
			if(nIndex == nIndexEmail)
			{
				bResult = TRUE;
				email = strEmail;
				break;
			}
		}

	}

	return bResult;
}


BOOL FUtility::ValidateEmail(const CString& email)
{
		//***********************************
	// E-mail validate  copied from ykw's code implementation
	//***********************************
	//Email??????: "^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$"
	//?????????email??????
	//?@?.???,????***@***.***?email?? 
	//???????,@?????"a-z / 0-9 / _ / ."
	CString strEmail = email;
	strEmail.MakeLower();

	// the length of the email could be any length
// 	if (strEmail.GetLength() > 63)	//Email长度<=63
// 		return FALSE;
	
	int aPos=strEmail.Find(L'@');
	if (aPos<1)
		return FALSE;
	
	if (strEmail.Find(L'@', aPos+1) != -1)		//@不能出现2次
		return FALSE;
	
	// verify characters before '@'
	int i = 0;
	for (i=aPos-1; i>=0; i--)
	{
		WCHAR ch = strEmail.GetAt(i);
		/*if (ch == L'-' || ch == L'.' ||ch == L'_' ||(ch >= L'a' && ch <=L'z') || (ch >=L'0' && ch <= L'9') || ch == L'$')
			continue;
		else
		{
			return FALSE;
		}*/

		//()[];:"<>,.

		if (ch == L'(' || ch == L')' || ch == L'[' || ch == L']' || ch == L';'
			|| ch == L':' || ch == L'"' || ch == L'<' || ch == L'>' || ch == L',' /*|| ch == L'.'*/)
		{
			return FALSE;
		}
	}
	
	int ePos  = strEmail.Find(L'.',aPos);
	if (ePos<aPos+2 || ePos+1 >= strEmail.GetLength())
		return FALSE;

	// verify characters after '@' and first '.'
	for (i=aPos+1; i<ePos ;i++)
	{
		WCHAR ch = strEmail.GetAt(i);
		if (ch == L'_' ||(ch >= L'a' && ch <=L'z') || (ch >=L'0' && ch <= L'9'))
			break;	
		else
		{
			return FALSE;
		}
	}
	// verify characters after the first '.' and last '.'
	int nFinalPos = strEmail.ReverseFind(L'.');
	for (i=ePos+1; i < nFinalPos; i++)
	{
		WCHAR wch = strEmail.GetAt(i);
		if(wch == L'.'  || wch == L'-' || wch == L'_' || (wch >= L'a' && wch <=L'z') || (wch >= L'0' && wch <= L'9'))
			continue;
		else
		{
			return FALSE;
		}
	}

	if(nFinalPos == strEmail.GetLength() - 1)
		return FALSE;
	for(i = nFinalPos + 1; i < strEmail.GetLength(); i++)
	{
		WCHAR ch = strEmail.GetAt(i);
		if(ch == L'.' || (ch >= L'a' && ch <= L'z'))
			continue;
		else
			return FALSE;
	}
	return TRUE;

}




BOOL FUtility::ValidateEmailList(const CString& EmailList, CString& invalidEmail)
{
	BOOL bResult = TRUE;

	if(!EmailList.IsEmpty())
	{
		CString strEmailList = EmailList;
		strEmailList.TrimRight(L" ;");
		strEmailList += L";";
		int nStart = 0;
		for(;;)
		{
			const int nsemiColon = strEmailList.Find(L';', nStart);
			if(-1 == nsemiColon)
				break;
			CString strEmail = strEmailList.Mid(nStart, nsemiColon - nStart);
			nStart = nsemiColon + 1;
			strEmail = strEmail.Trim(L' ');
			bResult = ValidateEmail(strEmail);
			if(!bResult)
			{
				invalidEmail = strEmail;
				break;
			}
		}
	}

	return bResult;
}

Outlook::_ApplicationPtr FUtility::CreateOutLookApp()
{
	HRESULT hr = S_OK;
	Outlook::_ApplicationPtr spOutlookApp;
	if (spOutlookApp == NULL)
	{
		int a = 0;
	}
	try
	{
		/////////////////////////////////////////////////////////////////////
		// Start Microsoft Outlook and log on with your profile.
		//	

		int nDisable = FRAppRegistryGetInt((FS_LPCWSTR)g_wszRMSSettingSection, (FS_LPCWSTR)g_wszDisableOutlookFeature);	
		if (nDisable == 1)
		{
			return NULL;
		}

		//for pre outlook2013 HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\Windows Messaging Subsystem\Profiles
		//for outlook2013 HKEY_CURRENT_USER\Software\Microsoft\Office\15.0\Outlook\Profiles
		//check whether outlook account is configured
		LPCWSTR lpwsSubKey = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows Messaging Subsystem\\Profiles";
		CStringArray arrSubKey;
		int nProfileCount = CReader_Register::RegEnumKey(HKEY_CURRENT_USER, lpwsSubKey, arrSubKey);
		if (nProfileCount <= 0)
		{
			lpwsSubKey = L"Software\\Microsoft\\Office\\15.0\\Outlook\\Profiles";
			nProfileCount = CReader_Register::RegEnumKey(HKEY_CURRENT_USER, lpwsSubKey, arrSubKey);
			if (nProfileCount <= 0)
			{
				//spOutlookApp->raw_Quit();
				return NULL;
			}
		}

		hr = spOutlookApp.CreateInstance(__uuidof(Outlook::Application));
		if (FAILED(hr))
		{
			TRACE(L"CreateInstance failed w/err 0x%08lx\n", hr);
			return NULL;
		}	
	}
	catch (_com_error &err)
	{
		TRACE(L"Outlook throws the error: %s\n", err.ErrorMessage());
		TRACE(L"Description: %s\n", (LPCWSTR) err.Description());
		hr = err.Error();
		return NULL;
	}

	// Uninitialize COM for this thread.
	return spOutlookApp;
}

struct THREADPARAM 
{
	FS_WideString wsDialogCaption;
	BOOL bExit;
};
UINT __cdecl PopWindow(LPVOID pParam)
{
	THREADPARAM* pThreadParam = (THREADPARAM*)pParam;

	CWnd *pWndPrev = NULL;
	while (TRUE)
	{
		if (pThreadParam->bExit)
		{
			break;
		}

		pWndPrev = CWnd::FindWindow(L"#32770", L"Microsoft Outlook");
		if (pWndPrev == NULL)
		{
			pWndPrev = CWnd::FindWindow(L"#32770", L"Microsoft Office Outlook");
			if (pWndPrev == NULL)
			{
				if (!pThreadParam->bExit)
				{
					LPCWSTR lpwsCaption = (LPCWSTR)FSWideStringCastToLPCWSTR(pThreadParam->wsDialogCaption);
					pWndPrev = CWnd::FindWindow(L"#32770", lpwsCaption/*L"Select Names: Global Address"*/);
				}
			}
		}
		
		if (pWndPrev != NULL)
		{			
			pWndPrev->ShowWindow(SW_SHOW); 
			pWndPrev->SetWindowPos(&(CWnd::wndTopMost),0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); 
			pWndPrev->SetWindowPos(&(CWnd::wndNoTopMost),0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

			break;			
		}
	}

	return 0;
}

HRESULT FUtility::SelectUsersByOutlookModel(Outlook::_ApplicationPtr spOutlookApp, int nBtnType, LPCWSTR lpwsFirstLabel, LPCWSTR lpwsSecondLabel, 
											CString& wsUserRead, CString& wsUserChange, BOOL& bCanUse)
{
	bCanUse = FALSE;
	if (spOutlookApp == NULL)
	{
		return S_FALSE;
	}

	bCanUse = TRUE;

	Outlook::_NameSpacePtr spNS = NULL;
	HRESULT hr = S_OK;

	THREADPARAM* pThreadParam = new THREADPARAM;
	pThreadParam->wsDialogCaption = FSWideStringNew3((FS_LPCWSTR)L"Temp Name", -1);
	pThreadParam->bExit = FALSE;

	try
	{
		// Get the namespace.
		CWnd *pWndPrev = NULL;
		if (pWndPrev = CWnd::FindWindow(L"rctrl_renwnd32", NULL))
		{
			BOOL bVisible = pWndPrev->IsWindowVisible();
			if (bVisible)
			{
				//pWndPrev->SetWindowPos(&(CWnd::wndTop), 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
				pWndPrev->ShowWindow(SW_SHOW); 
				pWndPrev->SetWindowPos(&(CWnd::wndTopMost),0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); 
				pWndPrev->SetWindowPos(&(CWnd::wndNoTopMost),0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			}
			else
			{
				CWinThread* pWinThread = NULL;
				pWinThread = AfxBeginThread(PopWindow, (LPVOID)pThreadParam);
			}
		}		
		else
		{
			CWinThread* pWinThread = NULL;
			pWinThread = AfxBeginThread(PopWindow, (LPVOID)pThreadParam);
		}

		spNS = spOutlookApp->GetNamespace(_bstr_t(L"MAPI"));
		 
		/*Outlook::_ExplorersPtr spExplorers = spOutlookApp->GetExplorers();
		if (spExplorers != NULL)
		{
			long lCountExplorer = spExplorers->GetCount();
			for (int j=1; j<=lCountExplorer; j++)
			{
				Outlook::_ExplorerPtr spExplorer = spExplorers->Item(j);
				_bstr_t bsCaption = spExplorer->GetCaption();
				spExplorer->Activate();
			}
		}*/

		// Log on by using a dialog box to choose the profile.
		//spNS->Logon(vtMissing, vtMissing, true, true);

		/////////////////////////////////////////////////////////////////////
		// Enumerate the contact items.

		//warning dialog solution
		//http://office.microsoft.com/en-us/outlook-help/i-get-warnings-about-a-program-accessing-e-mail-address-information-or-sending-e-mail-on-my-behalf-HA001229943.aspx
		//http://supportold.contactpad.com/index.php?p=/discussion/9/outlook-warning-about-access-to-address-book/p1
		Outlook::MAPIFolderPtr spCtFolder = spNS->GetDefaultFolder(Outlook::olFolderContacts);

		Outlook::AddressListPtr spAddrList = NULL;
		Outlook::AddressListsPtr spAddrLists = spNS->GetAddressLists();
		if (spAddrLists)
		{
			int nCount = spAddrLists->GetCount();
			if(nCount > 0)
			{
				int i = 1;
				while(i < nCount)
				{
					spAddrList = spAddrLists->Item(i);
					if (Outlook::olExchangeGlobalAddressList == spAddrList->AddressListType )
					{
						break;
					}
					else
						i++;
				}
				if (i == nCount)
				{
					spAddrList = spAddrLists->Item(1);
				}
			}
			
		}

		Outlook::_SelectNamesDialogPtr spSelectNamesDialog = spNS->GetSelectNamesDialog();
		//spSelectNamesDialog->ForceResolution = VARIANT_FALSE;

		_bstr_t bsCaption = spSelectNamesDialog->GetCaption();
		if (spAddrList != NULL)
		{
			spSelectNamesDialog->PutInitialAddressList(spAddrList);
			_bstr_t bsName = spAddrList->GetName();
			bsCaption += L": ";
			bsCaption += bsName;
		}	
		FSWideStringFill(pThreadParam->wsDialogCaption, (FS_LPCWSTR)(LPCWSTR)bsCaption);

		spSelectNamesDialog->AllowMultipleSelection = TRUE;
		Outlook::RecipientsPtr spRecipients = spSelectNamesDialog->GetRecipients();
		if (nBtnType == 1)
		{
			spSelectNamesDialog->NumberOfRecipientSelectors = Outlook::olShowToCc;
			spSelectNamesDialog->ToLabel = lpwsFirstLabel;
			spSelectNamesDialog->CcLabel = lpwsSecondLabel;

			for(int j = 0; ; j++)
			{
				CString strEmail;
				if(!FUtility::GetEmailFromList(wsUserRead, j, strEmail))
					break;
				Outlook::RecipientPtr spRecipient = spRecipients->Add((LPCWSTR)strEmail);
				spRecipient->PutType(Outlook::olTo);
			}
			for (int k=0;; k++)
			{
				CString strEmail;
				if(!FUtility::GetEmailFromList(wsUserChange, k, strEmail))
					break;
				Outlook::RecipientPtr spRecipient = spRecipients->Add((LPCWSTR)strEmail);
				spRecipient->PutType(Outlook::olCC);
			}
		}
		else if (nBtnType == 2)
		{
			spSelectNamesDialog->NumberOfRecipientSelectors = Outlook::olShowTo;
			spSelectNamesDialog->ToLabel = lpwsFirstLabel;

			for (int k=0;; k++)
			{
				CString strEmail;
				if(!FUtility::GetEmailFromList(wsUserChange, k, strEmail))
					break;
				Outlook::RecipientPtr spRecipient = spRecipients->Add((LPCWSTR)strEmail);
				spRecipient->PutType(Outlook::olTo);
			}
		}
		
		BOOL bDisplay = spSelectNamesDialog->Display();

		if (!bDisplay)
		{
			spNS->Logoff();

			pThreadParam->bExit = TRUE;			
			FSWideStringDestroy(pThreadParam->wsDialogCaption);
			delete pThreadParam;
			
			return S_FALSE;
		}

		spRecipients = spSelectNamesDialog->GetRecipients();
		int nRecipientCount = spRecipients->GetCount();
		wsUserRead = L"";//clear the read user
		wsUserChange = L"";//clear the change user
		for (int i=1; i<=nRecipientCount; i++)
		{
			Outlook::RecipientPtr spRecipient = spRecipients->Item(i);
			_bstr_t bsAddress;
			
			Outlook::AddressEntryPtr spAddEntry = spRecipient->GetAddressEntry();	
			if (spAddEntry != NULL)
			{
				Outlook::_ExchangeUserPtr spExchangeUser = spAddEntry->GetExchangeUser();
				Outlook::_ExchangeDistributionListPtr spExchangeDistList = spAddEntry->GetExchangeDistributionList();
				if (spExchangeUser != NULL)
				{
					bsAddress = spExchangeUser->GetPrimarySmtpAddress();
				}
				else if(spExchangeDistList != NULL)
				{
					bsAddress = spExchangeDistList->GetPrimarySmtpAddress();
				}
				else
				{
					bsAddress = spRecipient->GetAddress();
				}
			}
			else
			{
				bsAddress = spRecipient->GetAddress();
			}
			

			long lType = spRecipient->GetType();
			if (nBtnType == 1 && lType == Outlook::olTo)
			{
				if(!wsUserRead.IsEmpty())
					wsUserRead += L";";
				wsUserRead += (LPCWSTR)bsAddress;
			}
			else if (nBtnType == 1 && lType == Outlook::olCC)
			{
				if(!wsUserChange.IsEmpty())
					wsUserChange += L";";
				wsUserChange += (LPCWSTR)bsAddress;
			}
			else if (nBtnType == 2 && lType == Outlook::olTo)
			{
				if(!wsUserChange.IsEmpty())
					wsUserChange += L";";
				wsUserChange += (LPCWSTR)bsAddress;
			}
		}

		spNS->Logoff();
	}
	catch (_com_error &err)
	{
		if (spNS != NULL)
		{
			spNS->Logoff();
		}
		TRACE(L"Outlook throws the error: %s\n", err.ErrorMessage());
		TRACE(L"Description: %s\n", (LPCWSTR) err.Description());
		hr = err.Error();
	}

	// Uninitialize COM for this thread.
	pThreadParam->bExit = TRUE;
	FSWideStringDestroy(pThreadParam->wsDialogCaption);
	delete pThreadParam;

	return hr;
}

CStringArray g_arrayAddress;
BOOL g_bHasGetAllUsers = FALSE;


HRESULT FUtility::CheckUserByOutlookModel(CString& wsAddressToCheck)
{
	if (wsAddressToCheck.IsEmpty())
	{
		return S_FALSE;
	}
	wsAddressToCheck.TrimLeft();
	wsAddressToCheck.TrimRight();

	int nAddressSize = g_arrayAddress.GetSize();
	
	for (int i=0; i<nAddressSize; i++)
	{
		CString csAddress = g_arrayAddress.GetAt(i);
		CString csEmailAddressTemp = csAddress;
		csEmailAddressTemp.MakeLower();
		CString csAddressToCheckTemp = wsAddressToCheck;
		csAddressToCheckTemp.MakeLower();
		if (csEmailAddressTemp.Find(csAddressToCheckTemp) != -1)
		{
			wsAddressToCheck = csAddress;
			return S_OK;
		}
	}
	
	return S_FALSE;
}

HRESULT FUtility::SelectUsers(HWND hParent, PCWSTR wszTargetComputer, CString& selUser)
{
	HRESULT hr = S_OK;
	
	IDsObjectPicker *pDsObjectPicker = NULL;
	IDataObject *pdo = NULL;

	hr = CoInitialize(NULL);
	if(SUCCEEDED(hr))
	{
		// Create an instance of the object picker. 
		hr = CoCreateInstance(  CLSID_DsObjectPicker,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IDsObjectPicker,
			(void**) &pDsObjectPicker);
		if(SUCCEEDED(hr))
		{
			// Initialize the object picker instance. 
			hr = InitObjectPicker(pDsObjectPicker , wszTargetComputer);
			if(SUCCEEDED(hr))
			{
				// Supply a window handle to the application.
				HWND hwndParent = hParent; 

				// Invoke the modal dialog box.
				hr = pDsObjectPicker->InvokeDialog(hwndParent, &pdo);
				if (hr == S_OK) 
				{
					ProcessSelectedObjects(pdo, selUser);
					pdo->Release();
				}
			}
			pDsObjectPicker->Release();
		}	

		CoUninitialize();
	}
	return hr;
}



HRESULT FUtility::InitObjectPicker(IDsObjectPicker* pDsObjectPicker, PCWSTR wszTargetComputer)
{
	if(NULL == pDsObjectPicker)
    {
        return E_INVALIDARG;
    }

    DSOP_SCOPE_INIT_INFO aScopeInit[1];
    DSOP_INIT_INFO  InitInfo;
	
    // Initialize the DSOP_SCOPE_INIT_INFO array.
    ZeroMemory(aScopeInit, sizeof(aScopeInit));
	
    // Combine multiple scope types in a single array entry.
    aScopeInit[0].cbSize = sizeof(DSOP_SCOPE_INIT_INFO);
    aScopeInit[0].flType = DSOP_SCOPE_TYPE_UPLEVEL_JOINED_DOMAIN
		| DSOP_SCOPE_TYPE_DOWNLEVEL_JOINED_DOMAIN;
	if (wszTargetComputer!=NULL)
		aScopeInit[0].flType |= DSOP_SCOPE_TYPE_TARGET_COMPUTER;
	
		/*
		Set up-level and down-level filters to include only computer 
		objects. Up-level filters apply to both mixed and native modes. 
		Be aware that the up-level and down-level flags are different.
    */
	
    aScopeInit[0].FilterFlags.Uplevel.flBothModes = DSOP_FILTER_USERS|DSOP_FILTER_DOMAIN_LOCAL_GROUPS_DL|DSOP_FILTER_DOMAIN_LOCAL_GROUPS_SE|DSOP_FILTER_GLOBAL_GROUPS_DL|DSOP_FILTER_GLOBAL_GROUPS_SE|DSOP_FILTER_UNIVERSAL_GROUPS_DL|DSOP_FILTER_UNIVERSAL_GROUPS_SE;
    aScopeInit[0].FilterFlags.flDownlevel = DSOP_DOWNLEVEL_FILTER_USERS;
	
    // Initialize the DSOP_INIT_INFO structure.
    ZeroMemory(&InitInfo, sizeof(InitInfo));
	
    InitInfo.cbSize = sizeof(InitInfo);
    // Target is the local computer.
    InitInfo.pwzTargetComputer = wszTargetComputer;
    InitInfo.cDsScopeInfos = sizeof(aScopeInit) / 
		sizeof(DSOP_SCOPE_INIT_INFO);
    InitInfo.aDsScopeInfos = aScopeInit;
    InitInfo.flOptions = DSOP_FLAG_MULTISELECT;
	LPCWSTR attribs[] = {L"mail"};
	InitInfo.cAttributesToFetch = 1;
	InitInfo.apwzAttributeNames = attribs;
	
    /*
    Initialize can be called multiple times, but only the last 
    call has effect. Be aware that object picker makes its own
    copy of InitInfo. 
    */
    return pDsObjectPicker->Initialize(&InitInfo);
}

HRESULT FUtility::ProcessSelectedObjects(IDataObject* pdo, CString& selUser)
{
	if(NULL == pdo)
    {
        return E_INVALIDARG;
    }
    
    HRESULT     hr;
    STGMEDIUM   stm;
    FORMATETC   fe;
	
    // Get the global memory block that contain the user's selections.
    fe.cfFormat = 
		RegisterClipboardFormat(CFSTR_DSOP_DS_SELECTION_LIST);
    fe.ptd = NULL;
    fe.dwAspect = DVASPECT_CONTENT;
    fe.lindex = -1;
    fe.tymed = TYMED_HGLOBAL;
	
    hr = pdo->GetData(&fe, &stm);
    if(SUCCEEDED(hr))
    {
        PDS_SELECTION_LIST pDsSelList = NULL;
		
        // Retrieve a pointer to DS_SELECTION_LIST structure.
        pDsSelList = (PDS_SELECTION_LIST)GlobalLock(stm.hGlobal);
        if(NULL != pDsSelList)
        {
            ULONG i;
			
            // Loop through DS_SELECTION array of selected objects.
            for (i = 0; i < pDsSelList->cItems; i++) 
            {
				
				if (pDsSelList->aDsSelection[i].pvarFetchedAttributes->vt == VT_BSTR)
				{
					//m_strUserEmail = pDsSelList->aDsSelection[i].pvarFetchedAttributes->bstrVal;
					if(!selUser.IsEmpty())
						selUser += L";";
					selUser += pDsSelList->aDsSelection[i].pvarFetchedAttributes->bstrVal;
				}
				else if (pDsSelList->aDsSelection[i].pvarFetchedAttributes->vt == VT_EMPTY)
				{
					WideString wsNoEmail;
					MultiLanguage::FLoadString(IDS_STRING_NO_EMAIL_USER, wsNoEmail);
					WideString wsWarning;
					FSWideStringFormat(wsWarning, FSWideStringCastToLPCWSTR(wsNoEmail), pDsSelList->aDsSelection[i].pwzName);
					MultiLanguage::FShowMessageBox(
						(LPCWSTR)FSWideStringCastToLPCWSTR(wsWarning),
						NULL,
						MB_OK | MB_ICONINFORMATION,
						NULL
					);
					break;
				}

			}
            GlobalUnlock(stm.hGlobal);	
        }
        else
        {
            hr = E_POINTER;
        }
		
        ReleaseStgMedium(&stm);
    }
    return hr;
}




BOOL FUtility::IsThemeEnabled()
{
	HMODULE hinstDll;
	BOOL XPStyle = FALSE;
	bool (__stdcall *pIsAppThemed)();
	bool (__stdcall *pIsThemeActive)();

	// Test if operating system has themes enabled
	hinstDll = (HMODULE)FRAppLoadLibrary((FS_LPCWSTR)L"UxTheme.dll",FR_DIRECTORY_PATH_SYSTEM);
	if (hinstDll)
	{
		(FARPROC&)pIsAppThemed = ::GetProcAddress(hinstDll, "IsAppThemed");
		(FARPROC&)pIsThemeActive = ::GetProcAddress(hinstDll,"IsThemeActive");
		::FreeLibrary(hinstDll);
		if (pIsAppThemed != NULL && pIsThemeActive != NULL)
		{
			if (pIsAppThemed() && pIsThemeActive())
			{
				// Test if application has themes enabled by loading the proper DLL
				hinstDll = (HMODULE)FRAppLoadLibrary((FS_LPCWSTR)L"comctl32.dll",FR_DIRECTORY_PATH_SYSTEM);
				if (hinstDll)
				{
					DLLGETVERSIONPROC pDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hinstDll, "DllGetVersion");
					if (pDllGetVersion != NULL)
					{
						DLLVERSIONINFO dvi;
						ZeroMemory(&dvi, sizeof(dvi));
						dvi.cbSize = sizeof(dvi);
						HRESULT hRes = pDllGetVersion ((DLLVERSIONINFO *) &dvi);
						if (SUCCEEDED(hRes))
							XPStyle = dvi.dwMajorVersion >= 6;
					}
					::FreeLibrary(hinstDll);
				}
			}
		}
	}
	return XPStyle;
}


LRESULT FUtility::EnableWindowTheme(HWND hwnd, LPCWSTR app, LPCWSTR idlist)
{

	HMODULE hinstDll;
	HRESULT (__stdcall *pSetWindowTheme)(HWND, LPCWSTR, LPCWSTR);
	HANDLE (__stdcall *pOpenThemeData)(HWND, LPCWSTR);
	HRESULT (__stdcall *pCloseThemeData)(HANDLE);

	hinstDll = (HMODULE)FRAppLoadLibrary((FS_LPCWSTR)L"UxTheme.dll",FR_DIRECTORY_PATH_SYSTEM);
	if (hinstDll)
	{
		(FARPROC&)pOpenThemeData = ::GetProcAddress(hinstDll, "OpenThemeData");
		(FARPROC&)pCloseThemeData = ::GetProcAddress(hinstDll, "CloseThemeData");
		(FARPROC&)pSetWindowTheme = ::GetProcAddress(hinstDll, "SetWindowTheme");
		::FreeLibrary(hinstDll);
		if (pSetWindowTheme && pOpenThemeData && pCloseThemeData)
		{
			HANDLE theme = pOpenThemeData(hwnd,L"ListView");
			if (theme!=NULL)
			{
				pCloseThemeData(theme);
				return pSetWindowTheme(hwnd, app, idlist);
			}
		}
	}
	return S_FALSE;

}



BOOL FUtility::IsDirectoryOrFileExists(LPCWSTR wszPath)
{
	if(NULL == wszPath)
		return FALSE;

	BOOL bFind = FALSE;

	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA win32FindData;

	if(INVALID_HANDLE_VALUE != (hFind = ::FindFirstFile(wszPath, &win32FindData)))
	{
		bFind = TRUE;
		FindClose(hFind);
	}

	return bFind;
}

BOOL FUtility::IsPluginHadBeenDisabled(LPCWSTR pluginName)
{

	if (nullptr == pluginName)
		return FALSE;
	CString strSection = L"Installed\\";
	strSection += pluginName;
	int nRes = FRAppRegistryGetInt((FS_LPCWSTR)(LPCWSTR)strSection, L"LoadBehavior");
	if ((nRes & REG_PI_LOADBEHAVIOR_CONNECTED) == REG_PI_LOADBEHAVIOR_CONNECTED)
		return FALSE;
	else
		return TRUE;
}


/*
param:
		wszDirectoryPath: pattern = x:\xxx\aaa   or pattern = x:\xxx\aaa\
return -1: not a valid directory
		0: valid directory but not contains pdf file
		1:valid directory and contains pdf file
*/
int FUtility::IsDirectoryContainsPDFFile(LPCWSTR wszDirectoryPath, BOOL bSubDirectoryIncluding)
{
	if(NULL == wszDirectoryPath)
		return -1;


	CString strDirectoryPath = wszDirectoryPath;
	CString strRightOne = strDirectoryPath.Right(1);
	if(strRightOne == L"\\" || strRightOne == L"/")
		strDirectoryPath.Delete(strDirectoryPath.GetLength() - 1);



	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA win32FindData;

	hFind = ::FindFirstFile(LPCWSTR(strDirectoryPath), &win32FindData);

	if(INVALID_HANDLE_VALUE == hFind)
		return -1;

	if(!(FILE_ATTRIBUTE_DIRECTORY & win32FindData.dwFileAttributes))
	{
		::FindClose(hFind);
		return -1;
	}
	else
	{
		::FindClose(hFind);
	}

	BOOL bContainsFile = FALSE;

	CString strPDFFindPath = strDirectoryPath + L"\\*.pdf";

	HANDLE hPDFFind = INVALID_HANDLE_VALUE;
	hPDFFind = ::FindFirstFile(LPCWSTR(strPDFFindPath), &win32FindData);

	if(INVALID_HANDLE_VALUE != hPDFFind)
	{
		::FindClose(hPDFFind);
		return 1;
	}

	if(bSubDirectoryIncluding)
	{
		HANDLE hSubDirFind = INVALID_HANDLE_VALUE;

		CString strAllFilesPattern = strDirectoryPath + L"\\*";

		hSubDirFind = ::FindFirstFile(LPCWSTR(strAllFilesPattern), &win32FindData);
		if(INVALID_HANDLE_VALUE == hSubDirFind)
			return 0;

		if(FILE_ATTRIBUTE_DIRECTORY & win32FindData.dwFileAttributes)
		{
			if(0 != _wcsicmp(win32FindData.cFileName, L".") && 0 != _wcsicmp(win32FindData.cFileName, L".."))
			{
				CString strSubDir = strDirectoryPath + L"\\";
				strSubDir += win32FindData.cFileName;
				if(1 == FUtility::IsDirectoryContainsPDFFile(LPCWSTR(strSubDir), bSubDirectoryIncluding))
				{
					::FindClose(hSubDirFind);
					return 1;
				}
			}
		}


		int nResult = 0;
		while(::FindNextFile(hSubDirFind, &win32FindData))
		{
			if(FILE_ATTRIBUTE_DIRECTORY & win32FindData.dwFileAttributes)
			{
				if(0 != _wcsicmp(win32FindData.cFileName, L".") && 0 != _wcsicmp(win32FindData.cFileName, L".."))
				{
					CString strSubDir = strDirectoryPath + L"\\";
					strSubDir += win32FindData.cFileName;
					if(1 == FUtility::IsDirectoryContainsPDFFile(LPCWSTR(strSubDir), bSubDirectoryIncluding))
					{
						nResult = 1;
						break;
					}
				}
			}
		}
		::FindClose(hSubDirFind);
		return nResult;
	}

	return 0;
}


int FUtility::IsFileExists(LPCWSTR wszFilePath)
{
	if(NULL == wszFilePath)
		return 0;

	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA win32FindData;

	hFind = ::FindFirstFile(LPCWSTR(wszFilePath), &win32FindData);

	if(INVALID_HANDLE_VALUE == hFind)
		return 0;
	else
	{
		::FindClose(hFind);
		return 1;
	}


	return 0;
}



BOOL FUtility::GetUnofficialTemplatesDirectory(CString& strDirectory)
{
	FS_WideString fswsTemp = FSWideStringNew();
	if(!FRAppGetAppDataPath(&fswsTemp))
	{
		FSWideStringDestroy(fswsTemp);
		return FALSE;
	}
	CString strAppDataPath = (LPCWSTR)FSWideStringCastToLPCWSTR(fswsTemp);
	FSWideStringDestroy(fswsTemp);

	strAppDataPath.TrimRight(L"/\\");
	int nIndex = strAppDataPath.ReverseFind(L'\\');
	CString strRMSDataPath = strAppDataPath.Left(nIndex);
	strRMSDataPath += L"\\RMS";

	::CreateDirectoryW(LPCWSTR(strRMSDataPath), NULL);

	strRMSDataPath += L"\\UnofficialTemplates";

	::CreateDirectoryW(LPCWSTR(strRMSDataPath), NULL);

	strDirectory = strRMSDataPath;

	return TRUE;
}

CString FUtility::GetRMSExtensionConfigInfoFilePath()
{
	FS_WideString fswsTemp = FSWideStringNew();
	if(!FRAppGetAppDataPath(&fswsTemp))
	{
		FSWideStringDestroy(fswsTemp);
		return CString(L"");
	}
	CString strAppDataPath = (LPCWSTR)FSWideStringCastToLPCWSTR(fswsTemp);
	FSWideStringDestroy(fswsTemp);

	strAppDataPath.TrimRight(L"/\\");
	int nIndex = strAppDataPath.ReverseFind(L'\\');
	CString strRMSDataPath = strAppDataPath.Left(nIndex);
	strRMSDataPath += L"\\RMS";
	::CreateDirectoryW(LPCWSTR(strRMSDataPath), NULL);
	strRMSDataPath += L"\\Configuration";
	::CreateDirectoryW(LPCWSTR(strRMSDataPath), NULL);
	strRMSDataPath += L"\\RMSSetting.xml";

	return strRMSDataPath;
}


BOOL FUtility::SendEmail(LPCWSTR wszEmailAddress)
{
	if(NULL == wszEmailAddress)
		return FALSE;

	ByteString fsbsEmail;
	FSByteStringFromUnicode((FS_LPWSTR)wszEmailAddress, -1, fsbsEmail.GetAddr());


	HMODULE hMod = (HMODULE)FRAppLoadLibrary((FS_LPCWSTR)L"MAPI32.dll",FR_DIRECTORY_PATH_SYSTEM);
	if (hMod == NULL)
	{	
		return FALSE;
	}	
	//get the address of the function sending email
	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	(FARPROC&)lpfnSendMail = GetProcAddress(hMod, "MAPISendMail");	
	if (lpfnSendMail == NULL)
	{
		FreeLibrary(hMod);
		return FALSE;
	}

	
	vector<char*> vecEachEamil;
	
	int nStart = 0, nFind = -1;
	while(1)
	{
		nStart = nFind + 1;
		nFind = FSByteStringFind2(fsbsEmail, ';', nStart);
		if (nFind != -1)
		{
			if (nStart == nFind)
			{
				continue;
			}
			LPSTR temp = new char[nFind - nStart + 1];
			memset(temp, 0, sizeof(char) * (nFind - nStart + 1));
			strncpy(temp, (char*)FSByteStringCastToLPCSTR(fsbsEmail) + nStart, nFind - nStart);
			vecEachEamil.push_back(temp);
			//delete [] temp;
		}
		else
		{
			LPSTR temp = new char[FSByteStringGetLength(fsbsEmail) - nStart + 1];
			memset(temp, 0, sizeof(char) * (FSByteStringGetLength(fsbsEmail) - nStart + 1));
			strcpy(temp, (char*)FSByteStringCastToLPCSTR(fsbsEmail) + nStart);
			vecEachEamil.push_back(temp);
			//delete [] temp;
			break;
		}		
	}

	
	
	MapiRecipDesc *recip = new MapiRecipDesc[vecEachEamil.size()];
	for (size_t i = 0; i < vecEachEamil.size(); ++i)
	{
		memset(&recip[i], 0, sizeof(MapiRecipDesc));
				
		//recip[i].lpszAddress	= vecEachEamil[i];		//"@";
		recip[i].lpszName = vecEachEamil[i];
		recip[i].ulRecipClass  = MAPI_TO;
		recip[i].ulReserved = 0;
	}

		
		   	
	

	//int nPerson = 1;
	//if(((BYTE)GetVersion()) > 5) nPerson = 0;//vista
	// email structure information
	MapiMessage message;
	memset(&message, 0, sizeof(message));
	message.nFileCount	= 0;						  //number of file
	message.lpFiles		= NULL;						 //information of file
	message.nRecipCount  = vecEachEamil.size();	    //number of recipient
	message.lpRecips 	= recip;				   //recipient
	message.lpszSubject	= NULL;					  //the subject

	int nError = lpfnSendMail(0, 0,	&message, MAPI_LOGON_UI|MAPI_DIALOG, 0);
	if (nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT  
		&& nError != MAPI_E_LOGIN_FAILURE)
	{
		FS_WideString fswsEmailError = FSWideStringNew();
		MultiLanguage::FLoadString(IDS_STRING_SEND_MAIL_FAILE, fswsEmailError);
		MultiLanguage::FShowMessageBox(
			(LPCWSTR)FSWideStringCastToLPCWSTR(fswsEmailError),
			NULL,
			MB_OK | MB_ICONWARNING,
			NULL
			);
		FSWideStringDestroy(fswsEmailError);		
	}
		
	
	for (size_t i = 0; i < vecEachEamil.size(); i++)
	{
		delete [] vecEachEamil[i];
	}
	delete [] recip;

	FreeLibrary(hMod);
	
	return TRUE;
}



BOOL FUtility::UTCSysTime2LocalSysTime(LPSYSTEMTIME pstUTC, LPSYSTEMTIME pstLocal)
{
	if(NULL == pstUTC || NULL == pstLocal)
		return FALSE;

	FILETIME ftUTC;
	FILETIME ftLocal;

	::SystemTimeToFileTime(pstUTC, &ftUTC);
	::FileTimeToLocalFileTime(&ftUTC, &ftLocal);
	return ::FileTimeToSystemTime(&ftLocal, pstLocal);
}



BOOL FUtility::GetGuidString(FS_ByteString bsGuid)
{
	HRESULT hr = S_OK;
	UINT uGUIDLength = 0;
	GUID guid;

	hr = ::CoCreateGuid(&guid);
	if(FAILED(hr))
		return FALSE;

	WCHAR wszBuffer[128];
	memset(wszBuffer, 0, 128 * sizeof(WCHAR));

	if(0 == ::StringFromGUID2(guid,wszBuffer, 128))
		return FALSE;

	FSByteStringFromUnicode((FS_LPWSTR)wszBuffer, -1, &bsGuid);
	return TRUE;
}


BOOL FUtility::CenterWindowAccordingParentWnd(HWND hParentWnd, HWND hChildWnd, int nAdjustTopPos /* = 0 */)
{
	if(NULL == hParentWnd || NULL == hChildWnd)
		return FALSE;

	RECT desktopWndRect;
	RECT parentWndRect;
	RECT childWndRect;

	::GetWindowRect(::GetDesktopWindow(), &desktopWndRect);
	::GetWindowRect(hParentWnd, &parentWndRect);
	::GetWindowRect(hChildWnd, &childWndRect);

	POINT parentWndCenterScreenPos;
	parentWndCenterScreenPos.x = parentWndRect.right - (parentWndRect.right - parentWndRect.left) / 2;
	parentWndCenterScreenPos.y = parentWndRect.bottom - (parentWndRect.bottom - parentWndRect.top) / 2;

	int nChildWidth = childWndRect.right - childWndRect.left;
	int nChildHeight = childWndRect.bottom - childWndRect.top;

	POINT lefttopPos;
	POINT rightbottomPos;
	lefttopPos.x = parentWndCenterScreenPos.x - nChildWidth / 2;
	lefttopPos.y = parentWndCenterScreenPos.y - nChildHeight / 2;
	rightbottomPos.x = lefttopPos.x + nChildWidth;
	rightbottomPos.y = lefttopPos.y + nChildHeight;

	if(lefttopPos.x < desktopWndRect.left)
	{
		lefttopPos.x = desktopWndRect.left;
		rightbottomPos.x = lefttopPos.x + nChildWidth;
	}
	if (lefttopPos.y < desktopWndRect.top)
	{
		lefttopPos.y = desktopWndRect.top;
		rightbottomPos.y = lefttopPos.y + nChildHeight + nAdjustTopPos;
	}
	/*if(rightbottomPos.x > desktopWndRect.right)
	{
		rightbottomPos.x = desktopWndRect.right;
		lefttopPos.x = rightbottomPos.x - nChildWidth;
	}
	if(rightbottomPos.y > desktopWndRect.bottom)
	{
		rightbottomPos.y = desktopWndRect.bottom;
		lefttopPos.y = rightbottomPos.y - nChildHeight + nAdjustTopPos;
	}*/
	
// 	::ScreenToClient(hParentWnd, &lefttopPos);
// 	::ScreenToClient(hParentWnd, &rightbottomPos);

	::SetWindowPos(hChildWnd,
					NULL,
					lefttopPos.x,
					lefttopPos.y,
					nChildWidth,
					nChildHeight,
					SWP_NOZORDER | SWP_NOACTIVATE);

	return TRUE;
}



FS_BOOL FUtility::OnFileReadBlock(FS_LPVOID clientData, void* buffer, FS_DWORD offset, FS_DWORD size)
{
	PNGCLIENTDATA* pClientData = (PNGCLIENTDATA*)clientData;
	memcpy(buffer, pClientData->pData+offset, size);	
	return TRUE;
}

FS_DWORD FUtility::OnFileGetSize(FS_LPVOID clientData)
{
	PNGCLIENTDATA* pClientData = (PNGCLIENTDATA*)clientData;
	return pClientData->dwSize;
}

void FUtility::OnFileRelease(FS_LPVOID clientData)
{
	PNGCLIENTDATA* pClientData = (PNGCLIENTDATA*)clientData;
	delete pClientData;
}

FS_DIBitmap FUtility::LoadFSDIBitmapFromPNG(UINT uID)
{
	/*AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HINSTANCE hInstOld=AfxGetResourceHandle();
	AfxSetResourceHandle(FUIInteracting::GetInstanceHandle());
	HRSRC hRes = ::FindResource(FUIInteracting::GetInstanceHandle(), MAKEINTRESOURCE(uID), _T("PNG"));
	if (hRes == NULL) return NULL;
	DWORD dwLength = ::SizeofResource(FUIInteracting::GetInstanceHandle(), hRes);
	HGLOBAL hGlobal = ::LoadResource(FUIInteracting::GetInstanceHandle(), hRes);
	if (hGlobal == NULL) return NULL;
	LPVOID pData = ::LockResource(hGlobal);
	if (pData == NULL) return NULL;
	AfxSetResourceHandle(hInstOld);

	PNGCLIENTDATA* pClientData = new PNGCLIENTDATA;
	memset(pClientData, 0, sizeof(PNGCLIENTDATA));
	pClientData->pData = (BYTE*)pData;
	pClientData->dwSize = dwLength;

	memset(&fsFileReader, 0, sizeof(FS_FileRead));
	FUtility::fsFileReader.lStructSize = sizeof(FS_FileRead);
	FUtility::fsFileReader.FSFileRead_ReadBlock = &FUtility::OnFileReadBlock;
	FUtility::fsFileReader.FSFileRead_GetSize = &FUtility::OnFileGetSize;
	FUtility::fsFileReader.FSFileRead_Release = &FUtility::OnFileRelease;
	FUtility::fsFileReader.clientData = (LPVOID)pClientData;

	FS_FileReadHandler fileHandler = FSFileReadHandlerNew(FUtility::fsFileReader);
	FS_DIBitmap bmp = NULL;
	bmp = FSDIBitmapLoadFromPNGIcon2(fileHandler);
	FSFileReadHandlerDestroy(fileHandler);
	return bmp;*/

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return FSDIBitmapLoadFromPNGIcon3(FUIInteracting::GetInstanceHandle(), (FS_LPCWSTR)MAKEINTRESOURCE(uID));
}



FUtility::HostAppType FUtility::GetHostAppType()
{
#ifdef FXNET_FRD_PLUS//#17101-FXNET-HCM 领鲜版区分产品
	return FUtility::emFOXIT_READER_PLUS;
#endif
	FUtility::HostAppType hostAppType;

	ByteString fsbsAppName;
#ifdef FXNET_FPM_PER
	ByteString bsPhantom((FS_LPCSTR)"Foxit Phantom");
#else
	ByteString bsPhantom((FS_LPCSTR)"Foxit PhantomPDF");
#endif

	FRAppGetName(fsbsAppName.GetAddr());

	if(FSByteStringEqualNoCase(fsbsAppName, bsPhantom))
		hostAppType = FUtility::emFOXIT_PHANTOMPDF;
	else
		hostAppType = FUtility::emFOXIT_READER;
	
	return hostAppType;
}

void FUtility::GetGPOSubKey(FS_WideString wszGPOSubKey)
{

	FSWideStringFill(wszGPOSubKey, (FS_LPCWSTR)L"Software\\Policies\\");
	FUtility::HostAppType hostAppType = FUtility::GetHostAppType();
	if (emFOXIT_READER == hostAppType)
	{		
		FSWideStringConcat2(wszGPOSubKey, (FS_LPCWSTR)L"Foxit\\Reader");		
	}
#ifdef FXNET//#17101-FXNET-HCM 领鲜版区分产品
	else if (emFOXIT_READER_PLUS == hostAppType)
	{
		FSWideStringConcat2(wszGPOSubKey, (FS_LPCWSTR)L"Foxit\\Reader Plus");
	}
#endif
	else
	{
#ifdef FXNET_FPM_PER
		FSWideStringConcat2(wszGPOSubKey, (FS_LPCWSTR)L"Foxit\\Phantom");
#else
		FSWideStringConcat2(wszGPOSubKey, (FS_LPCWSTR)L"Foxit\\PhantomPDF");
#endif
	}	
	FSWideStringConcat2(wszGPOSubKey, (FS_LPCWSTR)L"\\RMS");

	FSWideStringConcat2(wszGPOSubKey, (FS_LPCWSTR)L"\\Preferences");
	FS_LPCWSTR lpwsGPOSubKey = FSWideStringCastToLPCWSTR(wszGPOSubKey);
	
#ifdef TEMP_TEST_CODE
	wprintf(L"The GPO sub key is %s\n", FSWideStringCastToLPCWSTR(wszGPOSubKey));
#endif
}

void FUtility::GetLogOrPrinterLimitSubKey(CString& wsSubKey, BOOL bPrinter /* = FALSE */)
{	
	wsSubKey = L"Software\\Policies\\";
	FUtility::HostAppType hostAppType = FUtility::GetHostAppType();
	if (emFOXIT_READER == hostAppType)
	{
		wsSubKey += L"Foxit\\Reader";
	}
#ifdef FXNET//#17101-FXNET-HCM 领鲜版区分产品
	else if (emFOXIT_READER_PLUS == hostAppType)
	{
		wsSubKey += L"Foxit\\Reader Plus";
	}
#endif
	else
	{
#ifdef FXNET_FPM_PER
		wsSubKey += L"Foxit\\Phantom";
#else
		wsSubKey += L"Foxit\\PhantomPDF";
#endif
	}
	wsSubKey += L"\\RMS";
	if (bPrinter)
	{
		wsSubKey += L"\\Printer";
	}
	else
		wsSubKey += L"\\AuditLog";
}

inline bool FUtility::ResolveName( Outlook::_NameSpacePtr spNS, CString& strInput )
{
	if(!spNS)
		return false;

	try
	{
		Outlook::RecipientPtr spRecipient = spNS->CreateRecipient(strInput.GetBuffer(0));
		if (spRecipient)
		{
			spRecipient->Resolve();
			if (spRecipient->Resolved)
			{
				Outlook::AddressEntryPtr spAddEntry = spRecipient->AddressEntry;
				if (spAddEntry)
				{
					Outlook::_ExchangeUserPtr spExchangeUser = spAddEntry->GetExchangeUser();
					_bstr_t strAddr("");
					if (spExchangeUser != NULL)
						strAddr = spExchangeUser->GetPrimarySmtpAddress();
					else
					{
						Outlook::_ExchangeDistributionListPtr spExchangeDistList = 
							spAddEntry->GetExchangeDistributionList();
						if(spExchangeDistList != NULL)
							strAddr = spExchangeDistList->GetPrimarySmtpAddress();
						else
							strAddr = spAddEntry->GetAddress();
					}
					CString strResolvedAddr((LPWSTR)strAddr);
					if (strResolvedAddr != _T(""))
					{
						strInput = strResolvedAddr; 
						return true;
					}
				}				
			}			
		}
		return false;

	}
	catch (_com_error &err)
	{
#ifdef DEBUG
		AfxMessageBox(err.ErrorMessage());
#endif
		TRACE(L"Outlook throws the error: %s\n", err.ErrorMessage());
		TRACE(L"Description: %s\n", (LPCWSTR) err.Description());
		return false;
	}	
}

void FUtility::ParseInput( CString& csIuput, std::list<CString>& lstNames )
{
	int nPosEnd = csIuput.Find( L";");

	if (nPosEnd != -1)
	{
		int nPosBegin = 0;
		do 
		{
			CString name = csIuput.Mid(nPosBegin, nPosEnd - nPosBegin);
			Trim(name);
			if (name != _T(""))
				lstNames.push_back(name);

			nPosBegin = nPosEnd + 1;
			nPosEnd = csIuput.Find( L";",nPosBegin);

			if (nPosEnd == -1 && nPosBegin != csIuput.GetLength())
			{
				name = csIuput.Mid(nPosBegin, 
					csIuput.GetLength() - nPosBegin);
				Trim(name);
				if (name != _T(""))
					lstNames.push_back(name);
			}
		} while(nPosEnd != -1);
	}
	else
	{
		if (csIuput.GetLength())
		{
			Trim(csIuput);
			if (csIuput != _T(""))
				lstNames.push_back(csIuput);
		}
	}
}

bool FUtility::ResolveAllNames( Outlook::_ApplicationPtr spOutlookApp, 
							   std::list<CString>& lstNames, 
							   V_RESOLVE_RESULTS& resolveResults, HWND parentHwnd /*= NULL*/ )
{
	ASSERT(parentHwnd);
	if(!spOutlookApp)
		return false;

	if (FUtility::s_LResolutionFailed == 1) //assume resolution succeed
	{
		if (::InterlockedExchange(&FUtility::s_LResolutionFailed, 0) != 1)
		{
			return false;
		}	
	}	

	try
	{
		Outlook::_NameSpacePtr spNS = spOutlookApp->GetNamespace(L"MAPI");
		if(!spNS)
			return false;

		std::list<CString>::size_type size = lstNames.size();
		for ( std::list<CString>::size_type i = 0; i != size; i++ )
		{
		
			bool bResolved = ResolveName(spNS, lstNames.front());
			NAMERESOLVE_RESULT result = {_T(""), false};
			result.csName = lstNames.front();
			result.bResolved = bResolved;

			AcquireSRWLockExclusive(&s_srwlock);

			resolveResults.push_back(result);

			ReleaseSRWLockExclusive(&s_srwlock);
			WakeAllConditionVariable(&s_cvReadyToPrint);

			lstNames.pop_front();
		}

	}
	catch (_com_error &err)
	{
		if (err.Error( ) == 0x800706ba)
		{
			FS_WideString fswsErrorOutlook = FSWideStringNew();
			MultiLanguage::FLoadString(IDS_STRING_OUTLOOKEXCEPTION, fswsErrorOutlook);
			MultiLanguage::FShowMessageBox(
				(LPCWSTR)FSWideStringCastToLPCWSTR(fswsErrorOutlook),
				NULL,
				MB_OK | MB_ICONERROR,
				parentHwnd
				);
			FSWideStringDestroy(fswsErrorOutlook);
		}

		TRACE(L"Outlook throws the error: %s\n", err.ErrorMessage());
		TRACE(L"Description: %s\n", (LPCWSTR) err.Description());
		return false;
	}	

	return true;
}

void FUtility::MaintainScrollBarVisible(HWND hWnd)
{
	if(NULL == hWnd)
		return;

	int cyScreen = 0, cyCaptionHeight = 0;
	//cyScreen = ::GetSystemMetrics(SM_CYSCREEN);
	cyCaptionHeight = ::GetSystemMetrics(SM_CYCAPTION);
	RECT workAreaRect;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &workAreaRect, 0);
	cyScreen = workAreaRect.bottom - workAreaRect.top;

	// get character height
	HDC hDC = ::GetDC(hWnd);
	TEXTMETRIC tm;
	::GetTextMetrics(hDC, &tm);
	::ReleaseDC(hWnd, hDC);

	RECT rect;
	::GetWindowRect(hWnd, &rect);
	int cyWindow = rect.bottom - rect.top;

	// calculate line height and line number
	int nLineHeight = tm.tmHeight + tm.tmExternalLeading;
	int nLineNumber = cyWindow / nLineHeight;

	if(cyWindow > cyScreen)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.nPage = (cyScreen - cyCaptionHeight) / nLineHeight;
		si.nMin = 0;
		si.nMax = nLineNumber;
		si.nPos = 0;
		si.fMask = SIF_ALL;
		::SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		::ShowScrollBar(hWnd, SB_VERT, TRUE);
	}
	else
		::ShowScrollBar(hWnd, SB_VERT, FALSE);
}


HWND FUtility::GetWindowHandleByPID(DWORD dwProcessID)
{
	HWND h = GetTopWindow(0);
	while ( h )
	{
		DWORD pid = 0;
		DWORD dwTheardId = GetWindowThreadProcessId( h,&pid);

		if (dwTheardId != 0)
		{
			if ( pid == dwProcessID/*your process id*/ )
			{
				// here h is the handle to the window
				return h;
			}
		}


		h = GetNextWindow( h , GW_HWNDNEXT);
	}

	return NULL;
}

void FUtility::GetANSIBufferFromUnicode(char** pDes, const CString strSrc)
{
	int nSrcLen = 0;
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	*pDes = new char[nSrcLen + 1];
	memset((void*)(*pDes), 0, (nSrcLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_ACP, 0, strSrc, -1, *pDes, nSrcLen, NULL, NULL);
}

void FUtility::GetLocaleIPAddress(CString& strIP)
{
	char hostname[MAX_PATH] = {0};
	gethostname(hostname,MAX_PATH);                
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);
	if(lpHostEnt == NULL)
	{
		return;
	}
	// 取得IP地址列表中的第一个为返回的IP(因为一台主机可能会绑定多个IP)
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      

	// 将IP地址转化成字符串形式
	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);
	strIP = CString(inet_ntoa(inAddr));     
}

void FUtility::GetDocFileName(FR_Document frDoc, FS_WideString *pFileName)
{
	FRDocGetFilePath(frDoc, pFileName);
	FS_WCHAR wsDrv[MAX_PATH] = {0};
	FS_WCHAR wsDir[MAX_PATH] = {0};
	FS_WCHAR wsName[MAX_PATH] = {0};
	FS_WCHAR wsExt[MAX_PATH] = {0};
	_wsplitpath((LPCWSTR)FSWideStringCastToLPCWSTR(*pFileName) , (wchar_t *)wsDrv , (wchar_t *)wsDir , (wchar_t *)wsName , (wchar_t *)wsExt);
	FSWideStringEmpty(*pFileName);
	FSWideStringFill(*pFileName, wsName);
	FSWideStringConcat2(*pFileName, (FS_LPCWSTR)wsExt);
}

void FUtility::GetFileName(const LPCWSTR pFilePath, FS_WideString *pFileName)
{
	FS_WCHAR wsDrv[MAX_PATH] = {0};
	FS_WCHAR wsDir[MAX_PATH] = {0};
	FS_WCHAR wsName[MAX_PATH] = {0};
	FS_WCHAR wsExt[MAX_PATH] = {0};
	_wsplitpath(pFilePath , (wchar_t *)wsDrv , (wchar_t *)wsDir , (wchar_t *)wsName , (wchar_t *)wsExt);
	FSWideStringEmpty(*pFileName);
	FSWideStringFill(*pFileName, wsName);
	FSWideStringConcat2(*pFileName, (FS_LPCWSTR)wsExt);
}

BOOL FUtility::IsWestEURLanguage()
{
	UINT nLangID = 49001;
	nLangID = FRLanguageGetCurrentID();

	//目前，已知西欧7国语系，习惯使用‘,’作为小数点。
	if (nLangID == 49010 || /*Dutch*/
		nLangID == 49014 || /*French*/
		nLangID == 49016 || /*German*/
		nLangID == 49018 || /*Italian*/
		nLangID == 49023 || /*Polish*/
		nLangID == 49024 || /*Portuguese*/
		nLangID == 49025 || /*Portuguese_Brazilian*/
		nLangID == 49027 || /*Russian*/
		nLangID == 49039 || /*Spanish_Latin American*/
		nLangID == 49009 ||   /*Danish*/
		nLangID == 49013 ||   /*Finnish*/
		nLangID == 49022 ||   /*Norwegian*/
		nLangID == 49030    /*Swedish*/
		)
	{
		return TRUE;
	}
	return FALSE;
}
void FUtility::GetFormatSymbolString(LPWSTR str, BOOL bInput)
{
	if (NULL == str) return;

	WCHAR chDot = L'.';

	//目前，已知西欧7国语系，习惯使用‘,’作为小数点。
	if (FUtility::IsWestEURLanguage())
	{
		chDot = L',';
	}

	while (*str)
	{
		if (*str == L'.' || *str == L',')
		{
			*str = bInput ? L'.' : chDot;
		}
		++str;
	}
}

//0066402: r167369,OCR/Bates Numbering操作，弹出RMS相应验证提示后直接关闭选择文件对话框及主程序，会停止工作
HWND FUtility::GetParentWnd()
{
	HWND hParentWnd = FRAppGetMainFrameWnd();
	BOOL bEnabled = ::IsWindowEnabled(hParentWnd);
	if (!bEnabled)
	{
		CWnd* pActive = CWnd::FromHandle(hParentWnd)->GetActiveWindow();
		if (pActive)
		{
			hParentWnd = pActive->GetSafeHwnd();
		}
	}
	return hParentWnd;
}

CString FUtility::GetFileVersion(const CString &strFilePath)
{
	CString szResult(_T("0.0.0.0"));
	if (strFilePath.GetLength() > 0 && PathFileExists(strFilePath))  
	{
		VS_FIXEDFILEINFO *pVerInfo = NULL;
		DWORD dwTemp, dwSize;
		BYTE *pData = NULL;
		UINT uLen;

		dwSize = GetFileVersionInfoSize(strFilePath, &dwTemp);
		if (dwSize == 0)
		{
			return szResult;
		}

		pData = new BYTE[dwSize+1];
		if (pData == NULL)
		{
			return szResult;
		}

		if (!GetFileVersionInfo(strFilePath, 0, dwSize, pData))
		{
			delete[] pData;
			return szResult;
		}

		if (!VerQueryValue(pData, L"\\", (void **)&pVerInfo, &uLen))
		{  
			delete[] pData;
			return szResult;
		}

		DWORD verMS = pVerInfo->dwFileVersionMS;
		DWORD verLS = pVerInfo->dwFileVersionLS;
		DWORD major = HIWORD(verMS);
		DWORD minor = LOWORD(verMS);
		DWORD build = HIWORD(verLS);
		DWORD revision = LOWORD(verLS);
		delete[] pData;

		szResult.Format(TEXT("%d.%d.%d.%d"), major, minor, build, revision);
	}  

	return szResult;
}

CString FUtility::GenerateTempWrapperFilePath()
{
	WCHAR tempPath[MAX_PATH] = {0};
	GetTempPath(MAX_PATH, tempPath);
	WCHAR fileName[MAX_PATH];
	::GetTempFileName(tempPath, L"fox", 0, fileName);
	CString strPath = fileName;
	return strPath;
}
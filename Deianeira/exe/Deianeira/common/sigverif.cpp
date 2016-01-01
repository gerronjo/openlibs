#include "sigverif.h"


//检测文件是否签名
//lpFileName:目标文件路径
//返回:true:有签名，false:没有签名
bool sigverif_check(LPCWSTR lpFileName)
{    
	WINTRUST_DATA wd = { 0 };   
	WINTRUST_FILE_INFO wfi = { 0 };   
	WINTRUST_CATALOG_INFO wci = { 0 };   
	CATALOG_INFO ci = { 0 };   

	HCATADMIN hCatAdmin = NULL;   

	if (!CryptCATAdminAcquireContext(&hCatAdmin, NULL, 0))   
	{   
		return false;   
	}   

	HANDLE hFile = CreateFileW(lpFileName, GENERIC_READ, FILE_SHARE_READ,   
		NULL, OPEN_EXISTING, 0, NULL);   
	if (INVALID_HANDLE_VALUE == hFile)   
	{   
		CryptCATAdminReleaseContext(hCatAdmin, 0);   
		return false;   
	}   

	DWORD dwCnt = 100;   
	BYTE byHash[100];   

	CryptCATAdminCalcHashFromFileHandle(hFile, &dwCnt, byHash, 0);   
	CloseHandle(hFile);   

	LPWSTR pszMemberTag = new WCHAR[dwCnt * 2 + 1];   
	for (DWORD dw = 0; dw < dwCnt; ++dw)   
	{   
		wsprintfW(&pszMemberTag[dw * 2], L"%02X", byHash[dw]);   
	}   

	HCATINFO hCatInfo = CryptCATAdminEnumCatalogFromHash(hCatAdmin,   
		byHash, dwCnt, 0, NULL);  

	if (NULL==hCatInfo)   
	{   
		wfi.cbStruct       = sizeof(WINTRUST_FILE_INFO);   
		wfi.pcwszFilePath  = lpFileName; 
		wfi.hFile          = NULL;   
		wfi.pgKnownSubject = NULL;   

		wd.cbStruct            = sizeof(WINTRUST_DATA);   
		wd.dwUnionChoice       = WTD_CHOICE_FILE;   
		wd.pFile               = &wfi;   
		wd.dwUIChoice          = WTD_UI_NONE;   
		wd.fdwRevocationChecks = WTD_REVOKE_NONE;   
		wd.dwStateAction       = WTD_STATEACTION_IGNORE;   
		wd.dwProvFlags         = WTD_SAFER_FLAG;   
		wd.hWVTStateData       = NULL;   
		wd.pwszURLReference    = NULL;   
	}   
	else   
	{   
		CryptCATCatalogInfoFromContext(hCatInfo, &ci, 0);   
		wci.cbStruct             = sizeof(WINTRUST_CATALOG_INFO);   
		wci.pcwszCatalogFilePath = ci.wszCatalogFile;   
		wci.pcwszMemberFilePath  = lpFileName;   
		wci.pcwszMemberTag       = pszMemberTag;   

		wd.cbStruct            = sizeof(WINTRUST_DATA);   
		wd.dwUnionChoice       = WTD_CHOICE_CATALOG;   
		wd.pCatalog            = &wci;   
		wd.dwUIChoice          = WTD_UI_NONE;   
		wd.fdwRevocationChecks = WTD_STATEACTION_VERIFY;   
		wd.dwProvFlags         = 0;   
		wd.hWVTStateData       = NULL;   
		wd.pwszURLReference    = NULL;   
	}   

	GUID action = WINTRUST_ACTION_GENERIC_VERIFY_V2;   
	HRESULT hr  = WinVerifyTrust(NULL, &action, &wd);   
	

	delete[] pszMemberTag;
	CryptCATAdminReleaseContext(hCatAdmin, 0); 

	if(SUCCEEDED(hr))
	{
		return true;
	}

	return false;
}  


//获得数字签名厂商
//pCertContext:签名上下文
//返回:数据签名厂商名称，否则为NULL
LPWSTR sigverif_printCertificateInfo(PCCERT_CONTEXT pCertContext)
{
	DWORD dwData;
	TCHAR* tcVerifyComp;

	__try
	{
		// Get Subject name size.
		if (!(dwData = CertGetNameString(pCertContext, 
			CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			NULL,
			0)))
		{
			return NULL;
		}

		// Allocate memory for subject name.
		tcVerifyComp = (TCHAR*)LocalAlloc(LPTR, dwData * sizeof(TCHAR));
		if (!tcVerifyComp)
		{
			return NULL;
		}

		// Get subject name.
		if (!(CertGetNameString(pCertContext, 
			CERT_NAME_SIMPLE_DISPLAY_TYPE,
			0,
			NULL,
			tcVerifyComp,
			dwData)))
		{
			return NULL;
		};
	
	}
	__finally
	{
		if (pCertContext)
		{
			CertFreeCertificateContext(pCertContext);
		}
	}

	return tcVerifyComp;
}


//获得数字签名厂商
//lpFileName:目标文件路径
//返回:数据签名厂商名称，否则为NULL
LPWSTR sigverif_getCompanyName(LPCWSTR lpFileName)
{
	WCHAR szFileName[MAX_PATH]; 
	HCERTSTORE hStore = NULL;
	HCRYPTMSG hMsg = NULL; 
	PCCERT_CONTEXT pCertContext = NULL;
	BOOL fResult;   
	DWORD dwEncoding, dwContentType, dwFormatType;
	PCMSG_SIGNER_INFO pSignerInfo = NULL;
	PCMSG_SIGNER_INFO pCounterSignerInfo = NULL;
	DWORD dwSignerInfo;
	CERT_INFO CertInfo;     
	SPROG_PUBLISHERINFO ProgPubInfo;

	ZeroMemory(&ProgPubInfo, sizeof(ProgPubInfo));
	//复制文件名到缓冲区
	lstrcpy(szFileName,lpFileName);

	// Get message handle and store handle from the signed file.
	fResult = CryptQueryObject(CERT_QUERY_OBJECT_FILE,
		szFileName,
		CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
		CERT_QUERY_FORMAT_FLAG_BINARY,
		0,
		&dwEncoding,
		&dwContentType,
		&dwFormatType,
		&hStore,
		&hMsg,
		NULL);

	if (!fResult)
	{
		return NULL;
	}

	// Get signer information size.
	fResult = CryptMsgGetParam(hMsg, 
		CMSG_SIGNER_INFO_PARAM, 
		0, 
		NULL, 
		&dwSignerInfo);

	if (!fResult)
	{
		return NULL;
	}

	// Allocate memory for signer information.
	pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, dwSignerInfo);

	if (!pSignerInfo)
	{
		return NULL;
	}

	// Get Signer Information.
	fResult = CryptMsgGetParam(hMsg, 
		CMSG_SIGNER_INFO_PARAM, 
		0, 
		(PVOID)pSignerInfo, 
		&dwSignerInfo);

	if (!fResult)
	{
		return NULL;
	}

	// Search for the signer certificate in the temporary 
	// certificate store.
	CertInfo.Issuer = pSignerInfo->Issuer;
	CertInfo.SerialNumber = pSignerInfo->SerialNumber;

	pCertContext = CertFindCertificateInStore(hStore,
		ENCODING,
		0,
		CERT_FIND_SUBJECT_CERT,
		(PVOID)&CertInfo,
		NULL);

	if (!pCertContext)
	{
		return NULL;
	}

	return sigverif_printCertificateInfo(pCertContext); 
}	


//获得详细信息
//pSignerInfo：
//Info:
//返回:true:成功，false:失败
bool sigverif_getProgAndPublisherInfo(PCMSG_SIGNER_INFO pSignerInfo,PSPROG_PUBLISHERINFO Info)
{
	bool bRet = false;
	PSPC_SP_OPUS_INFO OpusInfo = NULL;  
	DWORD dwData;
	BOOL fResult;

	__try
	{
		// Loop through authenticated attributes and find
		// SPC_SP_OPUS_INFO_OBJID OID.
		for (DWORD n = 0; n < pSignerInfo->AuthAttrs.cAttr; n++)
		{           
			if (lstrcmpA(SPC_SP_OPUS_INFO_OBJID, 
				pSignerInfo->AuthAttrs.rgAttr[n].pszObjId) == 0)
			{
				// Get Size of SPC_SP_OPUS_INFO structure.
				fResult = CryptDecodeObject(ENCODING,
					SPC_SP_OPUS_INFO_OBJID,
					pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
					pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
					0,
					NULL,
					&dwData);
				if (!fResult)
				{
					__leave;
				}

				// Allocate memory for SPC_SP_OPUS_INFO structure.
				OpusInfo = (PSPC_SP_OPUS_INFO)LocalAlloc(LPTR, dwData);
				if (!OpusInfo)
				{
					__leave;
				}

				// Decode and get SPC_SP_OPUS_INFO structure.
				fResult = CryptDecodeObject(ENCODING,
					SPC_SP_OPUS_INFO_OBJID,
					pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].pbData,
					pSignerInfo->AuthAttrs.rgAttr[n].rgValue[0].cbData,
					0,
					OpusInfo,
					&dwData);
				if (!fResult)
				{
					__leave;
				}

				// Fill in Program Name if present.
				if (OpusInfo->pwszProgramName)
				{
					Info->lpszProgramName = OpusInfo->pwszProgramName;
				}
				else
				{
					Info->lpszProgramName = NULL;
				}

				// Fill in Publisher Information if present.
				if (OpusInfo->pPublisherInfo)
				{

					switch (OpusInfo->pPublisherInfo->dwLinkChoice)
					{
					case SPC_URL_LINK_CHOICE:
						Info->lpszPublisherLink = OpusInfo->pPublisherInfo->pwszUrl;
						break;

					case SPC_FILE_LINK_CHOICE:
						Info->lpszPublisherLink = OpusInfo->pPublisherInfo->pwszFile;
						break;

					default:
						Info->lpszPublisherLink = NULL;
						break;
					}
				}
				else
				{
					Info->lpszPublisherLink = NULL;
				}

				// Fill in More Info if present.
				if (OpusInfo->pMoreInfo)
				{
					switch (OpusInfo->pMoreInfo->dwLinkChoice)
					{
					case SPC_URL_LINK_CHOICE:
						Info->lpszMoreInfoLink = OpusInfo->pMoreInfo->pwszUrl;
						break;

					case SPC_FILE_LINK_CHOICE:
						Info->lpszMoreInfoLink = OpusInfo->pMoreInfo->pwszFile;
						break;

					default:
						Info->lpszMoreInfoLink = NULL;
						break;
					}
				}               
				else
				{
					Info->lpszMoreInfoLink = NULL;
				}

				bRet = true;

				break;
			}                
		} 
	}
	__finally
	{
		if (OpusInfo != NULL)
		{
			LocalFree(OpusInfo); 
		}
	}

	return bRet;
}	 
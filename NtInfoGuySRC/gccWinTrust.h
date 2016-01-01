#if !defined(GCCWINTRUST_H)
	#define GCCWINTRUST_H
	
	#define __field_ecount(x)
	typedef HANDLE HCATADMIN;
	typedef HANDLE HCATINFO;
	
	#define WINTRUST_ACTION_GENERIC_VERIFY_V2 \
            {0xaac56b,\
              0xcd44,\
              0x11d0,\
              {0x8c,0xc2,0x0,0xc0,0x4f,0xc2,0x95,0xee} \
            }
	
	typedef struct CATALOG_INFO_
	{
		DWORD cbStruct;   // set to sizeof(CATALOG_INFO)
		WCHAR wszCatalogFile[MAX_PATH];
	} CATALOG_INFO;
	
	typedef struct _WINTRUST_DATA
	{
		DWORD cbStruct;
		LPVOID pPolicyCallbackData;
		LPVOID pSIPClientData;
		DWORD dwUIChoice;
		#define WTD_UI_ALL 1
		#define WTD_UI_NONE 2
		#define WTD_UI_NOBAD 3
		#define WTD_UI_NOGOOD 4
		DWORD fdwRevocationChecks;
		#define WTD_REVOKE_NONE 0x00000000
		#define WTD_REVOKE_WHOLECHAIN 0x00000001
		DWORD dwUnionChoice;
		#define WTD_CHOICE_FILE 1
		#define WTD_CHOICE_CATALOG 2
		#define WTD_CHOICE_BLOB 3
		#define WTD_CHOICE_SIGNER 4
		#define WTD_CHOICE_CERT 5
		union _noname
		{
			struct WINTRUST_FILE_INFO_      *pFile;         // individual file
			struct WINTRUST_CATALOG_INFO_   *pCatalog;      // member of a Catalog File
			struct WINTRUST_BLOB_INFO_      *pBlob;         // memory blob
			struct WINTRUST_SGNR_INFO_      *pSgnr;         // signer structure only
			struct WINTRUST_CERT_INFO_      *pCert;
		}noname;
		DWORD dwStateAction;
		#define WTD_STATEACTION_IGNORE           0x00000000
		#define WTD_STATEACTION_VERIFY           0x00000001
		#define WTD_STATEACTION_CLOSE            0x00000002
		#define WTD_STATEACTION_AUTO_CACHE       0x00000003
		#define WTD_STATEACTION_AUTO_CACHE_FLUSH 0x00000004
		HANDLE hWVTStateData;
		WCHAR *pwszURLReference;
		// 17-Feb-1998 philh: added
		DWORD dwProvFlags;
		#define WTD_PROV_FLAGS_MASK                      0x0000FFFF
		#define WTD_USE_IE4_TRUST_FLAG                   0x00000001
		#define WTD_NO_IE4_CHAIN_FLAG                    0x00000002
		#define WTD_NO_POLICY_USAGE_FLAG                 0x00000004
		#define WTD_REVOCATION_CHECK_NONE                0x00000010
		#define WTD_REVOCATION_CHECK_END_CERT            0x00000020
		#define WTD_REVOCATION_CHECK_CHAIN               0x00000040
		#define WTD_REVOCATION_CHECK_CHAIN_EXCLUDE_ROOT  0x00000080
		#define WTD_SAFER_FLAG                           0x00000100
		#define WTD_HASH_ONLY_FLAG                       0x00000200
		#define WTD_USE_DEFAULT_OSVER_CHECK              0x00000400
		#define WTD_LIFETIME_SIGNING_FLAG                0x00000800
		#define WTD_CACHE_ONLY_URL_RETRIEVAL             0x00001000 // affects CRL retrieval and AIA retrieval
		// 17-Dec-2004 JSchwart: re-added to fix build break in other depots
		DWORD dwUIContext;
		#define WTD_UICONTEXT_EXECUTE 0
		#define WTD_UICONTEXT_INSTALL 1
	}WINTRUST_DATA, *PWINTRUST_DATA;
	
	typedef struct WINTRUST_FILE_INFO_
	{
		DWORD cbStruct;
		LPCWSTR pcwszFilePath;
		HANDLE hFile;
		// 09-Dec-1997 pberkman: added
		GUID *pgKnownSubject;
	} WINTRUST_FILE_INFO, *PWINTRUST_FILE_INFO;
	
	typedef struct WINTRUST_CATALOG_INFO_
	{
		DWORD           cbStruct;               // = sizeof(WINTRUST_CATALOG_INFO)
		DWORD           dwCatalogVersion;       // optional: Catalog version number
		LPCWSTR         pcwszCatalogFilePath;   // required: path/name to Catalog file
		LPCWSTR         pcwszMemberTag;         // optional: tag to member in Catalog
		LPCWSTR         pcwszMemberFilePath;    // required: path/name to member file
		HANDLE          hMemberFile;            // optional: open handle to pcwszMemberFilePath
		// 30-Oct-1997 pberkman: added
		__field_ecount(cbCalculatedFileHash) BYTE *pbCalculatedFileHash;  // optional: pass in the calculated hash
		DWORD           cbCalculatedFileHash;   // optional: pass in the count bytes of the calc hash
		// 15-Jan-1998 pberkman: added
		PCCTL_CONTEXT   pcCatalogContext;       // optional: pass in to use instead of CatalogFilePath.
	}WINTRUST_CATALOG_INFO, *PWINTRUST_CATALOG_INFO;
	
	extern bool WINAPI CryptCATAdminAcquireContext(\
		OUT HCATADMIN *phCatAdmin,IN const GUID *pgSubsystem,\
		IN DWORD dwFlags);
		
	extern BOOL WINAPI CryptCATAdminReleaseContext(IN HCATADMIN hCatAdmin,\
		IN DWORD dwFlags);
		
	extern BOOL WINAPI CryptCATAdminCalcHashFromFileHandle(IN HANDLE hFile,\
		IN OUT DWORD *pcbHash,OUT OPTIONAL BYTE *pbHash,IN DWORD dwFlags);
		
	extern HCATINFO WINAPI CryptCATAdminEnumCatalogFromHash(IN HCATADMIN hCatAdmin,\
		IN BYTE *pbHash,IN DWORD cbHash,IN DWORD dwFlags,\
		IN OUT HCATINFO *phPrevCatInfo);
		
	extern BOOL WINAPI CryptCATCatalogInfoFromContext(IN HCATINFO hCatInfo,\
		IN OUT CATALOG_INFO *psCatInfo,IN DWORD dwFlags);
		
	extern BOOL WINAPI CryptCATAdminReleaseCatalogContext(IN HCATADMIN hCatAdmin,\
		IN HCATINFO hCatInfo,IN DWORD dwFlags);
	extern LONG WINAPI WinVerifyTrust(HWND hwnd, GUID *pgActionID,\
		LPVOID pWVTData);
#endif

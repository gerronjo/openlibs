#include "StdAfx.h"
#include "MyDriver.h"

#pragma warning( disable: 4996 )

MyDriver::MyDriver(void)
{
	driverHandle = NULL;
	   
	removable = TRUE;

	driverName = NULL;
	driverPath = NULL;
	driverDosName = NULL;

	initialized = FALSE;
	loaded = FALSE;
	started = FALSE;
}


MyDriver::~MyDriver(void)
{
	if(driverHandle != NULL)
	{
		CloseHandle(driverHandle); 
		driverHandle = NULL; 
	}
   
    UnloadDriver();
}

void MyDriver::SetRemovable(BOOL value)
{
	removable = value;
}


BOOL MyDriver::IsInitialized(void)
{
	return initialized;
}

BOOL MyDriver::IsLoaded(void)
{
	return loaded;
}

BOOL MyDriver::IsStarted(void)
{
	return started;
}
void MyDriver:: OpenDevice(LPCTSTR lpszDevicePath)
{
	

	// 打开设备
	driverHandle = CreateFile(lpszDevicePath,     // 设备路径
		GENERIC_READ | GENERIC_WRITE,        // 读写方式
		FILE_SHARE_READ | FILE_SHARE_WRITE,  // 共享方式
		NULL,                                // 默认的安全描述符
		OPEN_EXISTING,                       // 创建方式
		0,                                   // 不需设置文件属性
		NULL);                               // 不需参照模板文件


}

DWORD MyDriver::IniMyDriver(char * path)
{
	if(initialized)
	{
		if(UnloadDriver() != DRV_SUCCESS)
			return DRV_ERROR_ALREADY_INITIALIZED;
	}

	driverPath = (char *)malloc(strlen(path) + 1);

	if(driverPath == NULL)
		return DRV_ERROR_MEMORY;

	strcpy(driverPath, path);





	char * sPos1 = strrchr(driverPath, (int)'\\');

	
	if (sPos1 == NULL)
		sPos1 = driverPath;


	
	char * sPos2 = strrchr(sPos1, (int)'.');

	
	if (sPos2 == NULL || sPos1 > sPos2)
	{
		free(driverPath);
		driverPath = NULL;

		return DRV_ERROR_INVALID_PATH_OR_FILE;
	}
	
	
	driverName = (char *) malloc (sPos2 - sPos1);
	
	if(driverName == NULL)
	{
		free(driverPath);
		driverPath = NULL;

		return DRV_ERROR_MEMORY;
	}

	
	memcpy(driverName, sPos1 + 1, sPos2 - sPos1 - 1);
	
	driverName[sPos2 - sPos1 - 1] = 0;

	
	driverDosName = (char *) malloc (strlen(driverName) + 5);

	if(driverDosName == NULL)
	{
		free(driverPath);
		driverPath = NULL;

		free(driverName);
		driverName = NULL;

		return DRV_ERROR_MEMORY;
	}

	sprintf(driverDosName, "\\\\.\\%s", driverName);

		
	initialized = TRUE;

	return DRV_SUCCESS;
}


DWORD MyDriver::IniMyDriver(char * name, char * path, char * dosName)
{	
	if(initialized)
	{
		if(UnloadDriver() != DRV_SUCCESS)
			return DRV_ERROR_ALREADY_INITIALIZED;
	}

	char * dirBuffer;

	if (path != NULL) 
	{
		DWORD len = (DWORD)(strlen(name) + strlen(path) + 1);
		dirBuffer = (char *) malloc (len);

		if(dirBuffer == NULL)
			return DRV_ERROR_MEMORY;

		strcpy(dirBuffer, path);

	}

	else 
	{
		char * pathBuffer;
        DWORD len = GetCurrentDirectory(0, NULL);
      
		pathBuffer = (char *) malloc (len);

		if(pathBuffer == NULL)
			return DRV_ERROR_MEMORY;

		        
        if (GetCurrentDirectoryA(len, pathBuffer) != 0) 
		{
			len = (DWORD)(strlen(pathBuffer) + strlen(name) + 6);
			dirBuffer = (char *) malloc (len);

			if(dirBuffer == NULL)
			{
				free(pathBuffer);

				return DRV_ERROR_MEMORY;
			}

			
			sprintf(dirBuffer, "%s\\%s.sys", pathBuffer, name);

			
			if(GetFileAttributesA(dirBuffer) == 0xFFFFFFFF)
			{
				free(pathBuffer);
				free(dirBuffer);

				
				char * sysDriver = "\\system32\\Drivers\\";
				char * sysPath;
	    	    
				DWORD len = GetWindowsDirectory(NULL, 0);
     			sysPath = (char *) malloc (len + strlen(sysDriver));

				if(sysPath == NULL)
					return DRV_ERROR_MEMORY;

				if (GetWindowsDirectoryA(sysPath, len) == 0) 
				{
					free(sysPath);
					
					return DRV_ERROR_UNKNOWN;
				}
	
		
				strcat(sysPath, sysDriver);
				len = (DWORD)(strlen(sysPath) + strlen(name) + 5);

				dirBuffer = (char *) malloc (len);

				if(dirBuffer == NULL)
					return DRV_ERROR_MEMORY;

				sprintf(dirBuffer, "%s%s.sys", sysPath, name);

				free(sysPath);


				if(GetFileAttributesA(dirBuffer) == 0xFFFFFFFF)
				{
					free(dirBuffer);

					return DRV_ERROR_INVALID_PATH_OR_FILE;
				}
			}

			free(pathBuffer);

        }

		else
		{
			free(pathBuffer);

			return DRV_ERROR_UNKNOWN;
		}
	}
	

	driverPath = dirBuffer;

	driverName = (char *)malloc(strlen(name) + 1);

	if(driverName == NULL)
	{
		free(driverPath);
		driverPath = NULL;
		
		return DRV_ERROR_MEMORY;
	}

	strcpy(driverName, name);
	
	char * auxBuffer;
	if(dosName != NULL)
        auxBuffer = dosName;
	
	else
		auxBuffer = name;

	if(auxBuffer[0] != '\\' && auxBuffer[1] != '\\')
	{
		driverDosName = (char *) malloc (strlen(auxBuffer) + 5);

		if(driverDosName == NULL)
		{
			free(driverPath);
			driverPath = NULL;

			free(driverName);
			driverName = NULL;

			return DRV_ERROR_MEMORY;
		}

		sprintf(driverDosName, "\\\\.\\%s", auxBuffer);
	}

	else
	{
		driverDosName = (char *) malloc (strlen(auxBuffer));

		if(driverDosName == NULL)
		{
			free(driverPath);
			driverPath = NULL;

			free(driverName);
			driverName = NULL;

			return DRV_ERROR_MEMORY;
		}

		strcpy(driverDosName, auxBuffer);
	}


	initialized = TRUE;

	return DRV_SUCCESS;
}



DWORD MyDriver::LoadDriver(char * name, char * path, char * dosName, BOOL start)
{

	DWORD retCode = IniMyDriver(name, path, dosName);


	if(retCode == DRV_SUCCESS)
		retCode = LoadDriver(start);

	return retCode;
}


DWORD MyDriver::LoadDriver(char * path, BOOL start)
{
	
	DWORD retCode = IniMyDriver(path);


	if(retCode == DRV_SUCCESS)
		retCode = LoadDriver(start);

	return retCode;
}


DWORD MyDriver::LoadDriver(BOOL start)
{
	
	if(loaded)
		return DRV_SUCCESS;

	if(!initialized)
		return DRV_ERROR_NO_INITIALIZED;

	
	SC_HANDLE SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	DWORD retCode = DRV_SUCCESS;
	
	if (SCManager == NULL) 
		return DRV_ERROR_SCM;

    


    SC_HANDLE  SCService = CreateServiceA(SCManager,			  // SCManager database
									     driverName,            // nombre del servicio
							    		 driverName,            // nombre a mostrar
										 SERVICE_ALL_ACCESS,    // acceso total
										 SERVICE_KERNEL_DRIVER, // driver del kernel
										 SERVICE_DEMAND_START,  // comienzo bajo demanda
										 SERVICE_ERROR_NORMAL,  // control de errores normal
										 driverPath,	          // path del driver
										 NULL,                  // no pertenece a un grupo
										 NULL,                  // sin tag
										 NULL,                  // sin dependencias
										 NULL,                  // cuenta local del sistema
										 NULL                   // sin password
										 );
    
	if (SCService == NULL) 
	{
		SCService = OpenServiceA(SCManager, driverName, SERVICE_ALL_ACCESS);
		
		if (SCService == NULL) 
			retCode = DRV_ERROR_SERVICE;
	}

    CloseServiceHandle(SCService);
	SCService=NULL;

	CloseServiceHandle(SCManager);
	SCManager = NULL;

	
	if(retCode == DRV_SUCCESS)
	{
		loaded = TRUE;

		if(start)
			retCode = StarMyDriver();
	}

	return retCode;
}



DWORD MyDriver::UnloadDriver(BOOL forceClearData)
{
	DWORD retCode = DRV_SUCCESS;


	if (started)
	{
		if ((retCode = StopDriver()) == DRV_SUCCESS) 
		{
			
			if(removable)
			{
				
				SC_HANDLE SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
				
				if (SCManager == NULL) 
					return DRV_ERROR_SCM;

				SC_HANDLE SCService = OpenServiceA(SCManager, driverName, SERVICE_ALL_ACCESS);
				
				if (SCService != NULL)
				{
					if(!DeleteService(SCService))
						retCode = DRV_ERROR_REMOVING;
					else
						retCode = DRV_SUCCESS;
				}

				else
					retCode = DRV_ERROR_SERVICE;

				CloseServiceHandle(SCService);
				SCService = NULL;

				CloseServiceHandle(SCManager);
				SCManager = NULL;

				
				if(retCode == DRV_SUCCESS)
					loaded = FALSE;
			}
		}
	}

	
	if(initialized) 
	{
		
		if(retCode != DRV_SUCCESS && forceClearData == FALSE)
			return retCode;
		

		initialized = FALSE;
				

		if(driverPath != NULL)
		{
			free(driverPath);
			driverPath = NULL;
		}


		if(driverDosName != NULL)
		{
			free(driverDosName);
			driverDosName = NULL;
		}

		if(driverName != NULL)
		{
			free(driverName);
			driverName = NULL;
		}

	}

	return retCode;
}




DWORD MyDriver::StarMyDriver(void)
{

	if(started)
		return DRV_SUCCESS;


	SC_HANDLE SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	DWORD retCode;
	
	if (SCManager == NULL) 
		return DRV_ERROR_SCM;

    SC_HANDLE SCService = OpenServiceA(SCManager,
		                              (LPCSTR)driverName,
				                      SERVICE_ALL_ACCESS);
    
	if (SCService == NULL) 
        return DRV_ERROR_SERVICE;

    
    if (!StartService( SCService, 0, NULL)) 
	{
		
	
        if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING) 
		{
			removable = FALSE;

			retCode = DRV_SUCCESS;
		}

		else
			retCode = DRV_ERROR_STARTING;
    }

	else
		retCode = DRV_SUCCESS;

  
    CloseServiceHandle(SCService);
	SCService = NULL;

	CloseServiceHandle(SCManager);
	SCManager = NULL;

	
	if(retCode == DRV_SUCCESS)
	{
		started = TRUE;

		retCode = OpenDevice();
	}

    return retCode;
}




DWORD MyDriver::StopDriver(void)
{
	
	if(!started)
		return DRV_SUCCESS;


	SC_HANDLE SCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	DWORD retCode;
	
	if (SCManager == NULL) 
		return DRV_ERROR_SCM;

   
    SERVICE_STATUS  status;

    SC_HANDLE SCService = OpenServiceA(SCManager, driverName, SERVICE_ALL_ACCESS);
    
	if (SCService != NULL)
	{
		
		CloseHandle(driverHandle); 
		driverHandle = NULL; 

		if(!ControlService(SCService, SERVICE_CONTROL_STOP, &status))
			retCode = DRV_ERROR_STOPPING;

		else
			retCode = DRV_SUCCESS;
	}

	else
		retCode = DRV_ERROR_SERVICE;


    CloseServiceHandle(SCService);
	SCService = NULL;

	CloseServiceHandle(SCManager);
	SCManager = NULL;


	if(retCode == DRV_SUCCESS)
		started = FALSE;

    return retCode;
}



DWORD MyDriver::OpenDevice(void)
{

	if (driverHandle != NULL) 
		CloseHandle(driverHandle);

    driverHandle = CreateFileA(driverDosName,
							  GENERIC_READ | GENERIC_WRITE,
							  0,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);


    if(driverHandle == INVALID_HANDLE_VALUE)
		return DRV_ERROR_INVALID_HANDLE;
	
	return DRV_SUCCESS;
}



HANDLE MyDriver::GeMyDriverHandle(void)
{
	return driverHandle;
}



DWORD MyDriver::WriteIo(DWORD code, PVOID buffer, DWORD count)
{
	if(driverHandle == NULL)
	{
		AfxMessageBox(L"WriteIo Fail");
		return DRV_ERROR_INVALID_HANDLE;
	}
	DWORD bytesReturned;

	BOOL returnCode = DeviceIoControl(driverHandle,
								      code,
								      buffer,
								      count,
								      NULL,
								      0,
								      &bytesReturned,
								      NULL);
	//GetLastError();

	if(!returnCode)
		return DRV_ERROR_IO;

	return DRV_SUCCESS;
}


DWORD MyDriver::ReadIo(DWORD code, PVOID buffer, DWORD count)
{
	if(driverHandle == NULL)
	{
		AfxMessageBox(L"ReadIo Fail");
		return DRV_ERROR_INVALID_HANDLE;
	}

	DWORD bytesReturned;
	BOOL retCode = DeviceIoControl(driverHandle,
								   code,
								   NULL,
								   0,
								   buffer,
								   count,
								   &bytesReturned,
								   NULL);

	if(!retCode)
		return DRV_ERROR_IO;

	return bytesReturned;
}



DWORD MyDriver::RawIo(DWORD code, PVOID inBuffer, DWORD inCount, PVOID outBuffer, DWORD outCount)
{
	if(driverHandle == NULL)
	{
		AfxMessageBox(L"RawIo Fail");
		return DRV_ERROR_INVALID_HANDLE;
	}
	DWORD bytesReturned;
	BOOL retCode = DeviceIoControl(driverHandle,
								   code,
								   inBuffer,
								   inCount,
								   outBuffer,
								   outCount,
								   &bytesReturned,
								   NULL);

	if(!retCode)
		return DRV_ERROR_IO;

	return bytesReturned;
}
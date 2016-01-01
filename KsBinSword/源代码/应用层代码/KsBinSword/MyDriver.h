
// #if !defined(MyDriver_CLASS)
// #define MyDriver_CLASS
// 
 #pragma once
// 
 #include "winsvc.h"


//ERROR CODES
#define DRV_SUCCESS						 (DWORD)0		// Todo bien

#define DRV_ERROR_SCM					 (DWORD)-1		// ERROR al abrir el service manager
#define DRV_ERROR_SERVICE				 (DWORD)-2		// ERROR al crear el servicio
#define DRV_ERROR_MEMORY				 (DWORD)-3		// ERROR al reservar memoria
#define DRV_ERROR_INVALID_PATH_OR_FILE	 (DWORD)-4		// ERROR, Path no valido
#define DRV_ERROR_INVALID_HANDLE		 (DWORD)-5		// ERROR, driver handle no valido
#define DRV_ERROR_STARTING				 (DWORD)-6		// ERROR al poner en Start el driver
#define DRV_ERROR_STOPPING				 (DWORD)-7		// ERROR al parar el driver
#define DRV_ERROR_REMOVING				 (DWORD)-8		// ERROR eliminando el "servicio"
#define DRV_ERROR_IO					 (DWORD)-9		// ERROR en operacion de E/S
#define DRV_ERROR_NO_INITIALIZED		 (DWORD)-10		// ERROR, clase no inicializada
#define DRV_ERROR_ALREADY_INITIALIZED	 (DWORD)-11		// ERROR, clase ya inicializada
#define DRV_ERROR_NULL_POINTER			 (DWORD)-12		// ERROR, puntero a null como parametro
#define DRV_ERROR_UNKNOWN				 (DWORD)-13		// ERROR desconocido



class MyDriver
{
public:
	MyDriver(void);		//constructor
	~MyDriver(void);		//destructor


	DWORD IniMyDriver(char * name, char * path, char * dosName=NULL);
	DWORD IniMyDriver(char * path);


	DWORD LoadDriver(BOOL start = TRUE);
	DWORD LoadDriver(char * name, char * path, char * dosName=NULL, BOOL start=TRUE);
	DWORD LoadDriver(char * path, BOOL start=TRUE);

	DWORD UnloadDriver(BOOL forceClearData = FALSE);
	
	DWORD StarMyDriver(void);
	DWORD StopDriver(void);

	void SetRemovable(BOOL value);

	BOOL IsInitialized();
	BOOL IsStarted();
	BOOL IsLoaded();


	HANDLE GeMyDriverHandle(void);

	DWORD WriteIo(DWORD code, PVOID buffer, DWORD count);
	DWORD ReadIo(DWORD code, PVOID buffer, DWORD count);
	DWORD RawIo(DWORD code, PVOID inBuffer, DWORD inCount, PVOID outBuffer, DWORD outCount);
	void  OpenDevice(LPCTSTR lpszDevicePath);
	HANDLE driverHandle;	
private:
	
	
	char * driverName;		
	char * driverPath;		
	char * driverDosName;	

	BOOL initialized;		
	BOOL started;
	BOOL loaded;
	BOOL removable;

	DWORD OpenDevice(void);	
};

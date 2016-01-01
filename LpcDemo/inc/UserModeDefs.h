
/*
this header file defines the things cannot find at SDK but we
wanna use at our demo in ring3
just like macros , structures , functions .etc
*/

#define NTSTATUS LONG
#define NTAPI __stdcall

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#define InitializeObjectAttributes( p, n, a, r, s ) { \
	(p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
	(p)->RootDirectory = r;                             \
	(p)->Attributes = a;                                \
	(p)->ObjectName = n;                                \
	(p)->SecurityDescriptor = s;                        \
	(p)->SecurityQualityOfService = NULL;               \
	}

typedef struct _UNICODE_STRING {
  USHORT  Length;
  USHORT  MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG  Length;
	HANDLE  RootDirectory;
	PUNICODE_STRING  ObjectName;
	ULONG  Attributes;
	PVOID  SecurityDescriptor;
	PVOID  SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
typedef CONST OBJECT_ATTRIBUTES *PCOBJECT_ATTRIBUTES;

typedef void (NTAPI* _InitUnicodeString)(
	IN OUT PUNICODE_STRING  DestinationString,
	IN PCWSTR  SourceString
	);

typedef struct _CLIENT_ID {
	ULONG UniqueProcessId;
	ULONG UniqueThreadId;
} CLIENT_ID,*PCLIENT_ID;


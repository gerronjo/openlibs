
PVOID GetUndocumentFunctionAdress(int FindWhat)
{
	
	ULONG size,index;
	PULONG buf;
	ULONG i;
	PSYSTEM_MODULE_INFORMATION module;
	PVOID driverAddress=0;
	ULONG ntosknlBase;
	ULONG ntosknlEndAddr;
	ULONG curAddr;
	NTSTATUS status;
	ULONG retAddr=0;
    ULONG code1_sp2,code2_sp2,code3_sp2,code4_sp2;

	switch (FindWhat)
	{
	case FindPspTerminateThreadByPointer:
         code1_sp2=0x8b55ff8b;code2_sp2=0x0cec83ec;code3_sp2=0xfff84d83;code4_sp2=0x7d8b5756;
		break;
	case FindPspTerminateProcess:
		 code1_sp2=0x8b55ff8b;code2_sp2=0xa16456ec;code3_sp2=0x00000124;code4_sp2=0x3b08758b;
		break;
	}
		
    
	ZwQuerySystemInformation(SystemModuleInformation,&size, 0, &size);
	if(NULL==(buf = (PULONG)ExAllocatePool(PagedPool, size)))
	{
		DbgPrint("failed alloc memory failed \n");
		return 0;
	}
	
	status=ZwQuerySystemInformation(SystemModuleInformation,buf, size , 0);
	if(!NT_SUCCESS( status ))
	{
		DbgPrint("failed query\n");
		return 0;
	}
	
	module = (PSYSTEM_MODULE_INFORMATION)(( PULONG )buf + 1);
	ntosknlEndAddr=(ULONG)module->Base+(ULONG)module->Size;
	ntosknlBase=(ULONG)module->Base;
	curAddr=ntosknlBase;
	ExFreePool(buf);
	
	for (i=curAddr;i<=ntosknlEndAddr;i++)
	{
		if ((*((ULONG *)i)==code1_sp2)&&(*((ULONG *)(i+4))==code2_sp2)&&(*((ULONG *)(i+8))==code3_sp2)&&(*((ULONG*)(i+12))==code4_sp2)) 
			
		{
			
			retAddr=i;
			DbgPrint("adress is:%x",retAddr); 
			return (PVOID)retAddr;
			
		}
	}
	return (PVOID)retAddr;
}

void KillThread(int Pid,int Tid)
{
	
    //全部定义为ULONG类型
    ULONG pTargetProcess;     //self explanatory
    ULONG pTargetThread;     //thread that can be either alerable or non-alertable
    ULONG pNotAlertableThread; //non-alertable thread
    ULONG pSystemProcess;     //May not necessarily be the 'System' process
    ULONG pTempThread;
    ULONG pNextEntry, pListHead, pThNextEntry,pThListHead; 
	PSPTERMINATETHREADBYPOINTER LPspTerminateThreadByPointer=NULL;
    DWORD dwPidOffset  = GetPlantformDependentInfo ( PROCESS_ID_OFFSET ) ;//0x084
    DWORD dwPNameOffset   = GetPlantformDependentInfo ( FILE_NAME_OFFSET ) ; //0x174 
	PEPROCESS eprocess = NULL;
	int bFindTid=0,address=0;
	//LPspTerminateProcess = (PSPTERMINATETPROCESS)address;
	LPspTerminateThreadByPointer=(PSPTERMINATETHREADBYPOINTER)GetUndocumentFunctionAdress(FindPspTerminateThreadByPointer);
	DbgPrint("ThreadBy:%x",LPspTerminateThreadByPointer);
	//DbgBreakPoint();
	if (!LPspTerminateThreadByPointer)
	{
		return;
	}
	if (dwPidOffset==0||dwPNameOffset==0)
	{
		DbgPrint("Get Offset Fail !");
		dwPidOffset=0x084;dwPNameOffset=0x174;
	}
	if (0==Pid||0==Tid)
	{
		return ;
	}

	PsLookupProcessByProcessId( Pid,&eprocess );
	pSystemProcess=(ULONG)eprocess;
	//获取线程列表头
	//+0x050 ThreadListHead   : _LIST_ENTRY
	//也就是_KPROCESS(PCB)中ThreadListHead的偏移地址
	pThListHead = pSystemProcess+0x50;
	//得到ETHREAD结构中_KTHREAD(Tcb)的+0x1b0 ThreadListEntry  : _LIST_ENTRY地址
	pThNextEntry=*(ULONG *)pThListHead;
	//Now we loop through it's threads, seeking an alertable thread
	while(pThNextEntry != pThListHead)
	{    //所属ETHREAD的首地址
		pTempThread =pThNextEntry-0x1b0;

		//线程ID
		//ETHREAD+0x1ec Cid : _CLIENT_ID为进程ID
		//再向下+4为线程ID
		DbgPrint("thread Id is %d\n",*(ULONG *)(pTempThread+0x1f0));
		if (*(ULONG *)(pTempThread+0x1f0)==Tid)
		{
     		LPspTerminateThreadByPointer( (PETHREAD)pTempThread, 0);
		}
					
		pNotAlertableThread =pTempThread;
					
		//下一个线程块
		pThNextEntry = *(ULONG *)pThNextEntry; //check next thread
	}

	

}

VOID KillPro(int pid)
{
	PKTHREAD pThread;
	PKPROCESS pKProc;
	PEPROCESS pProc;
	PVOID pNext;
	PKTHREAD pKThread;

	NTSTATUS status;
	PEPROCESS eprocess = NULL;
	UNICODE_STRING path;
	WCHAR path_buffer[256];
	WCHAR buffer[256];
	int index;
	unsigned char* p;
	unsigned char* pExtiTime;
    KAPC_STATE apcstate;
	int address=0;
	DbgPrint("KillPro!\n");
	if (pid<=0)
	{
		return;
	}
	address=(int)GetUndocumentFunctionAdress(FindPspTerminateProcess);
	if (!address)
	{
		return;
	}
	LPspTerminateProcess = (PSPTERMINATETPROCESS)address;
	RtlInitEmptyUnicodeString(&path,path_buffer,sizeof(path_buffer));
	//for (index = 12; index < 65535; index += 4)
    index=pid;
	{
		status = PsLookupProcessByProcessId( index,&eprocess );
		if (NT_SUCCESS(status)) 
		{
			pExtiTime = (unsigned char*)eprocess;
			pExtiTime = pExtiTime+0x78;
			if ( *pExtiTime == 0 )
			{
// 				p = (unsigned char*)eprocess;
// 				p = p+0x1f4;
// 				_asm 
// 				{
// 				    	push eax
// 						mov eax,DWORD PTR[p]
// 						mov eax,DWORD PTR[eax]
// 						mov p,eax
// 						pop eax
// 				}
//				RtlCopyUnicodeString(&path,(PUNICODE_STRING)p);
//				RtlCopyMemory(buffer,(unsigned char*)(p+8),path.MaximumLength);
				//if ( wcsstr(buffer,L"360Safe.exe") || wcsstr(buffer,L"safeboxTray.exe") || wcsstr(buffer,L"360tray.exe") || wcsstr(buffer,L"360safebox.exe"))
				if (1)
				{
					DbgPrint("Find !");
					DbgPrint("EPROCESS is:%08x\n",eprocess);
					DbgPrint("%ws\n",buffer);
					//KeStackAttachProcess((PRKPROCESS)eprocess,&apcstate);
					//status = ZwTerminateProcess(0,0);
					status = LPspTerminateProcess(eprocess,0);
					if (NT_SUCCESS(status))
						DbgPrint("PspTerminateProcess OK!");
					//KeUnstackDetachProcess(&apcstate);
					DbgPrint("Kill !");
				}
				else
				{
				}
			}
			else 
			{
				//DbgPrint("Exited Process!\n");
			}
		}
	}
	//---
}
PVOID GetDriverBaseAdress(void)
{
  NTSTATUS status;
  ULONG size, index;
  PULONG buf;
  PSYSTEM_MODULE_INFORMATION module;
  PVOID driverAddress = 0;
  char *sysname[256];
  char *hljname[256];
  ZwQuerySystemInformation(SystemModuleInformation, &size, 0, &size);

  if (NULL == (buf = (PULONG)ExAllocatePool(PagedPool, size)))
  {
    DbgPrint("failed alloc memory failed \n");
    return 0;
  }

  status = ZwQuerySystemInformation(SystemModuleInformation, buf, size, 0);
  if (!NT_SUCCESS(status))
  {
    DbgPrint("failed query\n");
    return 0;
  }

  module = (PSYSTEM_MODULE_INFORMATION)((PULONG)buf + 1);

  for (index = 0; index <  *buf; index++)
  {
    driverAddress = module[index].Base;
    DbgPrint("Module found at:%x\n", driverAddress);
    sysname[index] = module[index].ImageName + module[index].ModuleNameOffset;
    DbgPrint("Module found at:%s\n", sysname[index]);
    hljname[index] = module[index].ImageName;
    DbgPrint("imagename found at:%s\n", hljname[index]);
  }
  ExFreePool(buf);
  return driverAddress;
}



//通过搜索PsLookupProcessByProcessId函数，获取PspCidTable的地址
ULONG GetCidAddr()
{
	PUCHAR addr;
	PUCHAR p;
	UNICODE_STRING          pslookup;	
	ULONG cid;
	
	RtlInitUnicodeString (&pslookup,
		L"PsLookupProcessByProcessId");	
		addr = (PUCHAR) MmGetSystemRoutineAddress(&pslookup);//MmGetSystemRoutineAddress可以通过函数名获得函数地址
		for (p=addr;p<addr+PAGE_SIZE;p++)
		{
			if((*(PUSHORT)p==0x35ff)&&(*(p+6)==0xe8))
			{
				cid=*(PULONG)(p+2);
				return cid;
				break;
			}
		}
		return 0;
}

//通过当前进程获取进程对象的类型指针
ULONG GetProcessType()
{
	ULONG eproc;
	ULONG type;
	ULONG total;
	eproc=(ULONG)PsGetCurrentProcess();//PsGetCurrentProcess获取当前活动进程的地址，实际上就是对象（体）指针
	eproc=(ULONG)OBJECT_TO_OBJECT_HEADER(eproc);
	type=*(PULONG)(eproc+TYPE);
	return type;
}

//使用单向链表记录进程信息
VOID RecordInfo(ULONG i)
{
	ProcessInfo *r;
	//DbgPrint("pCid:%x,head:%x",pCid,head);
	if(head==NULL)
	{
		if((head=(ProcessInfo *)ExAllocatePool(NonPagedPool,sizeof(ProcessInfo)))==NULL)
		{
			return;
		}
		head->addr=0x0;
	}
	if (head->addr==0x0)
	{
		head->addr=i;
		pCid=head;
	}
	else
	{
		if((r=(ProcessInfo *)ExAllocatePool(NonPagedPool,sizeof(ProcessInfo)))==NULL)
		{
			return;
		}
		if (NULL==pCid)
		{
			DbgPrint("Null! pCid:%x,head:%x",pCid,head);
			return;
		}
		
		pCid->next=r;
		r->addr=i;
		r->next=NULL;
		pCid=r;
	}
}
//判断是否是进程对象，是则记录，不是则放弃
VOID IsValidProcess()
{
	ULONG PspCidTable;
	ULONG TableCode;
	ULONG table1,table2;
	ULONG object,objectheader;
	ULONG NextFreeTableEntry;
	ULONG processtype,type;
	ULONG flags;
	ULONG i;
	PspCidTable=GetCidAddr();
	processtype=GetProcessType();
	if(PspCidTable==0)
	{
		return ;
	}
	else
	{
		//TableCode的最后两位在XP中决定了句柄表的层数
		TableCode=*(PULONG)(*(PULONG)PspCidTable);
		if((TableCode&0x3)==0x0)
		{
			table1=TableCode;
			table2=0x0;
		}
		if((TableCode&0x3)==0x1)
		{
			TableCode=TableCode&0xfffffffc;
			table1=*(PULONG)TableCode;
			table2=*(PULONG)(TableCode+0x4);
		}
		//对cid从0x0到0x4e1c进行遍历
		for(i=0x0;i<0x4e1c;i++)
		{
			if(i<=0x800)
			{	
				if(MmIsAddressValid((PULONG)(table1+i*2)))
				{
					object=*(PULONG)(table1+i*2);
					if(MmIsAddressValid((PULONG)(table1+i*2+NEXTFREETABLEENTRY)))
					{
						NextFreeTableEntry=*(PULONG)(table1+i*2+NEXTFREETABLEENTRY);
				    	if(NextFreeTableEntry==0x0)//正常的_HANDLE_TABLE_ENTRY中NextFreeTableEntry为0x0
						{
					    	object=((object | 0x80000000)& 0xfffffff8);//转换为对象（体）指针
		    		    	objectheader=(ULONG)OBJECT_TO_OBJECT_HEADER(object);//获取对象（头）指针
			    	    	if(MmIsAddressValid((PULONG)(objectheader+TYPE)))
							{
					        	type=*(PULONG)(objectheader+TYPE);
				            	if(type==processtype)
								{
						    		flags=*(PULONG)((ULONG)object+FLAGS);
						    		if((flags&0xc)!=0xc)
						    		RecordInfo(object);//flags显示进程没有退出
								}
							}
						}
					}
				}
			}
			else
			{
				if(table2!=0)
				{
					if(MmIsAddressValid((PULONG)(table2+(i-0x800)*2)))
					{
						object=*(PULONG)(table2+(i-0x800)*2);
						if(MmIsAddressValid((PULONG)((table2+(i-0x800)*2)+NEXTFREETABLEENTRY)))
						{
							NextFreeTableEntry=*(PULONG)((table2+(i-0x800)*2)+NEXTFREETABLEENTRY);
					    	if(NextFreeTableEntry==0x0)
							{
					    		object=((object | 0x80000000)& 0xfffffff8);
					        	objectheader=(ULONG)OBJECT_TO_OBJECT_HEADER(object);
					        	if(MmIsAddressValid((PULONG)(objectheader+TYPE)))
								{
					            	type=*(PULONG)(objectheader+TYPE);
				                	if(type==processtype)
									{	
							    		flags=*(PULONG)((ULONG)object+FLAGS);
							    	    if((flags&0xc)!=0xc)
							        	RecordInfo(object);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//得到当前进程数量
int GetProNum()
{
	int j=0;
	for(pCid=head;pCid;pCid=pCid->next)
	{
		j++;
	}
	return j;
}
//显示进程信息
VOID DisplayProcess(ProcessInfo *Pro)
{
	ProcessInfo  *q;  
	int j=0;
	//获取进程的ID和进程名
	for(pCid=head;pCid;pCid=pCid->next)
	{
		pCid->pid=*(int *)(pCid->addr+UNIQUEPROCESSID);
    	strcpy(pCid->name,(UCHAR *)(pCid->addr+IMAGEFILENAME));
	}
	for(pCid=head;pCid;pCid=pCid->next)
	{
		j++;
		//DbgPrint("process %8x,%4d,%s\n",pCid->addr,pCid->pid,pCid->name);
		memcpy(Pro+(j-1),pCid,sizeof(ProcessInfo));
		
	}
	//DbgPrint("total number _is %d",j);
	//释放链表
	pCid=head;
	q=pCid->next;
	while(q!=NULL)
	{
		ExFreePool(pCid);
		pCid=q;
		q=pCid->next;
	}
	ExFreePool(pCid);
	head=NULL;
}
VOID FreeProcess()
{
	//释放链表
	ProcessInfo  *q;
	pCid=head;
	q=pCid->next;
	while(q!=NULL)
	{
		ExFreePool(pCid);
		pCid=q;
		q=pCid->next;
	}
	if (pCid!=NULL)
	{
		ExFreePool(pCid);
	}

	head=NULL;
}
VOID FreeThread()
{
	int j=0;PTHREAD_INFO Temp={0};PTHREAD_INFO ThreadHead=ThreadInfoHead;
	//DbgBreakPoint();
	
	while (ThreadHead)
	{
		j++;
		
		Temp=ThreadHead;
		ThreadHead=ThreadHead->Next;
		
		ExFreePool(Temp);
	}

	ThreadInfoHead=NULL;
}
VOID DisplayThread(PTHREAD_INFO  OutBuf)
{
	int j=0;PTHREAD_INFO Temp={0};
	PTHREAD_INFO ThreadHead=ThreadInfoHead;
    //DbgBreakPoint();
	while (ThreadHead)
	{
		j++;
		memcpy(OutBuf+(j-1),ThreadHead,sizeof(THREAD_INFO));
		Temp=ThreadHead;
		ThreadHead=ThreadHead->Next;
		
		ExFreePool(Temp);
	}
}

DWORD GetPlantformDependentInfo ( DWORD dwFlag )   
{    
    DWORD current_build=0;    
    DWORD ans = 0;    
	
    PsGetVersion(NULL, NULL, &current_build, NULL);    
	
    switch ( dwFlag )   
    {    
    case EPROCESS_SIZE:    
        if (current_build == 2195) ans = 0 ;        // 2000，当前不支持2000，下同   
        if (current_build == 2600) ans = 0x25C;     // xp   
        if (current_build == 3790) ans = 0x270;     // 2003   
        break;    
    case PEB_OFFSET:    
        if (current_build == 2195)  ans = 0;    
        if (current_build == 2600)  ans = 0x1b0;    
        if (current_build == 3790)  ans = 0x1a0;   
        break;    
    case FILE_NAME_OFFSET:    
        if (current_build == 2195)  ans = 0;    
        if (current_build == 2600)  ans = 0x174;    
        if (current_build == 3790)  ans = 0x164;   
        break;    
    case PROCESS_LINK_OFFSET:    
        if (current_build == 2195)  ans = 0;    
        if (current_build == 2600)  ans = 0x088;    
        if (current_build == 3790)  ans = 0x098;   
        break;    
    case PROCESS_ID_OFFSET:    
        if (current_build == 2195)  ans = 0;    
        if (current_build == 2600)  ans = 0x084;    
        if (current_build == 3790)  ans = 0x094;   
        break;    
    case EXIT_TIME_OFFSET:    
        if (current_build == 2195)  ans = 0;    
        if (current_build == 2600)  ans = 0x078;    
        if (current_build == 3790)  ans = 0x088;   
        break;    
    }    
    return ans;    
}    
PTHREAD_INFO GetThread(int Pid)
{
	
    //全部定义为ULONG类型
    ULONG pTargetProcess;     //self explanatory
    ULONG pTargetThread;     //thread that can be either alerable or non-alertable
    ULONG pNotAlertableThread; //non-alertable thread
    ULONG pSystemProcess;     //May not necessarily be the 'System' process
    ULONG pTempThread;
    ULONG pNextEntry, pListHead, pThNextEntry,pThListHead,pMyProcess; 
	PTHREAD_INFO PthreadInfo;
    DWORD dwPidOffset  = GetPlantformDependentInfo ( PROCESS_ID_OFFSET ) ;//0x084
    DWORD dwPNameOffset   = GetPlantformDependentInfo ( FILE_NAME_OFFSET ) ; //0x174 
	PTHREAD_INFO temp1,temp2,temp3,Infohead;
	int bFindTid=0;
	if (dwPidOffset==0||dwPNameOffset==0)
	{
		DbgPrint("Get Offset Fail !");
		dwPidOffset=0x084;dwPNameOffset=0x174;
	}
	if (0==Pid)
	{
		return (PTHREAD_INFO)NULL;
	}
	Infohead=temp1=(PTHREAD_INFO)ExAllocatePool(NonPagedPool,sizeof(THREAD_INFO));
    //获得系统进程
    pMyProcess=pSystemProcess =(ULONG)PsGetCurrentProcess(); //make sure you are running at IRQL PASSIVE_LEVEL
	
    if(!pSystemProcess) 
    {
        DbgPrint("KernelExec -> Cannot find 'System' process!");
        return NULL;
    }
    //获取进程列表头(+0x088 ActiveProcessLinks : _LIST_ENTRY)
    pListHead=pSystemProcess+0x88;
    //得到下一个EPROCESS结构的ActiveProcessLinks偏移地址
    pNextEntry=*(ULONG*)pListHead;
    if(!pNextEntry)
        DbgPrint("KernelExec -> No processes found!");
    else
    {
		
        while(pNextEntry != pListHead) //start looping through the available processes
        {    //得到EPROCESS的首地址
            pSystemProcess =pNextEntry-0x88;
            //进程名偏移
            //+0x174 ImageFileName:[16] UChar
            //DbgPrint("ProcessName %s PID:%x\n",(char*)pSystemProcess+dwPNameOffset,*(int*)((char*)pSystemProcess+dwPidOffset));
			
			//Is this explorer.exe? 
			//DbgBreakPoint();
			
			//if(_strnicmp((char*)pSystemProcess+dwPNameOffset,"explorer.exe",12)==0)
			if (*(int*)((char*)pSystemProcess+dwPidOffset)==Pid)
			{    //得到进程的EPROCESS结构的地址
				bFindTid=1;
				pTargetProcess = pSystemProcess; //Yes,we have found it!
				//DbgPrint("yes,we have found explorer.exe!");
				
				pTargetThread = pNotAlertableThread = 0;
				//获取线程列表头
				//+0x050 ThreadListHead   : _LIST_ENTRY
				//也就是_KPROCESS(PCB)中ThreadListHead的偏移地址
				pThListHead = pSystemProcess+0x50;
				//得到ETHREAD结构中_KTHREAD(Tcb)的+0x1b0 ThreadListEntry  : _LIST_ENTRY地址
				pThNextEntry=*(ULONG *)pThListHead;
				//Now we loop through it's threads, seeking an alertable thread
				while(pThNextEntry != pThListHead)
				{    //所属ETHREAD的首地址
					pTempThread =pThNextEntry-0x1b0;
					//DbgPrint("ethread address is:0x%x\n",(ULONG *)pTempThread);
                    //DbgPrint("Start Address  is:0x%x\n",*(DWORD *)(pTempThread+0x228));
					//线程ID
					//ETHREAD+0x1ec Cid : _CLIENT_ID为进程ID
					//再向下+4为线程ID
					//DbgPrint("thread Id is %d\n",*(ULONG *)(pTempThread+0x1f0));
					
					temp2=(PTHREAD_INFO)ExAllocatePool(NonPagedPool,sizeof(THREAD_INFO));
					temp2->dwThreadId  =*(ULONG *)(pTempThread+0x1f0);
		        	temp2->StartAddress=*(int *)(pTempThread+0x228);
					temp2->pEThread=(ULONG *)pTempThread;
		
					temp1->Next=temp2;
					temp1=temp2;
					pNotAlertableThread =pTempThread;
					
					//下一个线程块
					pThNextEntry = *(ULONG *)pThNextEntry; //check next thread
				}
				break;    
			}
            //下一个进程块
            pNextEntry = *(ULONG *)pNextEntry; //get next process
        }
    }
    if (bFindTid==1)
    {	
	temp2->Next=NULL;
	
	temp3=Infohead; 
	Infohead=Infohead->Next;
	ExFreePool(temp3);
	//ObfDereferenceObject((PVOID)pMyProcess);
    return Infohead;
    }
     return NULL;
	

}

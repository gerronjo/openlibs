/*
存放comm_dll和comm_sys共有的但不适合放在comm.h中的信息
*/
#if !defined(COMM_EXT_H)
	#define COMM_EXT_H
	
	struct _XDT
	{
		WORD Limit;
		PVOID Base;
	}__attribute__ ((packed));	//强制该结构在字节粒度对齐，否则寻址结构元素错误
	
	typedef struct _XDT XDT,*PXDT;
	
	typedef struct _GATE
	{
		USHORT OffsetLo;
		USHORT Selector;
		BYTE Count;
		//BYTE Type;
		BYTE Type:4;
		BYTE DT:1;
		BYTE DPL:2;
		BYTE P:1;
		USHORT OffsetHi;
	}GATE,*PGATE;
	
	#define FULLOFFSET(gate) ((gate).OffsetHi<<16|(gate).OffsetLo)
	
	typedef struct _SEGMENT
	{
		USHORT LimitLo;
		USHORT BaseLo;
		BYTE BaseMi;
		BYTE Type:4;
		BYTE DT:1;
		BYTE DPL:2;
		BYTE P:1;
		BYTE LimitHi:4;
		BYTE AVL:1;
		BYTE X:1;
		BYTE D:1;
		BYTE G:1;
		BYTE BaseHi;
	}SEGMENT,*PSEGMENT;
	
	#define FULLBASE(seg) ((seg).BaseHi<<24|(seg).BaseMi<<16|(seg).BaseLo)
	#define FULLLIMIT(seg) ((seg).LimitHi<<16|(seg).LimitLo)
#endif

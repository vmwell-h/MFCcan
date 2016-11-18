#pragma once

/* SECURITYACCESS */
#define UNLOCKKEY					0x00000000
#define UNLOCKSEED					0x00000000
#define UNDEFINESEED				0xFFFFFFFF
#define SEEDMASK					0x80000000
#define SHIFTBIT					1
#define ALGORITHMASK				0x42303131

class CanUtil
{
public:
	CanUtil();
	~CanUtil();
public:
	static LONG str2HEX(PBYTE src_str, PBYTE dst_buf);
	static LONG str2char(CString str, PBYTE ptr);
	static LONG str2DEC(PBYTE src_str, PBYTE dst_buf);
};


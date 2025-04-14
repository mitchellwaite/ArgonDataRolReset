#include <stdint.h>
#include <xtl.h>

extern "C" void __stdcall HalSendSMCMessage(LPVOID pCommandBuffer, LPVOID pRecvBuffer);

extern "C"
    const TCHAR szModuleName[] = TEXT( "ArgonDataRolReset.dll" );

bool IsTrayOpen() { 
    unsigned char msg[0x10]; 
    unsigned char resp[0x10]; 
    memset(msg, 0x0, 0x10); 
    msg[0] = 0xa; 
    HalSendSMCMessage(msg, resp);
    return (resp[1] == 0x60); 
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	// Credits to InvoxiPlayGames/FreeMyXe, resets the ROL in case the
	// JTAG commands on the ARGON_DATA pin cause ROL weirdness
	uint8_t rol_led_buf[16] = {0x99,0x00,0x00,0,0,0,0,0,0,0,0,0,0,0,0,0};

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// Don't send the SMC message if the disc drive is open
		if (IsTrayOpen())
		{
			*(WORD*)((DWORD)hModule + 64) = 1;
			return FALSE;
		}
		
		HalSendSMCMessage(rol_led_buf, NULL);

		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;

	}
	return TRUE;
}
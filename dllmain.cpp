			/* @'   @+          @;         @@@@@         @@@@@,                  @
			@@  ;@+          @;         @,  @         @'  @,                  @           @
			@@, @@+ +@@@: @@@@; @@@@    @,  @ @; '@   @'  @, @@@@  @@@@ :@@@' @@@@, @@@@:@@@#
			@,@.@#+ ,. ## @  @; @  @    @@@@@ ,@ @:   @'  @, @` @` @  @ +@ :; @, @; @  @' @.
			@ @@:#+ '@@@# @  @; @@@@    @,  @  @ @    @'  @, @  @` @@@@  @@@  @  @; @  @' @.
			@ '@ #+ @. @# @  @; @  .    @,  @  '@     @;  @, @  @` @  . `` +@ @  @; @  @' @.
			@    #+ @@@@# @@@@; @@@@    @@@@@ :@:     @@@@@. @  @. @@@@ '@@@@ @` @; @@@@' @@+ */
// Base Made by Oneshot.
// Worked on by Demonic Bunny
#include "includes.h"
typedef HRESULT(__stdcall * f_EndScene)(IDirect3DDevice9 * pDevice); // our function prototype 
f_EndScene oEndScene; // original endscene

typedef long(__stdcall* Reset_t)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
Reset_t oReset = nullptr;

LPD3DXFONT pFont = nullptr;

HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9 * pDevice) // our hooked endscene
{
	DrawManager DrawMng; // Yay we are using our class to do stuff.
	DrawMng.BeginRender(pDevice); // Tell our Hacked Client that we are going to draw stuff and dont fuck up.
	if (!pFont)
	{
		D3DXCreateFontA(pDevice, 24, 0, FW_DONTCARE, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial", &pFont);

	}
	else
	{
		DrawMng.Write(pFont, pDevice, "Sample Text", 75, 25, 24);
	}
	// Put Render Draw here
	
	DrawMng.DrawBox(pDevice, 25, 25, 25, 25);
	DrawMng.DrawFillBox(pDevice, 25, 75, 25, 25);
	DrawMng.DrawLine(pDevice, 22, 125, 22, 25);
	DrawMng.DrawLine(pDevice, 15, 125, 15, 25);
	//DrawMng.DrawLine(pDevice, 23, 125, 23, 23);
	DrawMng.DrawOutlineBox(pDevice, 25, 125, 25, 25);
	
	// End of Render Draw
	DrawMng.EndRender(pDevice); // Tells our Hacked Client that if you fuck up we going to kill you. With this you don't get buggy asus walls
	return oEndScene(pDevice); // call original ensdcene so the game can draw
}

HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (pFont)
	{
		pFont->Release();
		pFont = NULL;

	}
	return oReset(pDevice, pPresentationParameters);
}

DWORD WINAPI MainThread(LPVOID param) // our main thread
{
	static DWORD DirectXDevice = NULL; // our device address. made it static but you can make it just standard DWORD doesent mather.

	while (!DirectXDevice) // while loop so it tries to get the device untill it gets it.
	//	old DirectXDevice = **(DWORD**)(FindPattern("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 0x1); // geting the device by patternscaning
	DirectXDevice = FindPattern2((DWORD)GetModuleHandle(L"d3d9.dll"), 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx"); // geting the device by patternscaning
	DirectX9VTable* VTable = (DirectX9VTable*)(*(DWORD*)(DirectXDevice + 2));
	oEndScene = (f_EndScene)DetourFunction((PBYTE)VTable->EndScene, (PBYTE)Hooked_EndScene);
	oReset = (Reset_t)DetourFunction((PBYTE)VTable->Reset, (PBYTE)Hooked_Reset);
  //old	void ** pVTable = *reinterpret_cast<void***>(DirectXDevice); // getting the vtable array
	// old oEndScene = (f_EndScene)DetourFunction((PBYTE)pVTable[42], (PBYTE)Hooked_EndScene); // hooking the endscene and putting a jump to our function and then storing original in oEndscene

	std::cout << oEndScene; // just writeing it to console to see if it hooked.
	
	//old oReset = (Reset_t)DetourFunction((PBYTE)pVTable[16], (PBYTE)Hooked_Reset);
	return false; 
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // gets runned when injected
		AllocConsole(); // enables the console
		freopen("CONIN$", "r", stdin); // makes it possible to output to output to console with cout.
		freopen("CONOUT$", "w", stdout);
		CreateThread(0, 0, MainThread, hModule, 0, 0); // creates our thread 
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


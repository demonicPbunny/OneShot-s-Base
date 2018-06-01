#pragma once
#include "includes.h"

class DrawManager
{
public:
	void Write(LPD3DXFONT pFont, IDirect3DDevice9* pDevice, char* Text, int x, int y, int Size);
	void DrawLine(IDirect3DDevice9* pDevice, int x1, int y1, int x2, int y2);
	void DrawBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h);
	void DrawFillBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h);
	void DrawOutlineBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h);
	void EndRender(IDirect3DDevice9* pDevice);
	void BeginRender(IDirect3DDevice9* pDevice);
//	void DrawRectangle(IDirect3DDevice9* pDevice);
//	void test();
	

	typedef struct D3DTLVERTEX
	{
		float x;
		float y;
		float z;
		float rhw;
		D3DCOLOR dxColor;
	} *PD3DTLVERTEX;



private:

	void SetVertexState(IDirect3DDevice9* pDevice);

	void CreateVertex(int x, int y, DWORD dxColor, PD3DTLVERTEX pVertext, DWORD index);


	DWORD					dwOld_D3DRS_COLORWRITEENABLE;
	IDirect3DStateBlock9*	m_pStateBlockDraw;
	IDirect3DStateBlock9*	m_pStateBlockText;
};
#include "DrawManager.h"
#define D3DFVF_CUSTOM_VERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )
void DrawManager::Write(LPD3DXFONT pFont, IDirect3DDevice9* pDevice, char* Text, int x, int y, int Size)
{
	//LPD3DXFONT pFont = nullptr;
	RECT pR = { x,y,x,y };
	pFont->DrawTextA(0, Text, -1, &pR, DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
}



void DrawManager::DrawLine(IDirect3DDevice9* pDevice, int x1, int y1, int x2, int y2)
{
	D3DTLVERTEX D3DTLV[2] = { 0 };
	DWORD dxLineColor = D3DCOLOR_RGBA(255,255,255,255);

	CreateVertex(x1, y1, dxLineColor, PD3DTLVERTEX(&D3DTLV), 0);
	CreateVertex(x2, y2, dxLineColor, PD3DTLVERTEX(&D3DTLV), 1);

	SetVertexState(pDevice);

	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, &D3DTLV, sizeof(D3DTLVERTEX));
}


void DrawManager::DrawBox(IDirect3DDevice9* pDevice,int x, int y, int w, int h)
{
	DWORD dxBoxColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	D3DTLVERTEX D3DTLV[8] = { 0 };

	// top
	CreateVertex(x, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 0);
	CreateVertex(x + w, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 1);

	// left
	CreateVertex(x, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 2);
	CreateVertex(x, y + h, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 3);

	// right
	CreateVertex(x + w - 1, y, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 4);
	CreateVertex(x + w - 1, y + h, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 5);

	// bottom
	CreateVertex(x, y + h - 1, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 6);
	CreateVertex(x + w, y + h - 1, dxBoxColor, PD3DTLVERTEX(&D3DTLV), 7);

	SetVertexState(pDevice);

	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 4, &D3DTLV, sizeof(D3DTLVERTEX));
}

void DrawManager::DrawFillBox(IDirect3DDevice9* pDevice,int x, int y, int w, int h)
{
	DWORD dxFillBoxColor = D3DCOLOR_RGBA(255, 255, 255, 255);

	D3DTLVERTEX D3DTLV[4] = { 0 };

	CreateVertex(x, y, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 0);
	CreateVertex(x + w, y, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 1);
	CreateVertex(x, y + h, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 2);
	CreateVertex(x + w, y + h, dxFillBoxColor, PD3DTLVERTEX(&D3DTLV), 3);

	SetVertexState(pDevice);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &D3DTLV, sizeof(D3DTLVERTEX));
}

void DrawManager::DrawOutlineBox(IDirect3DDevice9* pDevice,int x, int y, int w, int h)
{
	DrawBox(pDevice,x, y, w, h);
	DrawBox(pDevice,x + 1, y + 1, w - 2, h - 2);
	DrawBox(pDevice,x + 2, y + 2, w - 4, h - 4);
}

void DrawManager::CreateVertex(int x, int y, DWORD dxColor, PD3DTLVERTEX pVertext, DWORD index)
{
	pVertext[index].x = (float)x;
	pVertext[index].y = (float)y;
	pVertext[index].z = 0.f;
	pVertext[index].rhw = 0.f;
	pVertext[index].dxColor = dxColor;
}

void DrawManager::SetVertexState(IDirect3DDevice9* pDevice)
{
	pDevice->SetTexture(0, 0);
	pDevice->SetPixelShader(0);

	pDevice->SetFVF(D3DFVF_CUSTOM_VERTEX);

	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_FOGENABLE, false);

	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
	pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, false);

	pDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
}


void DrawManager::BeginRender(IDirect3DDevice9* pDevice)
{
	pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlockDraw);
	pDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateBlockText);

	if (m_pStateBlockDraw)
		m_pStateBlockDraw->Capture();

	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
}

void DrawManager::EndRender(IDirect3DDevice9* pDevice)
{
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);

	if (m_pStateBlockDraw)
	{
		m_pStateBlockDraw->Apply();
		m_pStateBlockDraw->Release();
	}

	if (m_pStateBlockText)
		m_pStateBlockText->Release();
}
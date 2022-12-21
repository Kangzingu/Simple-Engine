#include "textclass.h"

TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;

	m_sentence1 = 0;
	m_sentence2 = 0;
}

TextClass::TextClass(const TextClass& other)
{

}

TextClass::~TextClass()
{

}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	
	m_baseViewMatrix = baseViewMatrix;

	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	char fontDataFilename[128] = "../Resources/Fonts/FontIndex.txt";
	wchar_t fontTextureFilename[128] = L"../Resources/Fonts/Font.dds";

	result = m_Font->Initialize(device, fontDataFilename, fontTextureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}


}
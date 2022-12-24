#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Text = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.001f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	m_D3D->BeginScene(0.0f, 0.0f, 1.0f, 1.0f);
	
	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_D3D->TurnZBufferOff();

	m_D3D->TurnOnAlphaBlending();

	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	m_D3D->TurnOffAlphaBlending();

	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	return true;
}
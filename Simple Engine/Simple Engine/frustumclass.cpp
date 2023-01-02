#include "frustumclass.h"

FrustumClass::FrustumClass()
{

}

FrustumClass::FrustumClass(const FrustumClass& other)
{

}

FrustumClass::~FrustumClass()
{

}

void FrustumClass::ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix)
{
	float zMinimum, r;
	D3DXMATRIX matrix;

	// projectionMatrix
	// 
	// xScale		0			0			0
	// 0			yScale		0			0
	// 0			0		zf/(zf-zn)		1
	// 0			0	-zn*zf/(zf-zn)		0
	// 
	// viewMatrix가 Identity인 경우 proj * view
	// xScale		0			0			0
	// 0			yScale		0			0
	// 0			0		zf/(zf-zn)		1
	// 0			0	-zn*zf/(zf-zn)		0

	zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * zMinimum;

	D3DXMatrixMultiply(&matrix, &viewMatrix, &projectionMatrix);
	
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	D3DXPlaneNormalize(&m_planes[0], &m_planes[0]);

	m_planes[1].a = matrix._14 - matrix._13;
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	D3DXPlaneNormalize(&m_planes[1], &m_planes[1]);
	
	m_planes[2].a = matrix._14 + matrix._11;
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	D3DXPlaneNormalize(&m_planes[2], &m_planes[2]);

	m_planes[3].a = matrix._14 - matrix._11;
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	D3DXPlaneNormalize(&m_planes[3], &m_planes[3]);

	m_planes[4].a = matrix._14 - matrix._12;
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	D3DXPlaneNormalize(&m_planes[4], &m_planes[4]);

	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	D3DXPlaneNormalize(&m_planes[5], &m_planes[5]);

	return;
}

bool FrustumClass::CheckPoint(float x, float y, float z)
{
	int i;
	D3DXVECTOR3 pos;

	pos = D3DXVECTOR3(x, y, z);
	for (i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) < 0.0f)
		{
			return false;
		}
	}

	return true;
}

bool FrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;
	D3DXVECTOR3 pos;

	for (i = 0; i < 6; i++)
	{
		pos = D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter - radius));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter - radius));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter - radius));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter - radius));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter - radius), (yCenter - radius), (zCenter + radius));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter + radius), (yCenter - radius), (zCenter + radius));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter - radius), (yCenter + radius), (zCenter + radius));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter + radius), (yCenter + radius), (zCenter + radius));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		return false;
	}

	return true;
}

bool FrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	int i;
	D3DXVECTOR3 pos;

	pos = D3DXVECTOR3(xCenter, yCenter, zCenter);
	for (i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) < -radius)
		{
			return false;
		}
	}

	return true;
}

bool FrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	int i;
	D3DXVECTOR3 pos;

	for (i = 0; i < 6; i++)
	{
		pos = D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter - zSize));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter - zSize));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter - zSize));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter - zSize));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter - xSize), (yCenter - ySize), (zCenter + zSize));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter + xSize), (yCenter - ySize), (zCenter + zSize));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter - xSize), (yCenter + ySize), (zCenter + zSize));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		pos = D3DXVECTOR3((xCenter + xSize), (yCenter + ySize), (zCenter + zSize));
		if (D3DXPlaneDotCoord(&m_planes[i], &pos) >= 0)
		{
			continue;
		}

		return false;
	}

	return true;
}
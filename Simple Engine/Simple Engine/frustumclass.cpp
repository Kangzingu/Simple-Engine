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
	// 0			0		-zn*zf/(zf-zn)	0

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
	19/9
	-10/9
}
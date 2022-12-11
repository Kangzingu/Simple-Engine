#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <D3DX10math.h>

class LightClass
{
public:
	LightClass();
	LightCLass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
};

#endif
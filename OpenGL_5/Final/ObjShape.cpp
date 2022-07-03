#include "ObjShape.h"



ObjShape::ObjShape(ShapeData data)
{
	_iNumVtx = data.vtxNum;
	_pPoints = NULL; _pNormals = NULL; _pTex1 = NULL;

	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];

	_pTex1 = new vec2[_iNumVtx];
	_pTex2 = new vec2[_iNumVtx];  // ���� light map �һݪ��K�Ϯy��
	_pTex3 = new vec2[_iNumVtx];	// ���� normal map �һݪ��K�Ϯy��
	_pTangentV = new vec3[_iNumVtx];

	
	for (int i = 0; i < _iNumVtx; i++) { 
		_pPoints[i] = data.points[i];
		_pNormals[i] = data.normals[i];
		_pTex1[i] = data.texs[i];
		_pTex2[i] = data.texs[i];
		_pTex3[i] = data.texs[i];
	}
	// �p�� tangent vector
	for (int i = 0; i < _iNumVtx; i += 3) { // �T�� vertex �@��
		float dU1 = _pTex3[i + 1].x - _pTex3[i].x;
		float dV1 = _pTex3[i + 1].y - _pTex3[i].y;
		float dU2 = _pTex3[i + 2].x - _pTex3[i].x;
		float dV2 = _pTex3[i + 2].y - _pTex3[i].y;
		float f = 1.0f / (dU1 * dV2 - dU2 * dV1);
		vec4 E1 = _pPoints[i + 1] - _pPoints[i];
		vec4 E2 = _pPoints[i + 2] - _pPoints[i];

		vec3 tangent;
		tangent.x = f * (dV2 * E1.x + E2.x * (-dV1));
		tangent.y = f * (dV2 * E1.y + E2.y * (-dV1));
		tangent.z = f * (dV2 * E1.z + E2.z * (-dV1));

		_pTangentV[i] += tangent;
		_pTangentV[i + 1] += tangent;
		_pTangentV[i + 2] = tangent;
	}
	for (int i = 0; i < _iNumVtx; i++)
		_pTangentV[i] = normalize(_pTangentV[i]);

	// �]�w����
	setMaterials(vec4(0), vec4(0.5f, 0.5f, 0.5f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	setKaKdKsShini(0, 0.8f, 0.2f, 1);
}


void ObjShape::draw()
{
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Change to wireframe mode
	drawingSetShader();
	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Return to solid mode
}

void ObjShape::drawW()
{
	drawingWithoutSetShader();
	glDrawArrays(GL_TRIANGLES, 0, _iNumVtx);
}

// ���B�ҵ��� vLightPos �����O�@�ɮy�Ъ��T�w�����m
void ObjShape::update(float dt, point4 vLightPos, color4 vLightI)
{
	updateMatrix(); // �o��@�w�n���A�i��x�}����s�A�A�i������C��p��

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(vLightPos, vLightI);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(vLightPos, vLightI);
	else {
		_vLightInView = _mxView * vLightPos;	// �N Light �ഫ�����Y�y�ЦA�ǤJ shader
		// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
		_AmbientProduct = _Material.ka * _Material.ambient * vLightI;
		_DiffuseProduct = _Material.kd * _Material.diffuse * vLightI;
		_SpecularProduct = _Material.ks * _Material.specular * vLightI;
	}
}

void ObjShape::update(float dt, const LightSource& Lights)
{
	updateMatrix(); // �o��@�w�n���A�i��x�}����s�A�A�i������C��p��

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(Lights);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(Lights);
	else {
		_vLightInView = _mxView * Lights.position;		// �N Light �ഫ�����Y�y�ЦA�ǤJ shader
		// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
		_AmbientProduct = _Material.ka * _Material.ambient * Lights.ambient;
		_AmbientProduct.w = _Material.ambient.w;
		_DiffuseProduct = _Material.kd * _Material.diffuse * Lights.diffuse;
		_DiffuseProduct.w = _Material.diffuse.w;
		_SpecularProduct = _Material.ks * _Material.specular * Lights.specular;
		_SpecularProduct.w = _Material.specular.w;
	}
}

// �I�s�S���������� update �N��Ӫ��󤣷|�i������ө����p��
void ObjShape::update(float dt)
{
	updateMatrix(); // �o��@�w�n���A�i��x�}����s�A�A�i������C��p��
}


void ObjShape::update(float dt, const LightSource& Light1, const LightSource& Light2)
{
	updateMatrix();

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(Light1);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(Light1);
	else {
		_vLightInView = _mxView * (Light1.position * Light2.position);		// �N Light �ഫ�����Y�y�ЦA�ǤJ shader
		// ��X AmbientProduct DiffuseProduct �P SpecularProduct �����e
		_AmbientProduct = _Material.ka * _Material.ambient * (Light1.ambient + Light2.ambient);
		_AmbientProduct.w = _Material.ambient.w;
		_DiffuseProduct = _Material.kd * _Material.diffuse * (Light1.diffuse + Light2.diffuse);
		_DiffuseProduct.w = _Material.diffuse.w;
		_SpecularProduct = _Material.ks * _Material.specular * (Light1.specular + Light2.specular);
		_SpecularProduct.w = _Material.specular.w;
	}
}


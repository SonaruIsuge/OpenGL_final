#include "ObjShape.h"



ObjShape::ObjShape(ShapeData data)
{
	_iNumVtx = data.vtxNum;
	_pPoints = NULL; _pNormals = NULL; _pTex1 = NULL;

	_pPoints = new vec4[_iNumVtx];
	_pNormals = new vec3[_iNumVtx];
	_pColors = new vec4[_iNumVtx];

	_pTex1 = new vec2[_iNumVtx];
	_pTex2 = new vec2[_iNumVtx];  // 產生 light map 所需的貼圖座標
	_pTex3 = new vec2[_iNumVtx];	// 產生 normal map 所需的貼圖座標
	_pTangentV = new vec3[_iNumVtx];

	
	for (int i = 0; i < _iNumVtx; i++) { 
		_pPoints[i] = data.points[i];
		_pNormals[i] = data.normals[i];
		_pTex1[i] = data.texs[i];
		_pTex2[i] = data.texs[i];
		_pTex3[i] = data.texs[i];
	}
	// 計算 tangent vector
	for (int i = 0; i < _iNumVtx; i += 3) { // 三個 vertex 一組
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

	// 設定材質
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

// 此處所給的 vLightPos 必須是世界座標的確定絕對位置
void ObjShape::update(float dt, point4 vLightPos, color4 vLightI)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(vLightPos, vLightI);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(vLightPos, vLightI);
	else {
		_vLightInView = _mxView * vLightPos;	// 將 Light 轉換到鏡頭座標再傳入 shader
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_AmbientProduct = _Material.ka * _Material.ambient * vLightI;
		_DiffuseProduct = _Material.kd * _Material.diffuse * vLightI;
		_SpecularProduct = _Material.ks * _Material.specular * vLightI;
	}
}

void ObjShape::update(float dt, const LightSource& Lights)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(Lights);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(Lights);
	else {
		_vLightInView = _mxView * Lights.position;		// 將 Light 轉換到鏡頭座標再傳入 shader
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_AmbientProduct = _Material.ka * _Material.ambient * Lights.ambient;
		_AmbientProduct.w = _Material.ambient.w;
		_DiffuseProduct = _Material.kd * _Material.diffuse * Lights.diffuse;
		_DiffuseProduct.w = _Material.diffuse.w;
		_SpecularProduct = _Material.ks * _Material.specular * Lights.specular;
		_SpecularProduct.w = _Material.specular.w;
	}
}

// 呼叫沒有給光源的 update 代表該物件不會進行光源照明的計算
void ObjShape::update(float dt)
{
	updateMatrix(); // 這行一定要有，進行矩陣的更新，再進行後續的顏色計算
}


void ObjShape::update(float dt, const LightSource& Light1, const LightSource& Light2)
{
	updateMatrix();

	if (_iMode == ShadingMode::FLAT_SHADING_CPU) renderWithFlatShading(Light1);
	else if (_iMode == ShadingMode::GOURAUD_SHADING_CPU) renderWithGouraudShading(Light1);
	else {
		_vLightInView = _mxView * (Light1.position * Light2.position);		// 將 Light 轉換到鏡頭座標再傳入 shader
		// 算出 AmbientProduct DiffuseProduct 與 SpecularProduct 的內容
		_AmbientProduct = _Material.ka * _Material.ambient * (Light1.ambient + Light2.ambient);
		_AmbientProduct.w = _Material.ambient.w;
		_DiffuseProduct = _Material.kd * _Material.diffuse * (Light1.diffuse + Light2.diffuse);
		_DiffuseProduct.w = _Material.diffuse.w;
		_SpecularProduct = _Material.ks * _Material.specular * (Light1.specular + Light2.specular);
		_SpecularProduct.w = _Material.specular.w;
	}
}


////DirectionalLight Struct
//struct DirectionalLight {
//	float4 Ambient;
//	float4 Diffuse;
//	float4 Specular;
//	float3 Direction;
//	float pad;
//};
//
////PointLight Struct
//struct PointLight {
//	float4 Ambient;
//	float4 Diffuse;
//	float4 Specular;
//
//	float3 Position;
//	float Range;
//
//	float3 Att;
//	float pad;
//};
//
////SpotLight Struct
//struct SpotLight {
//	float4 Ambient;
//	float4 Diffuse;
//	float4 Specular;
//
//	float3 Position;
//	float Range;
//
//	float3 Direction;
//	float Spot;
//
//	float3 Att;
//	float pad;
//};
//
//cbuffer cbPerFrane {
//	DirectionalLight gDirLight;
//	PointLight gPointLight;
//	SpotLight gSpotLight;
//	float gEyePosW;
//};
//
//
//void ComputeDirectionalLight(Material mat, DirectionalLight L, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec) {
//	//initialise outputs
//	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	//light vector points in opposite direction of ray direction
//	float3 lightVec = -L.Direction;
//	
//	//adding ambient term
//	ambient = mat.Ambient * L.Ambient;
//
//	//Add diffuse and specular term (if surface is visible)
//	float diffuseFactor = dot(lightVec, normal);
//
//	//flatten to avoid dynamic branching
//	[Flatten]
//	if (diffuseFactor > 0.0f) {
//		float3 v = reflect(-lightVec, normal);
//		float specFactor = pov(max(dot(v, toEye), 0.0f), mat.Specular.w);
//
//		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
//		spec = specFactor * mat.Specular * L.Specular;
//
//	}
//}
//
//void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec) {
//	//initialise outputs
//	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	//Vector from surface to the light
//	float3 lightVec = L.Position - pos;
//
//	//Distance from surface to light
//	float d = length(lightVec);
//
//	//Range test
//	if (d > L.Range) {
//		return;
//	}
//
//	//Normalise the light vector
//	lightVec /= d;
//
//	//Ambient Term
//	ambient = mat.Ambient * L.Ambient;
//
//	//Add diffuse and specular term, provided the surface is in line of sight
//	float diffuseFactor = dot(lightVec, normal);
//	[Flatten]
//	if (diffuseFactor > 0.0f) {
//		float3 v = reflect(-lightVec, normal);
//		float specFactor = pov(max(dot(v, toEye), 0.0f), mat.Specular.w);
//
//		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
//		spec = specFactor * mat.Specular * L.Specular;
//
//	}
//	//attenuate
//	float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));
//	diffuse *= att;
//	spec *= att;
//
//
//}
//
//void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 spec) {
//	//initialise outputs
//	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
//	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
//
//	//Vector from surface to the light
//	float3 lightVec = L.Position - pos;
//
//	//Distance from surface to light
//	float d = length(lightVec);
//
//	//Range test
//	if (d > L.Range) {
//		return;
//	}
//	//Normalise the light vector
//	lightVec /= d;
//	//Ambient Term
//	ambient = mat.Ambient * L.Ambient;
//	//Add diffuse and specular term, provided the surface is in line of sight
//	float diffuseFactor = dot(lightVec, normal);
//	[Flatten]
//	if (diffuseFactor > 0.0f) {
//		float3 v = reflect(-lightVec, normal);
//		float specFactor = pov(max(dot(v, toEye), 0.0f), mat.Specular.w);
//
//		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
//		spec = specFactor * mat.Specular * L.Specular;
//
//	}
//
//	//Scale by spot light factot and attenuate
//	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);
//
//
//	//attenuate
//	float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));
//	ambient *= spot;
//	diffuse *= att;
//	spec *= att;
//}
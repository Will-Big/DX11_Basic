cbuffer VSFixedData : register(b0) // VS 0
{
    matrix gProjectionMatrix; // rarely updated
}

cbuffer VSCameraData : register(b1) // VS 1
{
    matrix gViewMatrix; // once updated per frame
}

cbuffer VSObjectData : register(b2) // VS 2
{
    matrix gWorldMatrix; // always updated per object
}

cbuffer VSMatrixPallete : register(b3) // VS 3
{
    matrix gMatrixPalleteArray[128]; // always updated per skinned object
}

cbuffer PSCameraData : register(b0) // PS 0
{
    float4 gWorldCameraPosition; // once updated per frame
}

cbuffer PSLightData : register(b1) // PS 1
{
    float4 gWorldLightDirection; // once updated per frame
    float4 gWorldLightColor; // once updated per frame
    float gWorldLightIntensity; // once updated per frame
}

cbuffer PSMaterialData : register(b2) // PS 2
{
    uint gShaderScope; // always updated per object
    float gSpecularPower; // always updated per object
    float gOpacityPower; // always updated per object
}

// 교수님 코드
struct VertexShaderInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 texcoord : TEXCOORD;
};

struct PixelShaderInput
{
    float4 pixelPosition : SV_POSITION;
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3x3 tangentBasis : TBASIS;
};

Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D metalnessTexture : register(t5);
Texture2D roughnessTexture : register(t6);
//TextureCube specularTexture : register(t4);
//TextureCube irradianceTexture : register(t5);
//Texture2D specularBRDF_LUT : register(t6);

SamplerState defaultSampler : register(s0);
SamplerState spBRDF_Sampler : register(s1);

static const float PI = 3.141592;
static const float Epsilon = 0.00001;
static const float3 Fdielectric = 0.04;

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor. 최소값 F0 , 최대값은 1.0,1.0,1.0
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
//uint querySpecularTextureLevels()
//{
//    uint width, height, levels;
//    specularTexture.GetDimensions(0, width, height, levels);
//    return levels;
//}

// Vertex shader
PixelShaderInput main_vs(VertexShaderInput vin)
{
    PixelShaderInput vout;

    vout.position = mul(vin.position, gWorldMatrix).xyz;
    vout.texcoord = float2(vin.texcoord.x, vin.texcoord.y);

	// Pass tangent space basis vectors (for normal mapping).
    float3x3 TBN = float3x3(vin.tangent, vin.bitangent, vin.normal);
    vout.tangentBasis = mul((float3x3) gWorldMatrix, transpose(TBN));

    float4x4 mvpMatrix = mul(mul(gWorldMatrix, gViewMatrix), gProjectionMatrix);
    vout.pixelPosition = mul(vin.position, mvpMatrix);

    return vout;
}

// Pixel shader
float4 main_ps(PixelShaderInput pin) : SV_Target
{
	// Sample input textures to get shading model params.
    float3 albedo = pow(albedoTexture.Sample(defaultSampler, pin.texcoord).rgb, 2.2f);
    float metalness = metalnessTexture.Sample(defaultSampler, pin.texcoord).r;
    float roughness = roughnessTexture.Sample(defaultSampler, pin.texcoord).r;

	// Outgoing light direction (vector from world-space fragment position to the "eye").
    float3 view = normalize(gWorldCameraPosition - pin.position);

	// Get current fragment's normal and transform to world space.
    float3 normal = normalize(2.0 * normalTexture.Sample(defaultSampler, pin.texcoord).rgb - 1.0);
    normal = normalize(mul(pin.tangentBasis, normal));
	
	// Angle between surface normal and outgoing light direction.
    float cosNV = max(0.0, dot(normal, view));
		
	// Specular reflection vector.
    float3 Lr = 2.0 * cosNV * normal - view;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
    float3 F0 = lerp(Fdielectric, albedo, metalness);

	// Direct lighting calculation for analytical lights.
    float3 directLighting = 0.0;

	//float3 ld = -lights[i].direction;
	//float3 lr = lights[i].radiance;

	// Half-vector between ld and lr.
    float3 hv = normalize(view + gWorldLightDirection.xyz);

		// Calculate angles between surface normal and various light vectors.
    float cosNL = max(0.0, dot(normal, gWorldLightDirection.xyz));
    float cosNH = max(0.0, dot(normal, hv));

		// Calculate Fresnel term for direct lighting. 
    float3 F = fresnelSchlick(F0, max(0.0, dot(hv, view))); //최소값 F0 , 최대값은 1.0,1.0,1.0
		// Calculate normal distribution for specular BRDF.
    float D = ndfGGX(cosNH, max(0.01, roughness)); // 러프니스 0 이되면 값이0이 되므로 0이면 최소값사용
		// Calculate geometric attenuation for specular BRDF.
    float G = gaSchlickGGX(cosNL, cosNV, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		// 금속성이 1이면 침투가 없으므로 표면산란이 일어나지 않아 표면산란의 색상은 0,0,0 으로 처리
		// 금속성이 0이면 반사의 나머지 비율만큼 표면산란의 색상 표현이 가능하다.
		// 금속성은 1픽셀에 해당되는 미세 표면의 분포를 의미하므로 보간을 상요하여 중간값을 처리한다.
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
    float3 diffuseBRDF = kd * albedo / PI;
       
		
		// Cook-Torrance specular microfacet BRDF.
    float3 specularBRDF = (F * D * G) / max(Epsilon, 4.0 * cosNL * cosNV);
		
		// Total contribution for this light.
    directLighting += (diffuseBRDF + specularBRDF) * cosNL;
    

	// Ambient lighting (IBL).
    float3 ambientLighting = albedo * 0.3f;
#ifdef IBL
    if (useIBL.r > 0)
    {
		// Sample diffuse irradiance at normal direction.
        float3 irradiance = irradianceTexture.Sample(defaultSampler, normal).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
        float3 F = fresnelSchlick(F0, cosNV);

		// Get diffuse contribution factor (as with direct lighting).
        float3 kd = lerp(1.0 - F, 0.0, metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
        float3 diffuseIBL = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level.
        uint specularTextureLevels = querySpecularTextureLevels();
        float3 specularIrradiance = specularTexture.SampleLevel(defaultSampler, Lr, roughness * specularTextureLevels).rgb;

		// Split-sum approximation factors for Cook-Torrance specular BRDF.
        float2 specularBRDF = specularBRDF_LUT.Sample(spBRDF_Sampler, float2(cosNV, roughness)).rg;

		// Total specular IBL contribution.
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
        ambientLighting = diffuseIBL + specularIBL;
    }
#endif

   
	// Final fragment color.
   // return float4(directLighting, 1.0);
    float4 finalColor = pow(float4(directLighting + ambientLighting, 1.0), 1 / 2.2);
    return finalColor;
}

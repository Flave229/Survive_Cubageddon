#include "TextSystem.h"
#include "UISystem.h"

TextSystem::TextSystem(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, Box screenSize) : _direct3D(direct3D), _shaderController(shaderController), _fontEngine(fontEngine), _screenSize(screenSize)
{
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMVECTOR upVector = XMLoadFloat3(&up);

	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	XMStoreFloat3(&lookAt, lookAtVector);
	XMStoreFloat3(&up, upVector);

	_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(new XMFLOAT3(0, 0, -1)), lookAtVector, upVector);
}

TextSystem::~TextSystem()
{
}

void TextSystem::Shutdown()
{
}

void TextSystem::Update(vector<Entity*>& entities, float delta)
{
	for (Entity* entity : entities)
	{
		IComponent* component = entity->GetComponent(TEXT);
		if (component == nullptr) continue;
		TextComponent* text = static_cast<TextComponent*>(component);

		if (text->Text == text->PreviousText)
			continue;

		UpdateTextEntity(text);
		
		for (TextTexture characterTexture : text->TextEntity)
			UpdateAppearance(characterTexture);
	}
}

void TextSystem::UpdateTextEntity(TextComponent* textComponent)
{
	try
	{
		vector<TextTexture>& textTextures = textComponent->TextEntity;
		string& currentText = textComponent->Text;
		string& previousText = textComponent->PreviousText;
		int textSizeDifference = currentText.size() - previousText.size();

		if (textSizeDifference > 0)
		{
			string additionalCharacters = string(currentText.end() - textSizeDifference, currentText.end());
			XMFLOAT2 adjustedPosition = XMFLOAT2(textComponent->FontPosition.x + (previousText.size() * textComponent->FontSize), textComponent->FontPosition.y);
			vector<TextTexture> additionalTextures = _fontEngine->ConvertTextToTextEntity(adjustedPosition, "Impact", additionalCharacters, textComponent->Color, textComponent->FontSize);
			textTextures.insert(textTextures.end(), additionalTextures.begin(), additionalTextures.end());
		}
		else if (textSizeDifference < 0)
		{
			for (int i = 0; i < textSizeDifference * -1; i++)
			{
				textTextures.at(textTextures.size() - 1).Shutdown();
				textTextures.pop_back();
			}
		}

		for (int i = 0; i < currentText.size() - textSizeDifference && i < textTextures.size(); i++)
		{
			if (currentText.at(i) == previousText.at(i))
				continue;

			textTextures.at(i).CharacterTexture = _fontEngine->ConvertCharacterToTexture("Impact", string(1, currentText.at(i)));
		}

		textComponent->PreviousText = textComponent->Text;
	}
	catch (...)
	{
		throw Exception("A problem occured updating the text \"" + textComponent->Text + "\". The previous frame displayed the text \"" + textComponent->PreviousText + "\".");
	}
}

void TextSystem::UpdateAppearance(TextTexture& texture)
{
	if ((texture.Position.x == static_cast<int>(texture.PreviousPosition.x)) && (texture.Position.y == static_cast<int>(texture.PreviousPosition.y))
		&& (texture.Size == texture.PreviousSize))
	{
		return;
	}

	texture.PreviousPosition = texture.Position;
	texture.PreviousSize = texture.Size;

	float left = ((_screenSize.Width / 2) * -1) + texture.Position.x;
	float right = left + texture.Size;

	float top = (_screenSize.Height / 2) - texture.Position.y;
	float bottom = top - texture.Size * 2;

	Vertex* vertices = new Vertex[texture.Model.VertexCount];
	if (!vertices) throw Exception("Failed to initialise vertices for bitmap");

	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(left, top, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(texture.Model.VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map vertex buffer to the Device Context");

	Vertex* verticesPtr = static_cast<Vertex*>(mappedResource.pData);

	memcpy(verticesPtr, static_cast<void*>(vertices), (sizeof(Vertex) * texture.Model.VertexCount));

	_direct3D->GetDeviceContext()->Unmap(texture.Model.VertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;
}

void TextSystem::Render(vector<Entity*>& entities)
{
	for (Entity* entity : entities)
	{
		IComponent* component = entity->GetComponent(APPEARANCE);

		if (component != nullptr)
		{
			AppearanceComponent* appearance = static_cast<AppearanceComponent*>(component);
			if (appearance->RenderEnabled == false) continue;
		}

		component = entity->GetComponent(TEXT);
		if (component == nullptr) continue;
		TextComponent* text = static_cast<TextComponent*>(component);

		RenderCharacters(text->TextEntity);
	}
}

void TextSystem::RenderCharacters(vector<TextTexture>& characters)
{
	for (TextTexture character : characters)
	{
		BuildBufferInformation(character);
		ShaderResources shaderResources = BuildShaderResources(character);

		IShaderType* shader = _shaderController->GetShader(SHADER_FONT);
		shader->Render(character.Model.IndexCount, shaderResources);
	}
}

void TextSystem::BuildBufferInformation(TextTexture& character) const
{
	_direct3D->GetRasterizer()->SetRasterizerCullMode(D3D11_CULL_BACK);
	_direct3D->TurnZBufferOff();

	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &character.Model.VertexBuffer, &character.Model.VBStride, &character.Model.VBOffset);
	deviceContext->IASetIndexBuffer(character.Model.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ShaderResources TextSystem::BuildShaderResources(TextTexture& character) const
{
	ShaderResources shaderResources = ShaderResources();
	shaderResources.MatrixParameters.ViewMatrix = _viewMatrix;
	shaderResources.MatrixParameters.WorldMatrix = _direct3D->GetWorldMatrix();
	shaderResources.MatrixParameters.ProjectionMatrix = _direct3D->GetOrthoMatrix();

	shaderResources.ColorParameters = character.Color;

	if (character.CharacterTexture != nullptr)
		shaderResources.TextureParameters.TextureArray = vector<ID3D11ShaderResourceView*> { ExtractResourceViewsFrom(character.CharacterTexture) };

	return shaderResources;
}

ID3D11ShaderResourceView* TextSystem::ExtractResourceViewsFrom(Texture* texture)
{
	return texture->GetTexture();
}
#include "Cube.hpp"
#include <glad/gl.h>

Cube::Cube(bool light)
    : Scale(1.0f), Rotation(0.0f), Position(0.0f), Color({1.0f, 0.5f, 0.31f})
{
    auto vertexBuffer = Buffer::Create(BufferType::Vertex, cubeVertices, sizeof(cubeVertices) / sizeof(f32));
    auto indexBuffer = Buffer::Create(BufferType::Index, cubeIndices, sizeof(cubeIndices) / sizeof(u32));
    m_CubeVA = VertexArray::Create(vertexBuffer, indexBuffer);

    if (light)
    {
        m_Shader = Shader::Create("assets/shaders/vertex.glsl", "assets/shaders/lightSourceFrag.glsl");
    }
    else
    {
        m_Shader = Shader::Create("assets/shaders/vertex.glsl", "assets/shaders/lightFrag.glsl");
    }
}

void Cube::Draw(const Camera& camera) const
{
    m_Shader->Bind();
    m_Shader->SetMat4f("model", GetModelMatrix());
    m_Shader->SetMat4f("view", camera.GetViewMatrix());
    m_Shader->SetMat4f("projection", camera.GetProjectionMatrix());
    m_Shader->Set3f("lightColor", Color.r, Color.g, Color.b);
    m_CubeVA->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::DrawLit(const Camera& camera, const std::unique_ptr<Cube>& lightSource) const
{
    m_Shader->Bind();
    auto lightPos = lightSource->GetModelMatrix()[3];
    m_Shader->Set3f("viewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    m_Shader->Set3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
    m_Shader->Set3f("objectColor", Color.r, Color.g, Color.b);
    m_Shader->Set3f("lightColor", lightSource->Color.r, lightSource->Color.g, lightSource->Color.b);
    m_Shader->SetMat4f("model", GetModelMatrix());
    m_Shader->SetMat4f("view", camera.GetViewMatrix());
    m_Shader->SetMat4f("projection", camera.GetProjectionMatrix());
    m_CubeVA->Bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
//    Renderer::Draw(m_CubeVA.get(), m_Shader.get());
}

void Cube::SetScale(const glm::vec3& scale)
{
    Scale = scale;
}

void Cube::SetPosition(const glm::vec3& position)
{
    Position = position;
}

void Cube::SetColor(const glm::vec3& color)
{
    Color = color;
}

glm::mat4 Cube::GetModelMatrix() const
{
    glm::mat4 out = glm::translate(glm::mat4(1.0f), Position);
    out = glm::scale(out, Scale);
    return out;
}
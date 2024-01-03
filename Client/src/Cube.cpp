#include "Cube.hpp"

Cube::Cube()
    : m_Scale(1.0f), m_Rotation(0.0f), m_Position(0.0f)
{
    auto vertexBuffer = Buffer::Create(BufferType::Vertex, cubeVertices, sizeof(cubeVertices) / sizeof(f32));
    auto indexBuffer = Buffer::Create(BufferType::Index, cubeIndices, sizeof(cubeIndices) / sizeof(u32));
    m_CubeVA = VertexArray::Create(vertexBuffer, indexBuffer);
    m_Shader = Shader::Create("assets/shaders/vertex.glsl", "assets/shaders/cubeFrag.glsl");
}

void Cube::Draw(const Camera& camera) const
{
    auto cubeModel = glm::scale(glm::mat4(1.0f), m_Scale);
    glm::rotate(cubeModel, 1.0f, m_Rotation);
    glm::translate(cubeModel, m_Position);
    m_Shader->SetMat4f("model", cubeModel);
    m_Shader->SetMat4f("view", camera.GetViewMatrix());
    m_Shader->SetMat4f("projection", camera.GetProjectionMatrix());
    Renderer::Draw(m_CubeVA.get(), m_Shader.get());
}
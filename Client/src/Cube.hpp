#pragma once

#include <Blaze.hpp>

inline constexpr f32 cubeVertices[] = {
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
};

inline constexpr u32 cubeIndices[] = {
    0, 2, 3, 0, 3, 1,
    2, 6, 7, 2, 7, 3,
    6, 4, 5, 6, 5, 7,
    4, 0, 1, 4, 1, 5,
    0, 4, 6, 0, 6, 2,
    1, 5, 7, 1, 7, 3,
};

class Cube
{
public:
    Cube();

    void Draw(const Camera& camera) const;
private:
    std::shared_ptr<VertexArray> m_CubeVA;
    std::shared_ptr<Shader> m_Shader;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;
    glm::vec3 m_Position;
};
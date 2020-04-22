#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"

class Specimen
{
public:
    Specimen();
    void InitGenome(size_t circleCount);
    void Generate(glm::vec2 dimensions);
    const Texture& GetTexture();

private:
    // For each circle: it's position, radius, and color
    static const size_t COMP_COUNT = 6;
    std::vector<GLfloat> m_genome;

    std::vector<Texture> m_buf{2};

    VertexBuffer m_vbo;
    VertexArray m_vao;
};
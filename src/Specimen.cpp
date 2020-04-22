#include <random>

#include "Framebuffer.hpp"

#include "Specimen.hpp"

Specimen::Specimen()
{
    int stride = COMP_COUNT * sizeof(GLfloat);
    m_vao.SetAttribute(0, VertexArray::VEC3, m_vbo, stride);
    m_vao.SetAttribute(1, VertexArray::VEC3, m_vbo, stride, 3 * sizeof(GLfloat));
}

void Specimen::InitGenome(size_t circleCount)
{
    std::mt19937 rd(std::random_device{}());
    std::uniform_real_distribution<GLfloat> posDist(-1, 1),
                                            radDist(0, sqrt(2*2 + 2*2)),
                                            colDist(0, 1);


    m_genome.reserve(circleCount * COMP_COUNT);

    for (int ci = 0; ci < circleCount; ++ci) {
        int gi = COMP_COUNT * ci;
        m_genome[gi + 0] = posDist(rd);
        m_genome[gi + 1] = posDist(rd);

        m_genome[gi + 2] = radDist(rd);

        m_genome[gi + 3] = colDist(rd);
        m_genome[gi + 4] = colDist(rd);
        m_genome[gi + 5] = colDist(rd);
    }
}

// UB if genome is empty
void Specimen::Generate(glm::vec2 dimensions)
{
    m_buf[0].Generate(dimensions, nullptr);
    m_buf[1].Generate(dimensions, nullptr);

    Framebuffer fbo(dimensions);

    // Fill initial canvas with solid color
    fbo.AttachTexture(Framebuffer::COLOR, m_buf[0]);
    fbo.Bind();
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawArrays(GL_POINTS, 0, 1);

    m_vbo.BufferData(m_genome, VertexBuffer::DYNAMIC_DRAW);
}

const Texture& Specimen::GetTexture()
{
    return m_buf[0];
}
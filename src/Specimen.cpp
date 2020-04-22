#include <random>

#include "Framebuffer.hpp"
#include "ResourceManager.hpp"

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
                                            radDist(0, 0.5), //sqrt(2*2 + 2*2)),
                                            colDist(0, 1);

    m_circleCount = circleCount;
    m_genome.resize(circleCount * COMP_COUNT);

    for (int ci = 0; ci < circleCount; ++ci) {
        int gi = COMP_COUNT * ci;
        m_genome[gi + 0] = posDist(rd);
        m_genome[gi + 1] = posDist(rd);

        m_genome[gi + 2] = radDist(rd);

        m_genome[gi + 3] = colDist(rd);
        m_genome[gi + 4] = colDist(rd);
        m_genome[gi + 5] = colDist(rd);

        //fmt::print("pos(-1, 1): {} {}    radius: {}\n", m_genome[gi], m_genome[gi + 1], m_genome[gi + 2]);
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
    glClearColor(0.4, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);


    // Prepare data
    m_vbo.BufferData(m_genome, VertexBuffer::DYNAMIC_DRAW);

    auto shader = ResourceManager::GetShader("specimen_gen");
    shader.Use();
    shader.SetUniform("circle_detail", 20);
    shader.SetUniform("canvas", 0);
    m_buf[0].Bind(0);


    m_vao.Bind();

    for (int ci = 0; ci < m_circleCount; ++ci) {
        glDrawArrays(GL_POINTS, ci, 1);
    }
}

const Texture& Specimen::GetTexture()
{
    return m_buf[0];
}
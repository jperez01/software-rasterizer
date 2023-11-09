//
// Created by Juan Perez on 11/3/23.
//

#ifndef SOFTWARE_RASTERIZER_SHADER_H
#define SOFTWARE_RASTERIZER_SHADER_H

#include "utils/common.h"
#include "utils/geometry.h"

#include <functional>

template <typename Vertex, typename Varying, typename Uniforms>
struct Program {
    using VertexShader = std::function< void (const Uniforms& uniforms, const Vertex& in, Varying& out)>;
    using FragmentShader = std::function< Color (const Uniforms& uniforms, const Varying& in)>;

    void onVertex(const VertexShader& shader) { m_vertShader = shader; }
    void onFragment(const FragmentShader& shader) { m_pixelShader = shader; }

    void doVertexShader(const Vertex& in, Varying& out) { m_vertShader(m_uniforms, in, out); }
    Color doFragmentShader(const Varying& in) { return m_pixelShader(m_uniforms, in); }

    void setUniforms(Uniforms& uniforms) { m_uniforms = uniforms; }

public:
    VertexShader m_vertShader;
    FragmentShader m_pixelShader;
    Uniforms m_uniforms;
};

#endif //SOFTWARE_RASTERIZER_SHADER_H

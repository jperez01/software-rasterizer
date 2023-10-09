#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) {
    std::unordered_map<OBJVertex, uint32_t, OBJVertexHash> vertexMap;

    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<uint32_t> indices;
    std::vector<OBJVertex> vertices;

    std::string line_str;
    while (std::getline(in, line_str)) {
        std::istringstream line(line_str);

        std::string prefix;
        line >> prefix;

        if (prefix == "v") {
            glm::vec3 p;
            line >> p.x;
            line >> p.y;
            line >> p.z;

            positions.push_back(p);
        } else if (prefix == "vt") {
            glm::vec2 tc;
            line >> tc.x >> tc.y;

            texCoords.push_back(tc);
        } else if (prefix == "vn") {
            glm::vec3 n;
            line >> n.x >> n.y >> n.z;

            normals.push_back(n);
        } else if (prefix == "f") {
            std::string v1, v2, v3, v4;
            line >> v1 >> v2 >> v3 >> v4;
            OBJVertex verts[6];
            int nVertices = 3;

            verts[0] = OBJVertex(v1);
            verts[1] = OBJVertex(v2);
            verts[2] = OBJVertex(v3);

            if (!v4.empty()) {
                verts[3] = OBJVertex(v4);
                verts[4] = verts[0];
                verts[5] = verts[2];
                nVertices = 6;
            }

            for (int i =0; i < nVertices; i++) {
                const OBJVertex& v = verts[i];
                auto it = vertexMap.find(v);
                if (it == vertexMap.end()) {
                    vertexMap[v] = (uint32_t) vertices.size();
                    indices.push_back((uint32_t) vertices.size());
                    vertices.push_back(v);
                } else {
                    indices.push_back(it->second);
                }
            }
        }
    }

    m_faces.resize(indices.size());
    memcpy(m_faces.data(), indices.data(), sizeof(uint32_t) * indices.size());

    m_vertices.resize(vertices.size());
    for (uint32_t i = 0; i < vertices.size(); i++) {
        m_vertices[i] = positions.at(vertices[i].p - 1);
    }

    if (!normals.empty()) {
        m_normals.resize(vertices.size());
        for (uint32_t i=0; i<vertices.size(); ++i) {
            m_normals[i] = normals.at(vertices[i].n-1);
        }
    }

    if (!texCoords.empty()) {
        m_uvs.resize(vertices.size());
        for (uint32_t i = 0; i < vertices.size(); i++) {
            m_uvs[i] = texCoords.at(vertices[i].uv - 1);
        }
    }

    m_name = filename;
    std::cout << "done. (V=" << m_vertices.size() << ", F=" << m_faces.size() << ", took "
              << ")" << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)m_vertices.size();
}

int Model::nfaces() {
    return (int)m_faces.size();
}

uint32_t Model::face(int idx) {
    return m_faces[idx];
}

glm::vec3 Model::vert(int i) {
    return m_vertices[i];
}

glm::vec2 Model::texCoords(int index) {
    return m_uvs[index];
}


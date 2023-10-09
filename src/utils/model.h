#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "common.h"

struct OBJVertex {
    uint32_t p = (uint32_t) -1;
    uint32_t n = (uint32_t) -1;
    uint32_t uv = (uint32_t) -1;

    inline OBJVertex() = default;

    inline OBJVertex(const std::string& data) {
        std::vector<std::string> tokens = tokenize(data, "/", true);

        if (tokens.size() < 1 || tokens.size() > 3)
            std::cout << "Invalid vertex data: " << data << "\n";

        p = toUInt(tokens[0]);

        if (tokens.size() >= 2 && !tokens[1].empty())
            uv = toUInt(tokens[1]);

        if (tokens.size() >= 3 && !tokens[2].empty())
            n = toUInt(tokens[2]);
    }

    inline bool operator==(const OBJVertex& v) const {
        return v.p == p && v.n == n && v.uv == uv;
    }
};

struct OBJVertexHash {
    std::size_t operator()(const OBJVertex& v) const {
        size_t hash = std::hash<uint32_t>()(v.p);
        hash = hash * 37 + std::hash<uint32_t>()(v.n);
        hash = hash * 37 + std::hash<uint32_t>()(v.uv);

        return hash;
    }
};

class Model {
private:
    std::string m_name;

	std::vector<glm::vec3> m_vertices;
	std::vector<uint32_t> m_faces;
    std::vector<glm::vec2> m_uvs;
    std::vector<glm::vec3> m_normals;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	glm::vec3 vert(int i);
	uint32_t face(int idx);
    glm::vec2 texCoords(int index);
};

#endif //__MODEL_H__

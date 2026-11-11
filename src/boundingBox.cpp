#include "BoundingBox.h"

BoundingBox::BoundingBox(std::vector<Mesh>& meshes)
    : indices{
        0, 7,
        0, 6,
        0, 4,
        1, 2,
        1, 3,
        1, 5,
        2, 6,
        2, 7,
        3, 4,
        3, 7,
        4, 5,
        5, 6
    }
{
    iteracjaPoModelu(meshes);
    tworzenienaroznikow();
    naroznikiNaFloat();
    collisionBox.setVertices(narozniki_float);
    collisionBox.setIndices(indices);
    collisionBox.setCzyIndices(true);
    collisionBox.wywolajsetUp();
}

void BoundingBox::Draw(ShaderManager& shader, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& backpackModel) {
    shader.shaderRun();
    shader.uniformSetMat("setup.projection", projection);
    shader.uniformSetMat("setup.view", view);
    shader.uniformSetMat("setup.model", backpackModel);

    shader.uniformSetVec3("boxColor", glm::vec3(0.0f, 0.0f, 1.0f));
    collisionBox.DrawLines();
}

bool BoundingBox::CheckCollision(const glm::vec3& camPos, const glm::vec3& camFront) {
    return rayAABB(camPos, camFront);
}

void BoundingBox::iteracjaPoModelu(std::vector<Mesh>& meshes) {
    glm::vec3 VertexPositionTempMax;
    glm::vec3 VertexPositionTempMin;
    int i = 0;
    for (const auto& mesh : meshes) {
        for (const auto& Vertex : mesh.vertices) {
            if (i < 1) {
                VertexPositionTempMax = glm::vec3(Vertex.Position.x, Vertex.Position.y, Vertex.Position.z);
                VertexPositionTempMin = glm::vec3(Vertex.Position.x, Vertex.Position.y, Vertex.Position.z);
            }

            if (Vertex.Position.x >= VertexPositionTempMax.x) {
                VertexPositionTempMax.x = Vertex.Position.x;
            }
            else if (Vertex.Position.x <= VertexPositionTempMin.x) {
                VertexPositionTempMin.x = Vertex.Position.x;
            }

            if (Vertex.Position.y >= VertexPositionTempMax.y) {
                VertexPositionTempMax.y = Vertex.Position.y;
            }
            else if (Vertex.Position.y <= VertexPositionTempMin.y) {
                VertexPositionTempMin.y = Vertex.Position.y;
            }

            if (Vertex.Position.z >= VertexPositionTempMax.z) {
                VertexPositionTempMax.z = Vertex.Position.z;
            }
            else if (Vertex.Position.z <= VertexPositionTempMin.z) {
                VertexPositionTempMin.z = Vertex.Position.z;
            }

            i++;
        }
    }

    MinVec = VertexPositionTempMin;
    MaxVec = VertexPositionTempMax;
}

void BoundingBox::tworzenienaroznikow() {
    narozniki.emplace_back(MinVec);
    narozniki.emplace_back(MaxVec);

    narozniki.emplace_back(MaxVec.x, MaxVec.y, MinVec.z);
    narozniki.emplace_back(MaxVec.x, MinVec.y, MaxVec.z);
    narozniki.emplace_back(MinVec.x, MinVec.y, MaxVec.z);
    narozniki.emplace_back(MinVec.x, MaxVec.y, MaxVec.z);
    narozniki.emplace_back(MinVec.x, MaxVec.y, MinVec.z);
    narozniki.emplace_back(MaxVec.x, MinVec.y, MinVec.z);
}

void BoundingBox::naroznikiNaFloat() {
    for (const auto& naroznik : narozniki) {
        narozniki_float.emplace_back(naroznik.x);
        narozniki_float.emplace_back(naroznik.y);
        narozniki_float.emplace_back(naroznik.z);
    }
}

bool BoundingBox::rayAABB(const glm::vec3& camPos, const glm::vec3& camFront) {
    std::vector<glm::vec2> wyniki;
    float newMax;
    float newMin;
    float inf = std::numeric_limits<float>::infinity();
    glm::vec2 czWspolna;
    for (int i = 0; i < 3; i++) {
        newMax = MaxVec[i] - camPos[i];
        newMin = MinVec[i] - camPos[i];
        if (camFront[i] > 0) {
            newMax = newMax / camFront[i];
            newMin = newMin / camFront[i];
            wyniki.emplace_back(newMin, newMax);
        }
        else if (camFront[i] < 0) {
            newMax = newMax / camFront[i];
            newMin = newMin / camFront[i];
            wyniki.emplace_back(newMax, newMin);
        }
        else {
            if (MinVec[i] <= camPos[i] && MaxVec[i] >= camPos[i]) {
                wyniki.emplace_back(-inf, inf);
            }
            else {
                return false;
            }
        }
    }
    czWspolna.x = std::max(wyniki[0].x, wyniki[1].x);
    czWspolna.y = std::min(wyniki[0].y, wyniki[1].y);
    if (czWspolna.x > czWspolna.y) {
        return false;
    }
    if (czWspolna.y < 0) {
        return false;
    }
    czWspolna.x = std::max(czWspolna.x, wyniki[2].x);
    czWspolna.y = std::min(czWspolna.y, wyniki[2].y);
    if (czWspolna.x > czWspolna.y) {
        return false;
    }
    if (czWspolna.y < 0) {
        return false;
    }
    return true;
}



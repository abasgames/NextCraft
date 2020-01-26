//
// Created by twome on 26/01/2020.
//

#include "SectionMesh.h"
#include "../NextCraft.h"
#include "Vertices.h"

using namespace chunk;

SectionMesh::SectionMesh(Section *section) {
    this->section = section;
    this->xo = section->x << 4;
    this->yo = section->y << 4;
    this->zo = section->z << 4;
}

void SectionMesh::Render() {
    if (state == State::RebuildScheduled) {
        state = State::Waiting;
        AsyncMeshBuilder::Schedule(section);
    } else if (state == State::UploadScheduled) {
        Upload();
        state = State::Rendering;
    } else if (state == State::Rendering) {
        vao.Draw();
    }
}

void SectionMesh::Build() {
    if (state != State::Waiting)
        return;

    delete mesh;
    this->mesh = new Mesh(35000);

    for (int x = 0; x < 16; x++) {
        int absX = x + xo;
        for (int y = 0; y < 16; y++) {
            int absY = y + yo;
            for (int z = 0; z < 16; z++) {
                int absZ = z + zo;
                const BlockData &data = section->GetBlockData(x, y, z);
                const Block &me = *BlockRegistry::Get(data.id);

                int tx = me.sideTex.x;
                int ty = me.sideTex.y;

                if (ShouldRender(&me, x + 1, y, z, 0))
                    PutVertices(PosXVertices, PosXTextures, absX, absY, absZ, tx, ty, mesh);
                if (ShouldRender(&me, x - 1, y, z, 1))
                    PutVertices(NegXVertices, NegXTextures, absX, absY, absZ, tx, ty, mesh);

                if (ShouldRender(&me, x, y, z + 1, 2))
                    PutVertices(PosZVertices, PosZTextures, absX, absY, absZ, tx, ty, mesh);
                if (ShouldRender(&me, x, y, z - 1, 3))
                    PutVertices(NegZVertices, NegZTextures, absX, absY, absZ, tx, ty, mesh);

                tx = me.topTex.x;
                ty = me.topTex.y;
                if (ShouldRender(&me, x, y + 1, z, 4))
                    PutVertices(PosYVertices, PosZTextures, absX, absY, absZ, tx, ty, mesh);

                tx = me.bottomTex.x;
                ty = me.bottomTex.y;
                if (ShouldRender(&me, x, y - 1, z, 5))
                    PutVertices(NegYVertices, NegYTextures, absX, absY, absZ, tx, ty, mesh);
            }
        }
    }

    state = State::UploadScheduled;
}

void SectionMesh::Upload() {
    if (state == State::Deleted || state == State::DeleteScheduled)
        return;

    vao.SetData(*mesh);

    delete mesh;
    mesh = nullptr;
}

bool SectionMesh::ShouldRender(const Block *me, int x, int y, int z, int f) {
    uint8_t otherId = 0;
    if (x < 0 || y < 0 || z < 0 || x > 15 || y > 15 || z > 15)
        otherId = NextCraft::client->world.GetBlockData(xo + x, yo + y, zo + z).id;
    else otherId = section->GetBlockData(x, y, z).id;
    return otherId == 0;
}

void SectionMesh::PutVertices(const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &textures, int x, int y,
                              int z, int texX, int texY, Mesh *mesh) {
    for (int i = 0; i < vertices.size(); i += 3) {
        GLfloat vx = vertices[x];
        GLfloat vy = vertices[y];
        GLfloat vz = vertices[z];

        mesh->AddVertex(vx, vy, vz);
        mesh->AddColor(1.0f, 1.0f, 1.0f);
    }

    const GLfloat d = 0.03125;

    for (int i = 0; i < textures.size(); i += 2) {
        GLfloat u = (textures[i] + texX) * d;
        GLfloat v = (textures[i + 1] + texY) * d;
        mesh->AddTexCoords(u, v);
    }
}
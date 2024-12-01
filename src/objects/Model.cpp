#include "Model.hpp"
#include "../models/bushes.hpp"
#include "../models/cube.hpp"
#include "../models/plain.hpp"
#include "../models/skycube.hpp"
#include "../models/sphere.hpp"
#include "../models/tree.hpp"
#include <assimp/color4.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glext.h>
#include <glm/ext/vector_float4.hpp>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <ostream>
#include <system_error>

void Model::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model::Model() {
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
}

Model::~Model() {
    glDeleteBuffers(1, &this->VBO);
    glDeleteVertexArrays(1, &this->VAO);
}

void Model::load_model(const std::initializer_list<float> &vertices) const {
    this->bind_buffer(vertices);
    this->vertices_normal();
}

void Model::bind_buffer(const std::initializer_list<float> &vertices) const {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.begin(), GL_STATIC_DRAW);
}

void Model::vertices_normal() const {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(0 * sizeof(float)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(float)));
}

void Model::vertices_uv() const {
    glEnableVertexAttribArray(0); // enable vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(6 * sizeof(float)));
}

namespace models {

ObjectFile::ObjectFile(std::string file_name) : Model() {
    Assimp::Importer importer;
    uint import_options =
        aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace;

    const auto scene = importer.ReadFile(file_name, import_options);

    if (scene) {
        // Materials (printed just for fun)
        for (size_t i = 0; i < scene->mNumMaterials; i++) {
            const auto mat = scene->mMaterials[i];
            aiString name;
            mat->Get(AI_MATKEY_NAME, name);
            std::cout << "Material " << i << " name " << name.C_Str() << std::endl;
            aiColor4D d;
            glm::vec4 diffuse = glm::vec4(.8, .8, .8, 1);
            if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &d)) {
                diffuse = glm::vec4(d.r, d.g, d.b, d.a);
            }
        }

        // Objects
        for (size_t i = 0; i < scene->mNumMeshes; i++) {
            auto mesh = scene->mMeshes[i];
            auto vertices = std::make_unique<Vertex[]>(mesh->mNumVertices);
            // std::memset(vertices.get(), 0, sizeof(Vertex) * mesh->mNumVertices);
            std::cout << "Parsing mesh number: " << i << std::endl;

            for (size_t j = 0; j < mesh->mNumVertices; j++) {
                if (mesh->HasPositions()) {
                    vertices[j].position[0] = mesh->mVertices[j].x;
                    vertices[j].position[1] = mesh->mVertices[j].y;
                    vertices[j].position[2] = mesh->mVertices[j].z;
                }

                if (mesh->HasNormals()) {
                    vertices[j].normal[0] = mesh->mNormals[j].x;
                    vertices[j].normal[1] = mesh->mNormals[j].y;
                    vertices[j].normal[2] = mesh->mNormals[j].z;
                }

                if (mesh->HasTextureCoords(0)) {
                    vertices[j].texture[0] = mesh->mTextureCoords[0][j].x;
                    vertices[j].texture[1] = mesh->mTextureCoords[0][j].y;
                }

                if (mesh->HasTangentsAndBitangents()) {
                    vertices[j].tangent[0] = mesh->mTangents[j].x;
                    vertices[j].tangent[1] = mesh->mTangents[j].y;
                    vertices[j].tangent[2] = mesh->mTangents[j].z;
                }
            }
            GLuint IBO;
            glGenBuffers(1, &IBO);

            glBindVertexArray(this->VAO);
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->mNumVertices, vertices.get(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(0));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(3 * sizeof(GLfloat)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(6 * sizeof(GLfloat)));

            // Tangent for Normal Map
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(8 * sizeof(GLfloat)));

            // Index Buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            if (mesh->HasFaces()) {
                auto indices = std::make_unique<uint[]>(mesh->mNumFaces * 3);
                for (size_t j = 0; j < mesh->mNumFaces; j++) {
                    indices[j * 3] = mesh->mFaces[j].mIndices[0];
                    indices[j * 3 + 1] = mesh->mFaces[j].mIndices[1];
                    indices[j * 3 + 2] = mesh->mFaces[j].mIndices[2];
                }
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh->mNumFaces * 3, indices.get(),
                             GL_STATIC_DRAW);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(VAO);

            GLuint err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "GL ERROR: " << err << std::endl;
            }
            this->indices_count = mesh->mNumFaces * 3;
        }

    } else {
        std::cerr << "Error during parsing mesh from " << file_name << " : " << importer.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }
    glBindVertexArray(0);
}

void ObjectFile::render() {
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices_count, GL_UNSIGNED_INT, NULL);
}

auto triangle_points = {
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
};
Triangle::Triangle() : Model() {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, triangle_points.size() * sizeof(float), triangle_points.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Triangle::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Cube::Cube() : Model() {
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, cube_vertices.size() * sizeof(float), cube_vertices.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
}

void Cube::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

Tree::Tree() : Model() { this->load_model(tree_vertices); }

void Tree::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 92814);
}

Bush::Bush() : Model() { this->load_model(bushes_vertices); }

void Bush::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 8730);
}

Sphere::Sphere() : Model() { this->load_model(sphere_vertices); }

void Sphere::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 2880);
}

Plain::Plain() : Model() {
    this->bind_buffer(plain_vertices);
    this->vertices_uv();
}

void Plain::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

SkyCube::SkyCube() {
    this->bind_buffer(skycube_vertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
}

void SkyCube::render() {
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 108);
}

} // namespace models

#ifndef _MODEL_H_
#define _MODEL_H_

#include "CommonDefine.hpp"
#include "Mesh.h"

#include <vector>
#include <string>


namespace GLModule
{
	class Model
	{
	public:
		Model(const std::string& modelFile);
		Model(std::vector<VertexNS::Vertex> vertices, 
			std::vector<unsigned int> indices, 
			std::vector<Texture> textures,
			GLenum drawType = GL_TRIANGLES);

		bool initialize();
		void draw(Shader& shader);
		std::string modelFilePath();
		std::vector<Mesh>& getMeshs();
		void setMesh(std::vector<VertexNS::Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures,
			GLenum drawMode = GL_TRIANGLES);
		void setMeshByPosition(std::vector<glm::vec3> pos, GLenum drawMode = GL_TRIANGLES);

	private:
		bool loadModel(std::string const& path);
		void appendMesh(std::vector<VertexNS::Vertex> vertices, 
			std::vector<unsigned int> indices,
			std::vector<Texture> textures,
			GLenum drawType = GL_TRIANGLES);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		unsigned int TextureFromFile(const char* path, const std::string& directory);

	private:
		std::string m_modelFilePath;
		std::string m_directory;
		std::vector<Mesh> m_meshs;
		std::vector<Texture> m_loadedTextures;
	};
}

#endif
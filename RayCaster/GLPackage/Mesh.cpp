#include "Mesh.h"

using namespace std;

namespace GLModule
{
	Mesh::Mesh(std::vector<VertexNS::Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, GLenum drawMode)
	{
		m_vertices = vertices;
		m_indices = indices;
		m_textures = textures;
		setDrawMode(drawMode);
		drawMesh = [](){};
	}

	void Mesh::setDrawMode(GLenum drawType)
	{
		m_drawType = drawType;
	}

	bool Mesh::initialize()
	{
		return setupMesh();
	}

	bool Mesh::setupMesh()
	{
		// gen buffer
		GLuint vbo = -1;
		GLuint ebo = -1;
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &vbo); 
		glGenBuffers(1, &ebo);
		if (m_vao == -1 || vbo == -1 || ebo == -1)
			return false;

		// bind buffer
		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexNS::Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		if (!m_indices.empty())
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

		// enable data layout
		glEnableVertexAttribArray(VertexNS::POSITION);
		glVertexAttribPointer(VertexNS::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNS::Vertex), (void*)offsetof(VertexNS::Vertex, position));

		glEnableVertexAttribArray(VertexNS::NORMAL);
		glVertexAttribPointer(VertexNS::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNS::Vertex), (void*)offsetof(VertexNS::Vertex, normal));

		glEnableVertexAttribArray(VertexNS::UVCOORD);
		glVertexAttribPointer(VertexNS::UVCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNS::Vertex), (void*)offsetof(VertexNS::Vertex, uvCoord));

		// set draw api
		if (!m_indices.empty())
		{
			drawMesh = [&](){glDrawElements(m_drawType, m_indices.size(), GL_UNSIGNED_INT, 0); };
		}
		else
		{
			drawMesh = [&](){glDrawArrays(m_drawType, 0, m_vertices.size()); };
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Mesh::activeAllTexture(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (auto i = 0; i < m_textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			auto name = m_textures[i].type;
			string number;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to string
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to string
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to string
			auto textureName = name + number;
			shader.setUniformInt(textureName, i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		}
	}

	void Mesh::draw(Shader& shader)
	{
		// active texture
		activeAllTexture(shader);
		// draw mesh
		glBindVertexArray(m_vao);
		drawMesh();
	}

	const std::vector<VertexNS::Vertex>& Mesh::getVertices()
	{
		return m_vertices;
	}

	const std::vector<unsigned int>& Mesh::getIndices()
	{
		return m_indices;
	}
};

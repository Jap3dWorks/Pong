#include "pongTypes.h"
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <map>


#ifndef PRIMITIVE_SHAPES
#define PRIMITIVE_SHAPES

namespace Pong {
	class Shape {
	public:
		Shape(std::string name);
		virtual ~Shape() {}

		std::string getName() const { return _name; }

		unsigned int getVertexCount() const { return(unsigned int)_vertices.size() / 3; }
		unsigned int getNormalCount() const { return (unsigned int)_normals.size() / 3; }
		unsigned int getTexCoordsCount() const { return (unsigned int)_texCoords.size() / 2; }
		unsigned int getIndexCount() const { return _indices.size(); }
		unsigned int getLineIndexCount() const { return (unsigned int)_lineIndices.size(); }
		unsigned int getTriangleCount() const { return getIndexCount() / 3; }

		unsigned int getVertexSize() const { return (unsigned int)_vertices.size() * sizeof(float); }
		unsigned int getNormalSize() const { return (unsigned int)_normals.size() * sizeof(float); }
		unsigned int getTexCoordsSize() const { return (unsigned int)_texCoords.size() * sizeof(float); }
		unsigned int getIndexSize() const { return (unsigned int)_indices.size() * sizeof(unsigned int); }
		unsigned int getLineIndexSize() const { return (unsigned int)_lineIndices.size() * sizeof(unsigned int); }

		const float* getVertices() const { return _vertices.data(); }
		const float* getNormals() const { return _normals.data(); }
		const float* getTexCoords() const { return _texCoords.data(); }
		const unsigned int* getIndices() const { return _indices.data(); }
		const unsigned int* getLineIndices() const { return _lineIndices.data(); }

		const float * getInterleavedVertices() const { return _interleavedVertices.data(); }
		int getInterleavedVertexSize() const {
			return (unsigned int)_interleavedVertices.size() * sizeof(float);
		}
		unsigned int getInterleavedVertexCount() const { return getVertexCount(); }

		unsigned int getVAO() const { return _VAO; }

		void setVAO();

		virtual void draw() const;

	protected:
		unsigned int _VAO = 0;

		std::string _name;
		std::vector<float> _vertices;
		std::vector<float> _normals;
		std::vector<float> _texCoords;
		std::vector<unsigned int> _indices;
		std::vector<unsigned int> _lineIndices;

		std::vector<float> _interleavedVertices;
		int _interleavedStride = 32;  // (pos + normal + txtcoords) * 4

		// --methods--
		void _addVertex(glm::vec3 v) { _vertices.push_back(v.x); _vertices.push_back(v.y); _vertices.push_back(v.z); }
		void _addVertex(float x, float y, float z) { _addVertex(glm::vec3(x, y, z)); }
		void _addNormal(glm::vec3 n) { _normals.push_back(n.x); _normals.push_back(n.y); _normals.push_back(n.z); }
		void _addNormal(float x, float y, float z) { _normals.push_back(x); _normals.push_back(y); _normals.push_back(z); }
		void _addTexCoord(glm::vec2 t) { _texCoords.push_back(t.x); _texCoords.push_back(t.y); }
		void _addTexCoord(float u, float v) { _texCoords.push_back(u); _texCoords.push_back(v); }
		void _addIndices(unsigned int i, unsigned int j, unsigned int k) 
		{ _indices.push_back(i); _indices.push_back(j); _indices.push_back(k); }
		void _buildInterleavedVertices();
	};

	// Icosphere
	class IcosphereShape : public Shape
	{
	public:
		IcosphereShape(std::string name, float radius=1.f, int subdivision=1, bool smooth=true); // no smooth is not implemented
		virtual ~IcosphereShape();

		void setRadius(float r);

	private:
		// --members--
		float _radius;
		int _subdivision;
		bool _smooth;
		std::map<std::pair<float, float>, unsigned int> _sharedIndices;
		
		// -----------
		// --methods--
		// -----------
		/**
			Build a smooth icosahedron, the basic icosahedron has 12 vertices, posterior subdivision
			are builded from this basic geometry, pushing new vertices using the radius length.*/
		void _buildVerticesSmooth();

		void buildIcosphere();

		/**
			Build a facethed icosahedron
		*/
		// void _buildVerticesFlat();

		/**
			Compute the 12 basic vertices of an icosahedron
		*/
		std::vector<glm::vec3> _computeIcosahedronVertices();

		/**
			subdivide the icosahedron vertices with smooth results
		*/
		void _subdivideVerticesSmooth();

		void computeHalfVertex(const float v1[3], const float v2[3], float length, float nv[3]);
		void _computeHalfTexCoord(const float t1[2], const float t2[2], float nt[2]);
		void _computeVertexNormal(const float v[3], float n[3]);
		/**
			add vertex to arrays
		*/
		unsigned int _addSubVertexAttribs(const float v[3], const float n[3], const float t[2]);

		void _addSubLineIndices(unsigned int i1,
			unsigned int i2,
			unsigned int i3,
			unsigned int i4,
			unsigned int i5,
			unsigned int i6);
		
		// --------------------
		// --static functions--
		// --------------------
		static bool _isSharedTexCoord(const float t[2]);
		
		/**
			determine a point c is on the segment a-b
		*/
		static bool _isOnLineSegment(const float a[2], const float b[2], const float c[2]);

	};

	class CubeShape : public Shape
	{
	private:
		float _width;
		float _height;
		float _depth;

		void _buildCubeVerticesHard();
		//void _buildCubeVertices();
		std::vector<glm::vec3> _computeCubeVertices();

		std::map<std::pair<glm::vec3, glm::vec2>, unsigned int> _sharedIndices;

	public:
		CubeShape(std::string name, float height=1.f, float width=1.f, float depth=1.f);

		virtual ~CubeShape();
	};


}
#endif // PRIMITIVE_SHAPES




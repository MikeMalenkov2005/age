#ifndef AGE_HPP
#define AGE_HPP

#ifdef AGE_EXPORTS
#define AGE_API __declspec(dllexport)
#else
#define AGE_API __declspec(dllimport)
#endif

#include <GL/glew.h>
#include <string>
#include <fstream>

#include "age_math.hpp"

namespace age
{
	namespace rp
	{
		struct AGE_API BufSize
		{
			size_t patch, array, element;
			BufSize(size_t patch, size_t array, size_t element);
			size_t getPatchByteSize();
			size_t getArrayByteSize();
			size_t getPatchCount();
		};

		class AGE_API GLBuffer
		{
		private:
			GLuint id;
			BufSize size;
			GLenum type;
		public:
			GLBuffer(BufSize size, GLenum type, const void* data, GLbitfield flags);
			GLBuffer(size_t patchSize, size_t length, const float* data, GLbitfield flags);
			GLBuffer(size_t patchSize, size_t length, const double* data, GLbitfield flags);
			GLBuffer(size_t patchSize, size_t length, const int* data, GLbitfield flags);
			GLBuffer(size_t patchSize, size_t length, const uint* data, GLbitfield flags);
			~GLBuffer();
			GLuint getId();
			BufSize getSize();
			GLenum getType();
		};

		class AGE_API Mesh
		{
		private:
			GLuint id;
			GLuint ebo;
			size_t count;
		public:
			Mesh(size_t vboCount, GLBuffer* vbos[], size_t indexCount, uint* indices);
			~Mesh();
			void setVBO(GLuint binding, GLBuffer* vbo);
			void draw();
		};

		enum UniformType
		{
			SINGLE,
			VECTOR2,
			VECTOR3,
			VECTOR4,
			MATRIX2,
			MATRIX2_T,
			MATRIX3,
			MATRIX3_T,
			MATRIX4,
			MATRIX4_T
		};

        enum UniformException
        {
            NO_EXCEPTION,
            INVALID_LOCATION,
            INVALID_TYPE
        };

		class AGE_API Shader
		{
		private:
			size_t count;
			GLuint program;
			GLuint* shaders;
		public:
			Shader(size_t count, const GLuint* shaders);
			~Shader();
			void bind();
			void unbind();
			UniformException loadUniform(std::string name, UniformType type, size_t count, const float* data);
			UniformException loadUniform(std::string name, UniformType type, size_t count, const double* data);
			UniformException loadUniform(std::string name, UniformType type, size_t count, const int* data);
			UniformException loadUniform(std::string name, UniformType type, size_t count, const uint* data);
			static GLuint CreateShader(GLenum type, std::string src);
			static GLuint CreateShader(GLenum type, std::istream& input);
			static void DeleteShader(GLuint detachedShader);
		};
	}
}

#endif

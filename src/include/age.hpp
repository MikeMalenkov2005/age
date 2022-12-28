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

#include <glm/glm.hpp>

typedef unsigned int uint;

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
			GLint GetUniformLocation(std::string name);
			static GLuint CreateShader(GLenum type, std::string src);
			static GLuint CreateShader(GLenum type, std::istream& input);
			static void DeleteShader(GLuint detachedShader);
		};
	}
}

#endif

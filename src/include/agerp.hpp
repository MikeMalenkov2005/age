#ifndef AGE_RENDERING_HPP
#define AGE_RENDERING_HPP

#include "agedef.hpp"

namespace age
{
	namespace rp
	{
		struct AGE_API BufSize
		{
			size_t patch, array, element;
			BufSize(size_t patch, size_t array, size_t element);
			size_t GetPatchByteSize();
			size_t GetArrayByteSize();
			size_t GetPatchCount();
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
			GLuint GetId();
			BufSize GetSize();
			GLenum GetType();
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
			void SetVBO(GLuint binding, GLBuffer* vbo);
			void Draw();
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
			void Bind();
			void Unbind();
			GLint GetUniformLocation(std::string name);
			static GLuint CreateShader(GLenum type, std::string src);
			static GLuint CreateShader(GLenum type, std::istream& input);
			static void DeleteShader(GLuint detachedShader);
		};
	}
}

#endif

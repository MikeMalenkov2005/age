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
			void SetData(int offset, size_t size, const void* data);
		};

		class AGE_API Mesh
		{
		private:
			GLuint id;
			GLuint ebo;
			size_t count;
		public:
			Mesh(size_t vboCount, GLBuffer* vbos[], size_t indexCount, const uint* indices);
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

		class AGE_API Texture
		{
		private:
			GLuint id;
			GLenum type, format, filter, wrapMode;
			int width, height, depth, levels;
		public:
			Texture(GLenum type, int width, int height, int depth, int levels, GLenum format, GLenum filter, GLenum wrapMode);
			Texture(int width, int levels, GLenum format, GLenum filter, GLenum wrapMode);
			Texture(int width, int height, int levels, GLenum format, GLenum filter, GLenum wrapMode);
			Texture(int width, int height, int depth, int levels, GLenum format, GLenum filter, GLenum wrapMode);
			static Texture* CubeMap(int width, int height, GLenum format, GLenum filter, GLenum wrapMode);
			~Texture();
			GLuint GetId();
			GLenum GetType();
			GLenum GetFormat();
			GLenum GetFilter();
			void SetFilter(GLenum filter);
			GLenum GetWrapMode();
			void SetWrapMode(GLenum filter);
			int GetWidth();
			int GetHeight();
			int GetDepth();
			void SetData(int level, int x, int y, int z, int width, int height, int depth, GLenum format, GLenum type, const void* data);
			bool SetData1D(int level, int x, int width, GLenum format, GLenum type, const void* data);
			bool SetData2D(int level, int x, int y, int width, int height, GLenum format, GLenum type, const void* data);
			bool SetData3D(int level, int x, int y, int z, int width, int height, int depth, GLenum format, GLenum type, const void* data);
			bool SetDataCubeMap(int level, int x, int y, int face, int width, int height, GLenum format, GLenum type, const void* data);
			void GenerateMipmap();
			void Bind(int index);
			void Unbind(int index);
			void BindImage(uint index, int level, GLboolean layered, int layer, GLenum access);
			void UnbindImage(uint index, int level, GLboolean layered, int layer, GLenum access);
		};
	}
}

#endif

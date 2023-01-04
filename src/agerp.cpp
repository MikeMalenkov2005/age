#include "include/agerp.hpp"

namespace age
{
	namespace rp
	{
		BufSize::BufSize(size_t patch, size_t array, size_t element) { this->patch = patch; this->array = array; this->element = element; }
		size_t BufSize::GetPatchByteSize() { return this->patch * this->element; }
		size_t BufSize::GetArrayByteSize() { return this->array * this->element; }
		size_t BufSize::GetPatchCount() { return this->array / this->patch; }

		GLBuffer::GLBuffer(BufSize size, GLenum type, const void* data, GLbitfield flags) : size(size.patch, size.array, size.element)
		{
			this->type = type;
			glCreateBuffers(1, &this->id);
			glNamedBufferStorage(this->id, size.GetArrayByteSize(), data, flags);
		}
		GLBuffer::GLBuffer(size_t patchSize, size_t length, const float* data, GLbitfield flags) : GLBuffer(BufSize(patchSize, length, sizeof(float)), GL_FLOAT, data, flags) {}
		GLBuffer::GLBuffer(size_t patchSize, size_t length, const double* data, GLbitfield flags) : GLBuffer(BufSize(patchSize, length, sizeof(double)), GL_DOUBLE, data, flags) {}
		GLBuffer::GLBuffer(size_t patchSize, size_t length, const int* data, GLbitfield flags) : GLBuffer(BufSize(patchSize, length, sizeof(int)), GL_INT, data, flags) {}
		GLBuffer::GLBuffer(size_t patchSize, size_t length, const uint* data, GLbitfield flags) : GLBuffer(BufSize(patchSize, length, sizeof(uint)), GL_UNSIGNED_INT, data, flags) {}
		GLBuffer::~GLBuffer() { glDeleteBuffers(1, &this->id); }
		GLuint GLBuffer::GetId() { return this->id; }
		BufSize GLBuffer::GetSize() { return this->size; }
		GLenum GLBuffer::GetType() { return this->type; }
		void GLBuffer::SetData(int offset, size_t size, const void* data)
		{
			glNamedBufferSubData(this->id, offset, size, data);
		}

		Mesh::Mesh(size_t vboCount, GLBuffer* vbos[], size_t indexCount, uint* indices)
		{
			glCreateBuffers(1, &this->ebo);
			glNamedBufferData(this->ebo, indexCount * sizeof(uint), indices, GL_STATIC_DRAW);
			this->count = indexCount;

			glCreateVertexArrays(1, &this->id);

			for (GLuint i = 0; i < vboCount; i++)
			{
				glEnableVertexArrayAttrib(this->id, i);
				glVertexArrayAttribBinding(this->id, i, i);
				glVertexArrayAttribFormat(this->id, i, vbos[i]->GetSize().patch, vbos[i]->GetType(), GL_FALSE, 0);
				glVertexArrayVertexBuffer(this->id, i, vbos[i]->GetId(), 0, vbos[i]->GetSize().GetPatchByteSize());
			}

			glVertexArrayElementBuffer(this->id, this->ebo);
		}
		Mesh::~Mesh()
		{
			glDeleteVertexArrays(1, &this->id);
			glDeleteBuffers(1, &this->ebo);
		}
		void Mesh::SetVBO(GLuint binding, GLBuffer* vbo)
		{
			glEnableVertexArrayAttrib(this->id, binding);
			glVertexArrayAttribBinding(this->id, binding, binding);
			glVertexArrayAttribFormat(this->id, binding, vbo->GetSize().patch, vbo->GetType(), GL_FALSE, 0);
			glVertexArrayVertexBuffer(this->id, binding, vbo->GetId(), 0, vbo->GetSize().GetPatchByteSize());
		}
		void Mesh::Draw()
		{
			glBindVertexArray(this->id);
			glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		GLuint boundShaderProgram = 0;
		Shader::Shader(size_t count, const GLuint* shaders)
		{
			this->count = count;
			this->program = glCreateProgram();
			this->shaders = new GLuint[count];
			for (size_t i = 0; i < count; i++)
			{
				this->shaders[i] = shaders[i];
				glAttachShader(this->program, shaders[i]);
			}
			glLinkProgram(program);
		}
		Shader::~Shader()
		{
			this->Unbind();
			for (size_t i = 0; i < this->count; i++)
			{
				glDetachShader(this->program, this->shaders[i]);
			}
			glDeleteProgram(this->program);
			delete[] this->shaders;
		}
		void Shader::Bind()
		{
			if (boundShaderProgram != this->program)
			{
				boundShaderProgram = this->program;
				glUseProgram(boundShaderProgram);
			}
		}
		void Shader::Unbind()
		{
			if (boundShaderProgram == this->program)
			{
				boundShaderProgram = 0;
				glUseProgram(0);
			}
		}
		GLint Shader::GetUniformLocation(std::string name)
		{
			return glGetUniformLocation(this->program, name.c_str());
		}
		GLuint Shader::CreateShader(GLenum type, std::string src)
		{
			int length = src.length();
			const char* cstr = src.c_str();
			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, &cstr, &length);
			glCompileShader(shader);
			return shader;
		}
		GLuint Shader::CreateShader(GLenum type, std::istream& input)
		{
			std::string src(std::istreambuf_iterator<char>(input), {});
			return Shader::CreateShader(type, src);
		}
		void Shader::DeleteShader(GLuint detachedShader)
		{
			glDeleteShader(detachedShader);
		}

		Texture::Texture(int width, int levels, GLenum format, GLenum filter, GLenum wrapMode)
		{
			this->type = GL_TEXTURE_1D;
			this->width = width;
			this->height = 1;
			this->depth = 1;
			this->levels = levels;
			this->format = format;
			glCreateTextures(GL_TEXTURE_1D, 1, &this->id);
			glTextureStorage1D(this->id, levels, format, width);
			this->SetWrapMode(wrapMode);
			this->SetFilter(filter);
		}
		Texture::Texture(int width, int height, int levels, GLenum format, GLenum filter, GLenum wrapMode)
		{
			this->type = GL_TEXTURE_2D;
			this->width = width;
			this->height = height;
			this->depth = 1;
			this->levels = levels;
			this->format = format;
			glCreateTextures(GL_TEXTURE_2D, 1, &this->id);
			glTextureStorage2D(this->id, levels, format, width, height);
			this->SetWrapMode(wrapMode);
			this->SetFilter(filter);
		}
		Texture::Texture(int width, int height, int depth, int levels, GLenum format, GLenum filter, GLenum wrapMode)
		{
			this->type = GL_TEXTURE_3D;
			this->width = width;
			this->height = height;
			this->depth = depth;
			this->levels = levels;
			this->format = format;
			glCreateTextures(GL_TEXTURE_3D, 1, &this->id);
			glTextureStorage3D(this->id, levels, format, width, height, depth);
			this->SetWrapMode(wrapMode);
			this->SetFilter(filter);
		}
		Texture::~Texture()
		{
			glDeleteTextures(1, &this->id);
		}
		GLuint Texture::GetId()
		{
			return this->id;
		}
		GLenum Texture::GetType()
		{
			return this->type;
		}
		GLenum Texture::GetFormat()
		{
			return this->format;
		}
		GLenum Texture::GetFilter()
		{
			return this->filter;
		}
		void Texture::SetFilter(GLenum filter)
		{
			this->filter = filter;
			glTextureParameteri(this->id, GL_TEXTURE_MIN_FILTER, filter);
			glTextureParameteri(this->id, GL_TEXTURE_MAG_FILTER, filter);
		}
		GLenum Texture::GetWrapMode()
		{
			return this->wrapMode;
		}
		void Texture::SetWrapMode(GLenum wrapMode)
		{
			this->wrapMode = wrapMode;
			glTextureParameteri(this->id, GL_TEXTURE_WRAP_S, wrapMode);
			glTextureParameteri(this->id, GL_TEXTURE_WRAP_T, wrapMode);
			glTextureParameteri(this->id, GL_TEXTURE_WRAP_R, wrapMode);
		}
		int Texture::GetWidth()
		{
			return this->width;
		}
		int Texture::GetHeight()
		{
			return this->height;
		}
		int Texture::GetDepth()
		{
			return this->depth;
		}
		void Texture::SetData(int level, int x, int y, int z, int width, int height, int depth, GLenum format, GLenum type, const void* data)
		{
			switch (this->type)
			{
			case GL_TEXTURE_1D:
				glTextureSubImage1D(this->id, level, x, width, format, type, data);
				break;
			case GL_TEXTURE_2D:
				glTextureSubImage2D(this->id, level, x, y, width, height, format, type, data);
				break;
			case GL_TEXTURE_3D:
				glTextureSubImage3D(this->id, level, x, y, z, width, height, depth, format, type, data);
				break;
			default:
				break;
			}
		}
		void Texture::GenerateMipmap()
		{
			glGenerateTextureMipmap(this->id);
		}
	}
}

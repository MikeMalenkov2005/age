#include "include/age.hpp"

namespace age
{
	namespace rp
	{
		BufSize::BufSize(size_t patch, size_t array, size_t element) { this->patch = patch; this->array = array; this->element = element; }
		size_t BufSize::getPatchByteSize() { return this->patch * this->element; }
		size_t BufSize::getArrayByteSize() { return this->array * this->element; }
		size_t BufSize::getPatchCount() { return this->array / this->patch; }

		GLBuffer::GLBuffer(BufSize size, GLenum type, const void* data, GLbitfield flags) : size(size.patch, size.array, size.element)
		{
			this->type = type;
			glCreateBuffers(1, &this->id);
			glNamedBufferStorage(this->id, size.getArrayByteSize(), data, flags);
		}
		GLBuffer::GLBuffer(size_t patchSize, size_t length, const float* data, GLbitfield flags) : GLBuffer(BufSize(patchSize, length, sizeof(float)), GL_FLOAT, data, flags) {}
		GLBuffer::GLBuffer(size_t patchSize, size_t length, const double* data, GLbitfield flags) : GLBuffer(BufSize(patchSize, length, sizeof(double)), GL_DOUBLE, data, flags) {}
		GLBuffer::GLBuffer(size_t patchSize, size_t length, const int* data, GLbitfield flags) : GLBuffer(BufSize(patchSize, length, sizeof(int)), GL_INT, data, flags) {}
		GLBuffer::GLBuffer(size_t patchSize, size_t length, const uint* data, GLbitfield flags) : GLBuffer(BufSize(patchSize, length, sizeof(uint)), GL_UNSIGNED_INT, data, flags) {}
		GLBuffer::~GLBuffer() { glDeleteBuffers(1, &this->id); }
		GLuint GLBuffer::getId() { return this->id; }
		BufSize GLBuffer::getSize() { return this->size; }
		GLenum GLBuffer::getType() { return this->type; }

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
				glVertexArrayAttribFormat(this->id, i, vbos[i]->getSize().patch, vbos[i]->getType(), GL_FALSE, 0);
				glVertexArrayVertexBuffer(this->id, i, vbos[i]->getId(), 0, vbos[i]->getSize().getPatchByteSize());
			}

			glVertexArrayElementBuffer(this->id, this->ebo);
		}
		Mesh::~Mesh()
		{
			glDeleteVertexArrays(1, &this->id);
			glDeleteBuffers(1, &this->ebo);
		}
		void Mesh::setVBO(GLuint binding, GLBuffer* vbo)
		{
			glEnableVertexArrayAttrib(this->id, binding);
			glVertexArrayAttribBinding(this->id, binding, binding);
			glVertexArrayAttribFormat(this->id, binding, vbo->getSize().patch, vbo->getType(), GL_FALSE, 0);
			glVertexArrayVertexBuffer(this->id, binding, vbo->getId(), 0, vbo->getSize().getPatchByteSize());
		}
		void Mesh::draw()
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
			this->unbind();
			for (size_t i = 0; i < this->count; i++)
			{
				glDetachShader(this->program, this->shaders[i]);
			}
			glDeleteProgram(this->program);
			delete[] this->shaders;
		}
		void Shader::bind()
		{
			if (boundShaderProgram != this->program)
			{
				boundShaderProgram = this->program;
				glUseProgram(boundShaderProgram);
			}
		}
		void Shader::unbind()
		{
			if (boundShaderProgram == this->program)
			{
				boundShaderProgram = 0;
				glUseProgram(0);
			}
		}
		UniformException Shader::loadUniform(std::string name, UniformType type, size_t count, const float* data)
		{
            GLint location = glGetUniformLocation(this->program, name.c_str());
            if (location == -1) return INVALID_LOCATION;
			switch (type)
			{
			case SINGLE:
				glUniform1fv(location, count, data);
				break;
			case VECTOR2:
				glUniform2fv(location, count, data);
				break;
			case VECTOR3:
				glUniform3fv(location, count, data);
				break;
			case VECTOR4:
				glUniform4fv(location, count, data);
				break;
			case MATRIX2:
				glUniformMatrix2fv(location, count, GL_FALSE, data);
				break;
			case MATRIX2_T:
				glUniformMatrix2fv(location, count, GL_TRUE, data);
				break;
			case MATRIX3:
				glUniformMatrix3fv(location, count, GL_FALSE, data);
				break;
			case MATRIX3_T:
				glUniformMatrix3fv(location, count, GL_TRUE, data);
				break;
			case MATRIX4:
				glUniformMatrix4fv(location, count, GL_FALSE, data);
				break;
			case MATRIX4_T:
				glUniformMatrix4fv(location, count, GL_TRUE, data);
				break;
			default:
                return INVALID_TYPE;
				break;
			}
            return NO_EXCEPTION;
		}
		UniformException Shader::loadUniform(std::string name, UniformType type, size_t count, const double* data)
		{
            GLint location = glGetUniformLocation(this->program, name.c_str());
            if (location == -1) return INVALID_LOCATION;
			switch (type)
			{
			case SINGLE:
				glUniform1dv(location, count, data);
				break;
			case VECTOR2:
				glUniform2dv(location, count, data);
				break;
			case VECTOR3:
				glUniform3dv(location, count, data);
				break;
			case VECTOR4:
				glUniform4dv(location, count, data);
				break;
			case MATRIX2:
				glUniformMatrix2dv(location, count, GL_FALSE, data);
				break;
			case MATRIX2_T:
				glUniformMatrix2dv(location, count, GL_TRUE, data);
				break;
			case MATRIX3:
				glUniformMatrix3dv(location, count, GL_FALSE, data);
				break;
			case MATRIX3_T:
				glUniformMatrix3dv(location, count, GL_TRUE, data);
				break;
			case MATRIX4:
				glUniformMatrix4dv(location, count, GL_FALSE, data);
				break;
			case MATRIX4_T:
				glUniformMatrix4dv(location, count, GL_TRUE, data);
				break;
			default:
                return INVALID_TYPE;
				break;
			}
            return NO_EXCEPTION;
		}
		UniformException Shader::loadUniform(std::string name, UniformType type, size_t count, const int* data)
		{
            GLint location = glGetUniformLocation(this->program, name.c_str());
            if (location == -1) return INVALID_LOCATION;
			switch (type)
			{
			case SINGLE:
				glUniform1iv(location, count, data);
				break;
			case VECTOR2:
				glUniform2iv(location, count, data);
				break;
			case VECTOR3:
				glUniform3iv(location, count, data);
				break;
			case VECTOR4:
				glUniform4iv(location, count, data);
				break;
			default:
                return INVALID_TYPE;
				break;
			}
            return NO_EXCEPTION;
		}
		UniformException Shader::loadUniform(std::string name, UniformType type, size_t count, const uint* data)
		{
            GLint location = glGetUniformLocation(this->program, name.c_str());
            if (location == -1) return INVALID_LOCATION;
			switch (type)
			{
			case SINGLE:
				glUniform1uiv(location, count, data);
				break;
			case VECTOR2:
				glUniform2uiv(location, count, data);
				break;
			case VECTOR3:
				glUniform3uiv(location, count, data);
				break;
			case VECTOR4:
				glUniform4uiv(location, count, data);
				break;
			default:
                return INVALID_TYPE;
				break;
			}
            return NO_EXCEPTION;
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
	}
}

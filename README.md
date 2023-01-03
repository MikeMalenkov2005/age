# AGE

AGE is an opensource game engine based on OpenGL rendering and custom physics

## Usage

Initialize, create a window and make its context current:
```c++
age::wnd::Init();
age::wnd::Window* window = new age::wnd::Window(title, x, y, width, height);
window->MakeContextCurrent();
```

Update the window in while it is opened:
```c++

while (!(window->ShouldClose()))
{
    window->HandleEvents();
    // Do some OpenGL stuff
}

```

Create and bind shaders:
```c++

GLuint shaders[2];

// create a vertex shader with std::istream source
std::istream src = ...;
shaders[0] = age::rp::Shader::CreateShader(GL_VERTEX_SHADER, src);

// create a fragment shader with std::string source
std::string src2 = ...;
shaders[1] = age::rp::Shader::CreateShader(GL_FRAGMENT_SHADER, src2);

// create a shader program
age::rp::Shader* program = new Shader(2, shaders);

// bind program before using
program->Bind();

// unbind program after using
program->Unbind();

// delete shader program when no longer needed
delete program;

// delete shaders when no longer needed
age::rp::Shader::DeleteShader(shaders[0]);
age::rp::Shader::DeleteShader(shaders[1]);

```

## License

[MIT](https://choosealicense.com/licenses/mit/)

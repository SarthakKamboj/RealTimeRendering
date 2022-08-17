#include "helper.h"
#include "glad/glad.h"
#include <exception>
#include <iostream>

#define ERR_HANDLING(err) std::cout << err << std::endl;

void detect_opengl_error() {
	GLenum error = glGetError();
	switch (error) {
	case GL_NO_ERROR:
		ERR_HANDLING("No opengl error at this point")
			break;
	case GL_INVALID_ENUM:
		ERR_HANDLING("An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.")
			break;
	case GL_INVALID_VALUE:
		ERR_HANDLING("A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.")
			break;
	case GL_INVALID_OPERATION:
		ERR_HANDLING("The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		ERR_HANDLING("The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.");
		break;
	case GL_OUT_OF_MEMORY:
		ERR_HANDLING("There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded");
		break;
	case GL_STACK_UNDERFLOW:
		ERR_HANDLING("An attempt has been made to perform an operation that would cause an internal stack to underflow.");
		break;
	case GL_STACK_OVERFLOW:
		ERR_HANDLING("An attempt has been made to perform an operation that would cause an internal stack to overflow.");
		break;
	}
}

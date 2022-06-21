#include <GL/glew.h>
//
#include "GLFW/glfw3.h"
#include "Logger.h"

bool Logger::Check_GL_ErrorCode()
{
    GLenum errorCode;
    GLenum lastErrorCode = GL_NO_ERROR;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        lastErrorCode = errorCode;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:
                m_logger.error("INVALID_ENUM");
                break;
            case GL_INVALID_VALUE:
                m_logger.error("INVALID_VALUE");
                break;
            case GL_INVALID_OPERATION:
                m_logger.error("INVALID_OPERATION");
                break;
            case GL_STACK_OVERFLOW:
                m_logger.error("STACK_OVERFLOW");
                break;
            case GL_STACK_UNDERFLOW:
                m_logger.error("STACK_UNDERFLOW");
                break;
            case GL_OUT_OF_MEMORY:
                m_logger.error("OUT_OF_MEMORY");
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                m_logger.error("INVALID_FRAME_BUFFER_OPERATION");
                break;
        }
    }

    return lastErrorCode == GL_NO_ERROR ? true : false;
}

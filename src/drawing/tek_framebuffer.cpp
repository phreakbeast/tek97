#include "tek_framebuffer.hpp"

#include "../platform/tek_platform.hpp"

#include <stdio.h>
#include <assert.h>

namespace tek
{
	TekFramebuffer::TekFramebuffer(u32 width, u32 height)
	{
		assert(width > 0 && height > 0);

		this->width = width;
		this->height = height;
		this->fbo = 0;
		this->tex_id = 0;
		this->depth_id = 0;

		//Texture
		GLCall(glGenTextures(1, &tex_id));
		GLCall(glBindTexture(GL_TEXTURE_2D, tex_id));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		//Depth Texture
		GLCall(glGenTextures(1, &depth_id));
		GLCall(glBindTexture(GL_TEXTURE_2D, depth_id));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
		                    GL_UNSIGNED_BYTE, 0));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		//Frame buffer
		GLCall(glGenFramebuffers(1, &fbo));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_id, 0));


		GLCall(GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("ERROR: couldn't create framebuffer.\nError: ");
			if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
			{
				printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
			}
			else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			{
				printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
			}
			else if (status == GL_FRAMEBUFFER_UNSUPPORTED)
			{
				printf("GL_FRAMEBUFFER_UNSUPPORTED\n");
			}
			else
			{
				printf("Other\n");
			}
		}

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	TekFramebuffer::~TekFramebuffer()
	{
		GLCall(glDeleteFramebuffers(1, &fbo));
		GLCall(glDeleteTextures(1, &depth_id));
		GLCall(glDeleteTextures(1, &tex_id));
	}

	void TekFramebuffer::bind_reading(u32 texture_unit)
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
		GLCall(glBindTexture(GL_TEXTURE_2D, tex_id));
	}

	void TekFramebuffer::bind_writing()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
		GLCall(glViewport(0, 0, width, height));
	}

	void TekFramebuffer::unbind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

}

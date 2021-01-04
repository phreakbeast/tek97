#include "tek_framebuffer.hpp"

#include "../platform/tek_platform.hpp"

#include <stdio.h>
#include <assert.h>

void tek_fb_init(TekFramebuffer* fb, u32 width, u32 height)
{
	assert(width > 0 && height > 0);

	fb->width = width;
	fb->height = height;
	fb->fbo = 0;
	fb->tex_id = 0;
	fb->depth_id = 0;

	//Texture
	GLCall(glGenTextures(1, &fb->tex_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, fb->tex_id));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	//Depth Texture
	GLCall(glGenTextures(1, &fb->depth_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, fb->depth_id));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT,
	                    GL_UNSIGNED_BYTE, 0));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	//Frame buffer
	GLCall(glGenFramebuffers(1, &fb->fbo));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fb->tex_id, 0));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fb->depth_id, 0));


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

void tek_fb_destroy(TekFramebuffer* fb)
{
	GLCall(glDeleteFramebuffers(1, &fb->fbo));
	GLCall(glDeleteTextures(1, &fb->depth_id));
	GLCall(glDeleteTextures(1, &fb->tex_id));
}

void tek_fb_bind_reading(TekFramebuffer* fb, u32 texture_unit)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + texture_unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, fb->tex_id));
}

void tek_fb_bind_writing(TekFramebuffer* fb)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo));
	GLCall(glViewport(0, 0, fb->width, fb->height));
}

void tek_fb_unbind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}


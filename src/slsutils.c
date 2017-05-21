//
// Created by Steven on 4/25/15.
//

/**
 * @file slsutils.c
 * @brief
 *
 * Copyright (c) 2015, Steven Shea
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
*this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the dist.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
*FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
*those
 * of the authors and should not be interpreted as representing official
*policies,
 * either expressed or implied, of Steven Shea.
**/

#include "slsutils.h"
#include "sls-gl.h"

#ifdef WIN32
#include <windows.h>
#else

#include <unistd.h>

#endif

void*
sls_objalloc(void const* prototype, size_t size)
{
  void* obj = NULL;
  obj = calloc(size, 1);
  sls_checkmem(obj);
  memcpy(obj, prototype, size);
  return obj;

error:
  return NULL;
}

int
sls_chdir(char const* path)
{

  int res = 0;

#ifdef WIN32
  res = _chdir(path);
#else
  res = chdir(path);
#endif // WIN32

  return res;
}

void
sls_drain_glerrors()
{
  while (glGetError() != GL_NO_ERROR) {
  }
}

uint32_t
sls_debug_glerrors_impl_(char const* file, size_t line)
{
  GLenum e = GL_NO_ERROR;
  GLenum first = GL_NO_ERROR;
  while ((e = glGetError()) != GL_NO_ERROR) {
    if (first == GL_NO_ERROR) {
      first = e;
    }
    char const* err_str;
    switch (e) {
      case GL_INVALID_VALUE:
        err_str = "GL_INVALID_VALUE";
        break;

      case GL_INVALID_OPERATION:
        err_str = "GL_INVALID_OPERATION";
        break;

      case GL_INVALID_ENUM:
        err_str = "GL_INVALID_ENUM";
        break;
      default:
        err_str = (char const*)"error not found!";
    }
    fprintf(stderr,
            "file %s: line: %lu\n\tgl Error: 0x%x, %s\n\n",
            file,
            line,
            e,
            err_str);
  }

#ifdef SLS_DEBUG_VERBOSE
  if (first == GL_NO_ERROR) {
    fprintf(
      stderr, "file %s: line: %lu\n\tgl Error: none found\n\n", file, line);
  }
#endif
  return first;
}

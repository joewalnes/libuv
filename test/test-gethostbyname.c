/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "../uv.h"
#include "task.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* strlen */

ares_channel channel;
struct ares_options options;
int optmask;
void* uv_data;

int ares_callbacks;


static uv_buf_t alloc_cb(uv_handle_t* handle, size_t size) {
  uv_buf_t buf;
  buf.base = (char*)malloc(size);
  buf.len = size;
  return buf;
}

static void arescallback( void *arg,
                          int status,
                          int timeouts,
                          struct hostent *hostent) {
    int * iargs;
    ASSERT(hostent != NULL);
    ASSERT(arg != NULL);
    iargs = (int*)arg;
    ASSERT(*iargs == 7);

    ares_callbacks++;
}

TEST_IMPL(gethostbyname) {

  int sig = 7;
  int rc = 0;

  rc = ares_library_init(ARES_LIB_INIT_ALL);
  if (rc != 0) {
    printf("ares library init fails %d\n", rc);
    return 1;
  }

  uv_init(alloc_cb);

  optmask = 0;
  ares_callbacks = 0;
  printf("Uv init\n");

  rc = uv_ares_init_options(&uv_data, &channel, &options, optmask);
  printf("Uv ares init %d\n", rc);
  ASSERT(rc == ARES_SUCCESS);

  ares_gethostbyname(channel, 
                    "microsoft.com",
                    AF_INET,
                    &arescallback,
                    &sig);
  printf("gethostbyname returned\n");

  uv_run();
  printf("Uv run completes\n");

  uv_ares_destroy(uv_data, channel);

  ASSERT(ares_callbacks == 1);

  return 0;
}

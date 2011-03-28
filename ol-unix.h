#ifndef OL_UNIX_H
#define OL_UNIX_H

#include "ngx-queue.h"

#define EV_MULTIPLICITY 0
#include "ev/ev.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


/**
 * Note can be cast to io_vec.
 */
typedef struct {
  char* base;
  size_t len;
} ol_buf;


typedef struct {
  ol_buf* bufs;
  int bufcnt;
  int current_index;
  size_t written;
  ol_write_cb write_cb;
  ol_handle* handle;
  ngx_queue_t write_queue;
} ol_bucket;



typedef struct {
  int local;
  ol_req_cb connect_cb;
} ol_req_private;


typedef struct {
  int fd;

  ol_read_cb read_cb;
  ol_close_cb close_cb;

  ol_req *connect_req;

  ev_io read_watcher;
  ev_io write_watcher;

  ngx_queue_t write_queue;
  ngx_queue_t all_handles;


} ol_handle_private;


#endif /* OL_UNIX_H */
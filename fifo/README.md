### open() behaviour
`open()` with `O_WRONLY` or `O_RDONLY` set and `O_NONBLOCK` not set blocks until the other end is opened too.

### read() and write() behaviour if the other end gets closed
On the read end, all read() calls will return 0, indicating EOF.
On the write end, all write() calls will return -1 with errno set to EPIPE, SIGPIPE signal will be raised as well. If not ignored or handled explicitly by `signal()`, this _kills the process_.

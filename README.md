# Log4g - A GObject-based logging utility

This is [Log4g][1], a port of [Log4j][2] to the GObject framework.

The initial intent of this project was to learn the GObject framework (and
indirectly some Java) by porting Log4j classes directly to GObject classes.
Perhaps some users of GLib and GObject will find this library useful.

Please report bugs on [Github][3].

[1]: http://mike.steinert.ca/log4g/
[2]: http://logging.apache.org/log4j/
[3]: http://github.com/msteinert/log4g/issues/

## Quickstart

Setting up Log4g in your application requires three simple steps:

### Include the main Log4g header file

    #include <log4g/log4g.h>

### Initialize the Log4g system

    log4g_init(&argc, &argv);

The initialization function should be called before you use the logging API.
If you plan to use threads in your program you must initialize the GLib
threading API before you initialize Log4g. If the threading system has not
been initialized before calling log4g_init() then Log4g will not perform
any locking or attempt to use any threading features.

### Insert logging statements into your application

    log4g_error("program failure: %s", error->message);

Log4g supports the notion of logging levels. There are 5 default logging
levels in Log4g (custom logging levels are also supported). The default log
levels are:

* TRACE via `log4g_trace()`
* DEBUG via `log4g_debug()`
* INFO via `log4g_info()`
* WARN via `log4g_warn()`
* ERROR via `log4g_error()`
* FATAL via `log4g_fatal()`

If you would like to use a logger other than the root logger, you can
define the macro `LOG4G_LOG_DOMAIN` before including the Log4g header file:

    #define LOG4G_LOG_DOMAIN ("org.gnome.test")
    #include <log4g/log4g.h>

This sets the Log4g logging domain for the duration of that file. To use
multiple logger instances from within the same file you will need to use a
slightly different API:

    log4g_logger_error(log4g_get_logger("org.gnome.foo"),
                       "program failure: %s", error->message);

    log4g_logger_warn(log4g_get_logger("org.gnome.bar"),
                      "you had better watch out %s", name);

These two log statements would use the "org.gnome.foo" & "org.gnome.bar"
loggers respectively. Logger names use a class hierarchy in which classes
are separated by the dot (".") character.

### Cleanup the logger hierarchy

    log4g_finalize();

It is not strictly necessary to call finalize, however this will destroy the
logger hierarchy. This may become important if you are using an asynchronous
logger. Destroying the hierarchy will ensure that any pending asynchronous
log messages are flushed before your application exits.

If you have used the default configuration (as in this example), log messages
will be printed to stdout using the TTCC layout. For information on how to
configure Log4g refer to the detailed documentation.

## Installation

Log4g ships with an Autotools build environment. First run the provided
configure script (supplying any desired options) and then run make:

    $ tar zxf log4g-0.1.2.tar.gz
    $ cd log4g-0.1.2
    $ ./configure
    $ make
    $ sudo make install

> If you are building an unstable release (cloned with git) then you will
> have to generate the configure script by running autogen.sh. This requires
> that you have autoconf, automake and libtool installed. Refer to the
> Autotools documentation for more information.

## Documentation

Log4g is documented using GTK-Doc. To build the documentation pass the
`--enable-gtk-doc` flag to the configure script. The documentation will be
built when you type `make`.

Once the documentation is generated, point your web browser to:

    docs/reference/log4g/html/index.html

A reasonably up to date version of the documentation can be found on
[Github][4].

[4]: http:/mike.steinert.ca/log4g/documentation.html

## Dependencies

[GNU Make][gmake] is required to build. Log4g depends on [Glib][glib]. The
DOM parser module requires [Libxml][libxml].

[gmake]: http://www.gnu.org/software/make/
[glib]: http://www.gtk.org/
[libxml]: http://xmlsoft.org/

## License

Log4g is released under version 2.1 of the [LGPL][lgpl].

[lgpl]: http://www.gnu.org/licenses/lgpl-2.1.html

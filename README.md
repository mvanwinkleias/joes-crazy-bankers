# Joe's Crazy Bankers

Joe Nelson wrote an article about Concurrent Programming:

* https://begriffs.com/posts/2020-03-23-concurrent-programming.html

This was an attempt to learn about automake / autoconf, and packaging
a bit of code that links against another library.

Get started:

```
autoreconf --install
```

```
# To build:
./configure
make
```

```
# To build a Debian package:
make dist-gzip
# Copy the resulting file elsewhere, into it's own directory
debmake -a (name-of-tar.gz) -i debuild
```


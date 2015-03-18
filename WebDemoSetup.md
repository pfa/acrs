

# Introduction #

Included in the last release is a python CGI script and an example web page which can be used to demo the ACRS library through a web server. This page explains how to set up an Apache web server to serve the ACRS demo and where to place the acrs scripts.

This assumes Apache is already functional, but perhaps does not have CGI enabled.

# Prerequirements #

  * Python 2.6 must be installed (won't work with 2.5, haven't tested with anything else). Check the current version of Python using `python -V`.

# How to #

  * Verify that CGI support is enabled in the site file (for example, /etc/apache2/sites-enabled/000-default). Also note the DocumentRoot and cgi-bin directories:

```
    DocumentRoot /var/www/
    <Directory "/usr/lib/cgi-bin">
        Options +ExecCGI
        # Other options omitted
    </Directory>
```

  * Extract the acrs source code:

```
tar -xzf acrs-r56.tar.gz
```

  * Place the acrs.html page in the web server's document root directory (I'm no web designer, feel free to use your own page instead):

```
cp acrs/python/acrs.html /var/www/
```

  * Make sure the page is readable by 'other':

```
chmod o+r /var/www/acrs.html
```

  * Place the acrs-web-demo.py file in the cgi-bin directory. This is the CGI script.

```
cp acrs/python/acrs-web-demo.py /usr/lib/cgi-bin/
```

  * Change permissions on the cgi script so that it is readable and executable by 'other' and not writable by anyone:

```
chmod o+rx,a-w /usr/lib/cgi-bin/acrs-web-demo.py
```

  * The cgi script needs to be able to use the supporting acrs libraries. This means the libraries need to be copied into the python module search path. Ideally there would be an install script that does this for you, but there is currently no such script. So, you have two options:
    * Easy way: place libraries in the same directory as acrs-web-demo.py. The 'current directory' is always in the python search path. For simplicity, I'm assuming you're going to do this.
    * Other way: place libraries in a proper search path, such as /usr/lib/python2.6 on most systems. If you want to do it this way, you can check the python search path directories using the following command. You can use this directory instead of /usr/lib/cgi-bin/ through the rest of the how to:

```
python -c "import sys ; print sys.path"
```

  * I'm going to assume everything is being placed in the cgi-bin directory. So, do the following:

```
cd acrs/python
cp acrs.py ip4route.py ip4addr.py /usr/lib/cgi-bin/
```

  * Lastly, it would be nice to compile the libraries into bytecode now. The python interpreter won't have permission to write to the cgi-bin directory when it's being ran by the web server, so it would normally have to re-generate bytecode for the libraries every time it is ran. That takes (small amounts of) time. Run the script once as root to produce bytecode (or copy in the `*`.pyc files that are generated when you run the script as an unprivileged user). All you need to do is execute the acrs-web-demo.py script, no arguments are necessary:

```
./acrs-web-demo.py
```

If you ran the script as a user having permissions to write to the cgi-bin directory, you should now see three .pyc files in the directory: acrs.pyc, ip4addr.pyc, and ip4route.pyc. This will speed subsequent runs up.

At this point, you should be able to load the page by connecting to http://localhost/acrs.html and use the demo program.

# Other notes #

  1. By default, the script will print diagnostic messages to the web page if an exception is thrown. Along with a stack trace, this will print the version of python you have installed and a timestamp. This information aids in debugging. If you don't want the python version or time advertised from your web server, open the acrs-web-demo.py script and comment out the line "cgitb.enable()" (around line 33). You can also set this to write to a log file instead, check out the python documentation for cgitb.
  1. The number of routes allowed to be summarized is limited in the web version to prevent misuse. It's currently set to 50. This can be increased by setting the ARGS\_LIMIT variable near the top of acrs-web-demo.py.
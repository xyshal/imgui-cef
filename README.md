Summary
=======
Prototype for investigating whether it's possible (and how well it works) to
embed a VNC client in an imgui application using the CEF.  This is not
production code and should not be used.

The license of this repository applies only to the code and scripts in this
repository; its output -- since it pulls in a variety of thirdparty libraries
-- is of mixed state and this repository only demonstrates the feasability of
such an application and says nothing about the legality of its distribution.

Prerequisites
=============
* cmake
* OpenGL development libraries
* On Linux, the X development libraries

Build
=====
Run thirdparty/download-cef.sh to grab Spotify's already-built CEF.

Optionally... run thirdparty/build-cef.sh.  This took me about 5 hours on an M1
Mac and required 176 GB of disk space.

Once it's built or acquired you can do a standard `mkdir build && cmake .. && make`

Run
===
On Mac OS in order to get the Framework to run I needed to:

xattr -dr com.apple.quarantine Chromium\ Embedded\ Framework.framework
codesign --force --deep --sign - Chromium\ Embedded\ Framework.framework

Current State of Affairs
========================
I proceeded down this track:
# Write the imgui prototype
# Integrate CEF
# Integrate noVNC

This ultimately failed on step 2.  I tried various combinations of platforms
and methods, and generally wound up in one of two states:

Invalid file descriptor to ICU data received
--------------------------------------------
This happened across every Linux machine I had, including VMs; Debian, Arch;
arm64, x86\_64... an old Intel Macbook 2012 running Ubuntu...

It happens in a validity check following the existence check of the
(presumably) ICU database file in Resources/ (in InitializeICU()) and if it was
worth the time I'd probably go down the path of debugging it further to see
what about the validity check failed; but the Debug release of CEF (or I) had
some considerable issues reading local variables; gdb knew about a colossal
amount of global variables but nothing from the current function.

You'd think that with such a wide variety of issues there was one of two
problems:
* The prototype code is wrong.  To this end I replaced the code in src/
  entirely with code yanked from the CEF repository's cefsimple application.
  This built successfully... and had the exact same problem.
* The current build of the precompiled library is bad.  To this end I
  downloaded the beta branch and encountered the same result.
* The precompiled library just coincidentally doesn't work on the combinations
  of platforms and package versions I tried.  To this end I would try to
  compile it up on Linux (I only ever tried to compile it on Mac OS) but I
  think it's not worth the time.

Crashing
--------
On Mac OS, I got an immediate nullptr crash with an extremely vague stack.  It
probably just has to do with the fact that I don't understand how Frameworks
work.

Conclusions
===========
I don't have a Windows machine at the moment, unfortunately, so wasn't able to
test there.

Doubtless these obstacles could be overcome, as there are plenty of examples of
CEF being successfully integrated into an application, but there isn't enough
time to overcome them with this project.

I do expect it's viable, but was unable to prove it myself.

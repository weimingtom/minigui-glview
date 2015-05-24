MiniGLView includes three separated parts of work:

1. External control of MiniGUI (http://www.minigui.org), named GLView. MiniGLView embedded a third party software renderer, TinyGL(http://bellard.org/TinyGL), which is a subset of OpenGL(http://www.opengl.org). MiniGLView provides some basic but fast and lightweight rendering algorithms (powered by TinyGL) as well as keyboard and mouse interactive controlls (powered by MiniGUI).

2. Modified copy of TinyGL. New functions included:
> a. Offseted block copying for direct-screen-access(from TinyGL internal buffer to screen framebuffer);
> b. Pixel line copying which supports non-4-bytes alignment

3. Three rendering examples from official TinyGL examples with minimum changes.
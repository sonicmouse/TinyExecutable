# TinyExecutable
This is a solution created in Visual Studio 2019 that produces an executable at 1,824 bytes. This executable will show an overlapped window.

Why? Because of this [YouTube video](https://www.youtube.com/watch?v=b0zxIfJJLAY).

Dave claimed he created the smallest executable ever. He got his down to about 3.5 KB using UPX.

I decided to take him on. I more-or-less ported his asm code to C and got it to compile to 1,824 bytes using some linker tricks. Oh, and I didn't use any packers such as UPX.

So, now I hold the title for the smallest executable ever created.
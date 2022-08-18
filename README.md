# HelloOpenGL
A clonable project to quickly dive into the world of OpenGL (GLFW, GLAD).

## How to recreate
Step 1: Copy paste that CMake file  
Step 2: Learn how it interacts with this directory  
Step 3: Change the directory structure if you dislike this one  

libs
- glad
- - src
- - - gl.c (glad c file)
- include
- - GLFW
- - - glfw3.h
- - - glfw3native.h
- - KHR
- - - khrplatform.h
- - glad
- - - gl.h (glad h file)

Alternatively don't split the glad c and h file and just have them in the one folder  
As of writing (18/08/22) I think this makes more sense (glad library in a glad folder, glad include file in the include folder)  
glfw binaries are some of the most misleading things to ever exist in this world  

## References
glfw repository https://github.com/glfw/glfw  
glad repository https://github.com/Dav1dde/glad  
Install Glad 2 from here https://gen.glad.sh/

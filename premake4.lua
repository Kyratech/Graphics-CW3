--
-- see https://premake.github.io/ for explanations of these commands.
--
solution ('ml2g14_GraphicsCW3') --Top Level name of the project
   configurations { 'Release' }
      language 'C++' -- The programming language
      project("GraphicsCW3") -- The specific name
        kind 'ConsoleApp' -- the target application
        targetdir('.') -- where to put the executable
        links{'glew32', 'glfw3', 'opengl32', 'libBulletDynamics', 'libBulletCollision', 'libLinearMath'} -- the libraries to link
        files {"*.cpp"} -- collects all the cpp files in the directory, there must only be one main function.
        buildoptions{'-Wno-write-strings'} -- build option to suppress a very common warning  about strings
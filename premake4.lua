--
-- see https://premake.github.io/ for explanations of these commands.
--
solution ('BulletWrapperTesterCB') --Top Level name of the project
   configurations { 'Release' }
      language 'C++' -- The programming language
      project("WrapperTesterCB") -- The specific name
        kind 'ConsoleApp' -- the target application
        targetdir('./bin/') -- where to put the executable
        links{'glew32', 'glfw3', 'opengl32', 'libPhysicsWrapper', 'libBulletDynamics', 'libBulletCollision', 'libLinearMath', 'gdi32'} -- the libraries to link
        files {"src/*.cpp"} -- collects all the cpp files in the directory, there must only be one main function.
        buildoptions{'-Wno-write-strings'} -- build option to suppress a very common warning  about strings
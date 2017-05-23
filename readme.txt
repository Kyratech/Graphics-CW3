Program operation:

- The program starts in free camera mode:
	> [UP] Increases your forward speed
	> [DOWN] Decreased your forward speed
	> [SPACE] Brings the camera to a instant stop
	> [LEFT][RIGHT] Angle the camera left/right
	> [PG_UP][PG_DOWN] Angle the camera up/down
	> [LMB + Move mouse] Aim the camera (like a FPS game)
	> [Z] Resets the position of the camera if you manage to fly off into space.
- [T] Enters tour mode:
	> The tour is synced to the animation, so you will enter the tour at the relevant point.
	> To view the tour from the beginning, press [R] to reset the animation once in the tour.
	> [E] exits tour mode to free camera mode.
- [L] Enters top-down mode.
- [P] Enters screenshot mode.
- [M] Enter free-camera mode, if not already in it.

There is no game component.

Files:

.
+-- Animation.cpp			-Creates keyframes for animation + tour
+-- config.lua
+-- Images
¦   +-- DawnTank
¦   ¦   +-- DuelTankBody_Dawn_DIFFUSE.png
¦   ¦   +-- DuelTankBody_Dawn_SPECULAR.png
¦   ¦   +-- DuelTankGun_Dawn_DIFFUSE.png
¦   ¦   +-- DuelTankGun_Dawn_SPECULAR.png
¦   ¦   +-- DuelTankTurret_Dawn_DIFFUSE.png
¦   ¦   +-- DuelTankTurret_Dawn_SPECULAR.png
¦   +-- DawnTransport
¦   ¦   +-- Transport_Dawn_DIFFUSE.png
¦   ¦   +-- Transport_Dawn_SPECULAR.png
¦   +-- DuskTank
¦   ¦   +-- DuelTankBody_Dusk_DIFFUSE.png
¦   ¦   +-- DuelTankBody_Dusk_SPECULAR.png
¦   ¦   +-- DuelTankGun_Dusk_DIFFUSE.png
¦   ¦   +-- DuelTankGun_Dusk_SPECULAR.png
¦   ¦   +-- DuelTankTurret_Dusk_DIFFUSE.png
¦   ¦   +-- DuelTankTurret_Dusk_SPECULAR.png
¦   +-- DuskTransport
¦   ¦   +-- Transport_DIFFUSE.png
¦   ¦   +-- Transport_SPECULAR.png
¦   +-- Rock
¦   ¦   +-- FloatingIsland1DIFFUSE.png
¦   ¦   +-- MainIsland_DIFFUSE.png
¦   ¦   +-- River_DIFFUSE.png
¦   ¦   +-- River_SPECULAR.png
¦   ¦   +-- Rock1_DIFFUSE.png
¦   ¦   +-- Rock1_SPECULAR.png
¦   +-- Skybox
¦   ¦   +-- Skybox_BACK.png
¦   ¦   +-- Skybox_BOTTOM.png
¦   ¦   +-- Skybox_FRONT.png
¦   ¦   +-- Skybox_LEFT.png
¦   ¦   +-- Skybox_RIGHT.png
¦   ¦   +-- Skybox_TOP.png
¦   +-- TransportArm_DIFFUSE.png
¦   +-- TransportArm_SPECULAR.png
+-- Include
¦   +-- Animation.h			-Interface for animation method
¦   +-- BLCamera.h			-Control for the tour camera
¦	+-- CubeGeometry.h 		-Used to get vertices for the skybox
¦   +-- CWObject.h			-Superclass for drawable objects
¦   +-- CWPhysicsLamp.h		-Object that moves a point light + draws a mesh at origin of a rigidbody
¦   +-- CWPhysicsObject.h	-Object that draws a mesh at the transform of a rigidbidy
¦   +-- fps_camera.h		-Control of the free camera
¦   +-- GraphicsObject.h	-Non-physics drawable object
¦   +-- Introduction.h		-Top level header for program, has global structs etc.
¦   +-- Light.h				-Defines interface for creating directional, point and spot lights (last one not used)
¦   +-- Mesh.h				-Superclass for meshes
¦   +-- OBJMesh.h			-Meshes created from imported .obj files
¦   +-- PhysicsWrapperMini			-Interface for simplified physics wrapper
¦   ¦   +-- PhysicsCollision.h
¦   ¦   +-- PhysicsObject.h
¦   ¦   +-- PhysicsObjectTypes.h
¦   ¦   +-- PhysicsUtilities.h
¦   ¦   +-- PhysicsWorld.h
¦   +-- ScrollingObject.h	-Drawable object with scrolling texture
¦   +-- ShaderLoader.h		-Loads and compiles shader source files
¦   +-- Skybox.h			-Draws a skybox
¦   +-- stb_image
¦   ¦   +-- README.md
¦   ¦   +-- stb_image.h
¦   +-- tank				-Drawable tank with angleable turret and cannon
¦   ¦   +-- CannonObject.h
¦   ¦   +-- TankObject.h
¦   ¦   +-- TurretObject.h
¦   +-- TinyOBJLoader
¦   ¦   +-- LICENSE
¦   ¦   +-- README.md
¦   ¦   +-- tiny_obj_loader.h
¦   +-- transport			-Drawable transport with angleable lift arms
¦   ¦   +-- TransportObject.h
¦   +-- TriangleMesh.h		-Mesh created from triangles of vertices
¦   +-- UVSphereGeometry.h	-Returns vertices for use with above
+-- Light.cpp				-Implementation for creating lights and calculating shadows
+-- main.cpp				-Program entry
+-- Models
¦   +-- DuelTank_Body.obj
¦   +-- DuelTank_Body_smooth.obj
¦   +-- DuelTank_Cannon_smooth.obj
¦   +-- DuelTank_Cannon_smooth_moved.obj
¦   +-- DuelTank_Turret_smooth.obj
¦   +-- DuelTank_Turret_smooth_moved.obj
¦   +-- FloatingIsland1.obj
¦   +-- MainIsland.obj
¦   +-- MainIslandCollision.obj
¦   +-- MainIslandWaterfall.obj
¦   +-- MediumIsland.obj
¦   +-- MediumWaterfall.obj
¦   +-- Rock1.obj
¦   +-- SmallIsland.obj
¦   +-- SmallWaterfall.obj
¦   +-- TransportArmOffset.obj
¦   +-- TransportOffset.obj
+-- PhysicsCollision.cpp	-Implmentation of physics wrapper
+-- PhysicsObject.cpp
+-- PhysicsObjectTypes.cpp
+-- PhysicsWorld.cpp
+-- premake4.lua
+-- readme.txt				-You are here
+-- Shaders
¦   +-- CelShader.frag		-Shader for cel-shaded effect
¦   +-- CelShader.vert
¦   +-- DepthShader.frag	-Shader for calcualting directional shadows
¦   +-- DepthShader.vert
¦   +-- SkyboxShader.frag	-Shader for skyboxes
¦   +-- SkyboxShader.vert
¦   +-- UnlitShader.frag	-Shader for objects that are not affected by light levels
¦   +-- UnlitShader.vert
¦   +-- WaterfallShader.frag	-Cel-shaded effect + scrolling texture support
¦   +-- WaterfallShader.vert
+-- stb_image.cpp
+-- tiny_obj_loader.cpp

How to build:

I have been building this project using codeblocks, and the Msys GNU GCC compiler.
To build the program, you must link, in order:
'glew32'
'glfw3'
'opengl32'
'libBulletDynamics'
'libBulletCollision'
'libLinearMath'
And you must have these libraries in your include path:
-Glew
-Glfw3
-glm
-bullet
The other libraries used by the project, stb_image and tiny_obj_loader, are contained in the project include directory.

How the program works:

-The program uses a heirarchical structure to contain meshes inside of drawable objects.
-These objects can be kinematic, or controlled by a bullet physics rigidbody (by way of the mini wrapper).
-The static islands and tank projectiles use physics objects to control them.
-The tanks and transports are animated using a keyframe system:
	>Keyframes define positions, rotations etc.
	>As the program runs, each of these objects interpolates its transform between the two surrounding keyframes.
	>The motion tween is only linear, so can look a bit janky.
-The tour camera also uses a keyframe system.
-Cel shading is achieved by clamping the output of the fragment shader to discrete increments.
-Scrolling textures simply apply an offset to the UV coordinates to simulated a moving image.
-Shadows are only enabled on the directional 'sun' light. It uses a single 2048x shadow map to get the effect. (No CSM).
-The free camera is attached to a bullet sphere rigidbody, so it collides with the static environment (this does make moving a bit floaty).


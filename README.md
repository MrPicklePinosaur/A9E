
# A9E - pinosaur's ascii game engine

**A9E** is a simple ascii game engine that comes bundled with an entity
component system and a simple physics engine that is capable of rigid body
collision detection and response.

three demo programs (**invaders**, **flappy birb** and a **physics demo**) are
also included in this repository to show off how the **A9E** engine can be used
to create games.

## INSTALLING A9E LIBRARY

install the shared library and headers by running:
```
$ make liba9e.so && sudo make install
```

uninstall with
```
$ sudo make uninstall
```

## BUILDING FROM SOURCE

the ncurses library is required. install it depending on your system. then
to build the engine along with the demos, simply run:
```
$ make
```

## DOCUMENTATION

a wiki for **A9E** may or may not be coming in the future.

## TODO
- [x] entity component system
- [x] color
- [ ] terminal beep
- [ ] utf8 support
- [x] better way of assigning entity ids
- [x] user input
- [x] simple physics
- [x] collision system
- [x] collision mask
- [x] error/exception handling
- [x] ability to create and destroy entities in update loop
- [x] renderer + input should really be decoupled from scene
- [x] build shared library
- [ ] package for linux distros

## RESOURCES

some resources that were used when writing this project
- [ECS design](https://austinmorlan.com/posts/entity_component_system/)
- [another ECS design](https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html)
- [physics engine in 5 min](https://www.youtube.com/watch?v=-_IspRG548E)
- [impulse resolution and collision detection](https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331)
- [paper on impulse resolution](https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/5collisionresponse/Physics%20-%20Collision%20Response.pdf)


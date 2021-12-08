
# A9E - pinosaur's ascii game engine

## BUILDING

the ncurses library is required. install it depending on your system. then simply run
```
$ make
```

## TODO
- [x] entity component system
- [ ] color
- [x] better way of assigning entity ids
- [x] user input
- [x] simple physics
- [x] collision system
- [ ] collision mask
- [ ] error/exception handling
- [ ] logger that can either do to file or the ncurses stauts 'console'
- [ ] ability to create and destroy entities in update loop (might need a create/destroy queue)
- [ ] package for linux distros

## RESOURCES

some resources that were used when writing this project
- [ECS design](https://austinmorlan.com/posts/entity_component_system/)
- [another ECS design](https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html)
- [physics engine in 5 min](https://www.youtube.com/watch?v=-_IspRG548E)
- [impulse resolution and collision detection](https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331)
- [paper on impulse resolution](https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/5collisionresponse/Physics%20-%20Collision%20Response.pdf)

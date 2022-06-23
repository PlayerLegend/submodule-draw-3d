
depend: draw-3d-depend
draw-3d-depend:
	cdeps src/draw-3d > src/draw-3d/depends.makefile

run-tests: run-draw-3d-tests
run-draw-3d-tests:

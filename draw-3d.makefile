test/draw-3d: CFLAGS += `pkg-config --cflags glfw3`
test/draw-3d: LDLIBS += `pkg-config --libs glfw3` -ldl -lm
test/draw-3d: \
	src/draw-3d/test/draw-3d.test.o \
	src/game-interface/init.o \
	src/game-interface/window.o \
	src/game-interface/input.o \
	src/game-interface/time.o \
	src/log/log.o \
	src/glad/glad.o \
	src/draw-3d/buffer/loader.o \
	src/convert/source.o \
	src/convert/fd/source.o \
	src/gltf/convert.o \
	src/window/alloc.o \
	src/gltf/env.o \
	src/gltf/gltf.o \
	src/table/string.o \
	src/json/json.o \
	src/range/streq.o \
	src/range/strdup.o \
	src/range/string_init.o \
	src/range/strdup_to_string.o \
	src/draw-3d/buffer/def.o \
	src/draw-3d/mesh/def.o \
	src/draw-3d/shader/load.o \
	src/draw-3d/buffer/draw.o \
	src/vec/mat4.o \
	src/vec/vec3.o \
	src/vec/vec4.o \

test/run-draw-3d: src/draw-3d/test/run.sh

C_PROGRAMS += test/draw-3d
SH_PROGRAMS += test/run-draw-3d

draw-3d-tests: test/draw-3d test/run-draw-3d

run-draw-3d-test:
	make depend
	make BUILD_ENV=debug draw-3d-tests
	test/run-draw-3d

depend: draw-3d-depend
draw-3d-depend:
	cdeps src/draw-3d > src/draw-3d/depends.makefile

run-tests: run-draw-3d-tests
run-draw-3d-tests:

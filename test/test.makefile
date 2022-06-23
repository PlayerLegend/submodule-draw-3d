test/draw-3d: CFLAGS += `pkg-config --cflags glfw3`
test/draw-3d: LDLIBS += `pkg-config --libs glfw3` -ldl -lm
test/draw-3d: \
	src/convert/fd/source.o \
	src/convert/source.o \
	src/draw-3d/buffer/display.o \
	src/draw-3d/buffer/free.o \
	src/draw-3d/buffer/index.o \
	src/draw-3d/buffer/load/glb.o \
	src/draw-3d/buffer/load/path.o \
	src/draw-3d/object/set.o \
	src/draw-3d/shader/free.o \
	src/draw-3d/shader/link.o \
	src/draw-3d/shader/load/text.o \
	src/draw-3d/shader/load/source.o \
	src/draw-3d/shader/load/path.o \
	src/draw-3d/test/draw-3d.test.o \
	src/game-interface/init.o \
	src/game-interface/input.o \
	src/game-interface/time.o \
	src/game-interface/window.o \
	src/glad/glad.o \
	src/gltf/copy.o \
	src/gltf/env.o \
	src/gltf/free.o \
	src/gltf/load/memory.o \
	src/gltf/load/source.o \
	src/json/json.o \
	src/log/log.o \
	src/range/alloc.o \
	src/range/strdup.o \
	src/range/strdup_to_string.o \
	src/range/streq.o \
	src/range/string_init.o \
	src/table/string.o \
	src/vec/mat4.o \
	src/vec/vec3.o \
	src/vec/vec4.o \
	src/window/alloc.o \

test/run-draw-3d: src/draw-3d/test/run.sh

C_PROGRAMS += test/draw-3d
SH_PROGRAMS += test/run-draw-3d

draw-3d-tests: test/draw-3d test/run-draw-3d

run-draw-3d-test:
	make depend
	make BUILD_ENV=debug draw-3d-tests
	test/run-draw-3d

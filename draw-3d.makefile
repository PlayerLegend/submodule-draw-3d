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
	sh makedepend.sh src/draw-3d/draw-3d.makefile

run-tests: run-draw-3d-tests
run-draw-3d-tests:

# DO NOT DELETE

src/draw-3d/buffer/def.o: src/draw-3d/buffer/def.h src/draw-3d/mesh/def.h
src/draw-3d/buffer/def.o: src/gltf/def.h src/window/def.h src/range/def.h
src/draw-3d/buffer/def.o: src/vec/vec.h src/vec/vec3.h src/vec/vec4.h
src/draw-3d/buffer/def.o: src/glad/glad.h src/draw-3d/buffer/internal/def.h
src/draw-3d/buffer/loader.o: src/draw-3d/buffer/loader.h
src/draw-3d/buffer/loader.o: src/draw-3d/buffer/def.h src/draw-3d/mesh/def.h
src/draw-3d/buffer/loader.o: src/gltf/def.h src/window/def.h src/range/def.h
src/draw-3d/buffer/loader.o: src/vec/vec.h src/vec/vec3.h src/vec/vec4.h
src/draw-3d/buffer/loader.o: src/glad/glad.h src/gltf/parse.h
src/draw-3d/buffer/loader.o: src/window/alloc.h src/gltf/env.h
src/draw-3d/buffer/loader.o: src/convert/status.h src/convert/source.h
src/draw-3d/buffer/loader.o: src/gltf/convert.h src/log/log.h
src/draw-3d/buffer/loader.o: src/draw-3d/buffer/internal/def.h
src/draw-3d/buffer/draw.o: src/draw-3d/buffer/draw.h src/draw-3d/buffer/def.h
src/draw-3d/buffer/draw.o: src/draw-3d/mesh/def.h src/gltf/def.h
src/draw-3d/buffer/draw.o: src/window/def.h src/range/def.h src/vec/vec.h
src/draw-3d/buffer/draw.o: src/vec/vec3.h src/vec/vec4.h src/glad/glad.h
src/draw-3d/buffer/draw.o: src/draw-3d/shader/def.h
src/draw-3d/buffer/draw.o: src/draw-3d/buffer/internal/def.h
src/draw-3d/buffer/draw.o: src/keyargs/keyargs.h src/vec/mat4.h
src/draw-3d/buffer/draw.o: src/game-interface/time.h src/log/log.h
src/draw-3d/buffer/draw.o: src/draw-3d/debug.h
src/draw-3d/shader/load.o: src/draw-3d/shader/load.h src/convert/source.h
src/draw-3d/shader/load.o: src/keyargs/keyargs.h src/draw-3d/buffer/def.h
src/draw-3d/shader/load.o: src/draw-3d/mesh/def.h src/gltf/def.h
src/draw-3d/shader/load.o: src/window/def.h src/range/def.h src/vec/vec.h
src/draw-3d/shader/load.o: src/vec/vec3.h src/vec/vec4.h src/glad/glad.h
src/draw-3d/shader/load.o: src/convert/fd/source.h src/log/log.h
src/draw-3d/shader/load.o: src/window/alloc.h src/draw-3d/debug.h
src/draw-3d/test/draw-3d.test.o: src/draw-3d/buffer/loader.h
src/draw-3d/test/draw-3d.test.o: src/draw-3d/buffer/def.h
src/draw-3d/test/draw-3d.test.o: src/draw-3d/mesh/def.h src/gltf/def.h
src/draw-3d/test/draw-3d.test.o: src/window/def.h src/range/def.h
src/draw-3d/test/draw-3d.test.o: src/vec/vec.h src/vec/vec3.h src/vec/vec4.h
src/draw-3d/test/draw-3d.test.o: src/glad/glad.h src/keyargs/keyargs.h
src/draw-3d/test/draw-3d.test.o: src/game-interface/init.h
src/draw-3d/test/draw-3d.test.o: src/game-interface/window.h src/log/log.h
src/draw-3d/test/draw-3d.test.o: src/convert/status.h src/convert/source.h
src/draw-3d/test/draw-3d.test.o: src/convert/fd/source.h src/gltf/convert.h
src/draw-3d/test/draw-3d.test.o: src/window/alloc.h src/gltf/parse.h
src/draw-3d/test/draw-3d.test.o: src/draw-3d/shader/def.h
src/draw-3d/test/draw-3d.test.o: src/draw-3d/buffer/draw.h
src/draw-3d/test/draw-3d.test.o: src/draw-3d/shader/load.h
src/draw-3d/test/draw-3d.test.o: src/game-interface/time.h
src/draw-3d/mesh/def.o: src/draw-3d/buffer/def.h src/draw-3d/mesh/def.h
src/draw-3d/mesh/def.o: src/gltf/def.h src/window/def.h src/range/def.h
src/draw-3d/mesh/def.o: src/vec/vec.h src/vec/vec3.h src/vec/vec4.h
src/draw-3d/mesh/def.o: src/glad/glad.h src/window/alloc.h

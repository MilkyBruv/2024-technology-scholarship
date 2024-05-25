LIBS_UNIX:=include/imgui/*.cpp include/imgui/*.h include/rlImGui/*.cpp include/rlImGui/*.h include/rlImGui/extras/*.h include/ImGuiColorTextEdit/*.h include/ImGuiColorTextEdit/*.cpp -Iinclude/raylib/linux -Linclude/raylib/linux -lraylib
SRC:=src/*.cpp
OUTPUT:=outcome.exe

unix:
	g++ ${SRC} ${LIBS_UNIX} -o ${OUTPUT}
	./outcome.exe
	
unix-nr:
	g++ ${SRC} ${LIBS_UNIX} -o ${OUTPUT}

unix-path:
	export LD_LIBRARY_PATH=$(pwd)/include/raylib/linux:$LD_LIBRARY_PATH

builddir := 'build'

fmt:
    find src -type f \( -name "*.cpp" -o -name "*.hpp" \) -print0 | xargs -0 clang-format -i

lint:
	find src -type f -name "*.cpp" -print0 | parallel -q0 --eta clang-tidy --load={{ env_var("TIDYFOX") }}


configure target='debug' *FLAGS='':
    echo 'config: meson setup {{builddir}} --reconfigure --{{ target }} {{FLAGS}};'
    meson setup {{builddir}} --reconfigure --{{ target }} {{FLAGS}};
    
    ln -sf {{builddir}}/compile_commands.json compile_commands.json
    
_configure_if_clean:
	@if ! [ -d {{builddir}} ]; then just configure; fi

build: _configure_if_clean
	meson compile -C {{builddir}}

release: (configure "release") build

clean:
	rm -f compile_commands.json
	rm -rf {{builddir}}

run *ARGS='': build
	{{builddir}}/qt-mpv-bg-wlr {{ARGS}}


#!/bin/bash

function install {
	pushd artifacts

	git clone --depth=1 $1
	pushd $(basename $1)

	if [ -e "install-dependencies.bash" ]; then
		bash install-dependencies.bash
	fi

	mkdir -p build
	pushd build
	cmake -DCMAKE_INSTALL_PREFIX:PATH=../../../install ..
	make -j4
	make install
	popd

	popd

	popd
}

function dependencies_begin {
	# create dependecies directory
	mkdir -p dependencies
	pushd dependencies

	mkdir -p artifacts # This holds the git repos
	mkdir -p install # This holds the installed libraries
}

function dependencies_end {
	popd
}

dependencies_begin

install https://github.com/Pfeifenjoy/logging.h
install https://github.com/Pfeifenjoy/trie.h

dependencies_end


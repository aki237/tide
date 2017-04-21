LIBS="-L/home/aki237/Desktop/fltk-1.3.4-1/lib/" \
	"-ldl" \
	"-pthread" \
	"-lcairo" \
	"-lfontconfig" \
	"-lfreetype" \
	"-lXrender" \
	"-lX11" \
	"-lXext" \
	"-lXft" \
	"-lXcursor" \
	"-lXfixes" \
	"-lXinerama"

STATICLIBS="./install/lua/lib/liblua.a" \
		"./install/fltk/lib/libfltk.a" \
		"./install/fltk/lib/libfltk_cairo.a"

CLIPBOARD_LIBS="./external/libclipboard/lib/libclipboard.a"

CLIPBOARD_FLAGS="-lxcb" \
		"-pthread"
INCLUDE=`pkg-config --cflags cairo` \
	"-I./src/"\
	"-I./external/sol2" \
	"-I./install/lua/include/" \
	"-I./install/fltk/include/"
CLIPBOARD_INCLUDE="-I./external/libclipboard/include"
CC="/usr/bin/g++"
RED=`tput setaf 9`
GREEN=`tput setaf 10`
YELLOW=`tput setaf 11`
BLUE=`tput setaf 12`
RESET=`tput sgr0`
ROOTDIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SRCPATH = $(join ${ROOTDIR},src)
BUILDDIR      =  $(join ${ROOTDIR},build/)
DOCDIR      =  $(join ${ROOTDIR},doc/)
TARGET      =  $(join ${BUILDDIR},tide)
.PHONY   : clean install



all : prebuild ${TARGET} doc

doc : predoc \
	doc/Color.md \
	doc/Const.md \
	doc/Error.md \
	doc/Buffer.md \
	doc/Node.md \
	doc/Option.md \
	doc/Clipboard.md \
	doc/TextView.md

${TARGET} : ${BUILDDIR}/Buffer.o ${BUILDDIR}/Color.o ${BUILDDIR}/Clipboard.o ${BUILDDIR}/TextView.o ${BUILDDIR}/main.o
	@echo "${RED}Building (g++) ${BLUE}$(notdir $^)${RESET} to ${GREEN}$(notdir $@)${RESET}"
	@${CC} -std=c++14 -g -Wall -O3 $^ ${CLIPBOARD_LIBS} ${CLIPBOARD_FLAGS} -o $@ ${STATICLIBS} ${LIBS}

${BUILDDIR}/Clipboard.o : ${SRCPATH}/Clipboard.cpp
	@echo "${RED}Compiling (g++) ${BLUE}$(notdir $^)${RESET} to ${GREEN}$(notdir $@)${RESET}"
	@${CC} -std=c++14 -g -Wall -O3 -pthread ${INCLUDE} ${CLIPBOARD_INCLUDE} -c $< -o $@  ${CLIPBOARD_FLAGS}

${BUILDDIR}/%.o : ${SRCPATH}/%.cpp
	@echo "${RED}Compiling (g++) ${BLUE}$(notdir $^)${RESET} to ${GREEN}$(notdir $@)${RESET}"
	@${CC} -std=c++14 -g -Wall -O3 ${INCLUDE} ${CLIPBOARD_INCLUDE} -c $< -o $@  -std=c++14

run : all
	@${TARGET}

prebuild : deps
	@mkdir -p ${BUILDDIR}

doc/%.md : src/%.hpp src/%.cpp
	@echo "${RED}Building documentation ${BLUE}$(notdir $(abspath $^))${RESET}"
	@${ROOTDIR}scripts/doc.py $^ > $@

doc/%.md : src/%.hpp
	@echo "${RED}Building documentation ${BLUE}$(notdir $^)${RESET}"
	@scripts/doc.py $^ > $@

predoc :
	@mkdir -p ${DOCDIR}

clean: libclipboard_clean lua_clean fltk_clean tide_clean clean_bin
lua_clean:
	@make -C ${ROOTDIR}/external/lua clean
fltk_clean:
	@make -C ${ROOTDIR}/external/fltk clean
libclipboard_clean:
	@make -C ${ROOTDIR}/external/libclipboard clean
tide_clean:
	@rm -rf ${BUILDDIR}
	@rm -rf ${ROOTDIR}/doc
clean_bin:
	@rm -rf ${ROOTDIR}/install

deps: libclipboard lua fltk

lua:
	@echo "${RED}Building${RESET} ${BLUE}Lua${RESET} from ${YELLOW}external${RESET}"
	@make -C ${ROOTDIR}/external/lua linux
	@make -C ${ROOTDIR}/external/lua INSTALL_TOP=${ROOTDIR}/install/lua/ install

fltk:
	@echo "${RED}Building${RESET} ${BLUE}fltk${RESET} from ${YELLOW}external${RESET}"
	@cd ${ROOTDIR}/external/fltk && ./configure --prefix=${ROOTDIR}/install/fltk/ --enable-cairo
	@make -C ${ROOTDIR}/external/fltk
	@make -C ${ROOTDIR}/external/fltk install

libclipboard:
	@echo "${RED}Building${RESET} ${BLUE}libclipboard${RESET} from ${YELLOW}external${RESET}"
	@cd ./external/libclipboard/ && cmake . && make

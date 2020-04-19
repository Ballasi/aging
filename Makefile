INSTALL_PREFIX := /usr

BIN_NAME := aging
BIN_FOLDER := build
SRC_FOLDER := .

MAKE := make

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
QT5_PREFIX := $(shell brew --prefix qt)/lib/cmake/Qt5
GMP_PREFIX := $(shell brew --prefix gmp)
endif

$(MAKE): $(BIN_FOLDER)
	@echo -e "=> \e[1mCompiling and linking $(BIN_NAME)\e[0m"
ifeq ($(UNAME_S),Darwin)
		cd $(BIN_FOLDER)
		$(MAKE) $(BIN_FOLDER) --no-print-directory
else
		@env --chdir=$(BIN_FOLDER) $(MAKE) --no-print-directory
endif

$(BIN_FOLDER):
	@echo -e "=> \e[1mBuilding CMake configuration...\e[0m"
ifeq ($(UNAME_S), Darwin)
	@cmake -S $(SRC_FOLDER) -B $(BIN_FOLDER) -DQt5_DIR=$(QT5_PREFIX) -DGMP_PREFIX=$(GMP_PREFIX)
else
	@cmake -S $(SRC_FOLDER) -B $(BIN_FOLDER)
endif
run: $(MAKE)
	@echo -e "=> \e[1mRunning $(BIN_NAME)\e[0m"
	@env --chdir=$(BIN_FOLDER) ./$(BIN_NAME)

verify:
	@SUCCESS=1; \
	 echo -e "=> \e[1mCode analysis...\e[0m"; \
	 cppcheck --inline-suppr --error-exitcode=1 src || SUCCESS=0; \
	 echo -e "=> \e[1mDoing lint checks...\e[0m"; \
	 cpplint $(shell find src -type f) || SUCCESS=0; \
	 if [[ $$SUCCESS -eq 0 ]]; then echo -e "\e[101m=> Errors have been found, please check the outputs\e[0m"; fi

cserver_setup:
	@echo -e "=> \e[1mSetting up the clang/ccls environment...\e[0m"
	@cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -S . -B tmp > /dev/null 2> /dev/null
	@cp tmp/compile_commands.json .
	@rm -Rf tmp

clean:
	@echo -e "=> \e[1mCleaning $(BIN_FOLDER)/\e[0m"
	@rm -Rf $(BIN_FOLDER)

INSTALL_PREFIX := /usr

BIN_NAME := aging
BIN_FOLDER := build
SRC_FOLDER := .

MAKE := make

$(MAKE): $(BIN_FOLDER)
	@echo -e "=> \e[1mCompiling and linking $(BIN_NAME)\e[0m"
	@env --chdir=$(BIN_FOLDER) $(MAKE) --no-print-directory

$(BIN_FOLDER):
	@echo -e "=> \e[1mBuilding CMake configuration...\e[0m"
	@cmake -S $(SRC_FOLDER) -B $(BIN_FOLDER)

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

clean:
	@echo -e "=> \e[1mCleaning $(BIN_FOLDER)/\e[0m"
	@rm -Rf $(BIN_FOLDER)

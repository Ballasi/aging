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

clean:
	@echo -e "=> \e[1mCleaning $(BIN_FOLDER)/\e[0m"
	@rm -Rf $(BIN_FOLDER)

PROJ_NAME = tinygl
export PROJ_LIB_SO = lib$(PROJ_NAME).so
export PROJ_LIB_A = lib$(PROJ_NAME).a
export LIB_FOLDER = .libs
SRC_FOLDER = src
INST_FOLDER = /usr/local


.PHONY: all
all:
	$(MAKE) -C $(SRC_FOLDER)


.PHONY: clean
clean:
	$(MAKE) -C src clean


.PHONY: install
install:
	cp -f $(SRC_FOLDER)/$(LIB_FOLDER)/$(PROJ_LIB_SO) $(INST_FOLDER)/lib/$(PROJ_LIB_SO)
	cp -f $(SRC_FOLDER)/$(LIB_FOLDER)/$(PROJ_LIB_A) $(INST_FOLDER)/lib/$(PROJ_LIB_A)
	mkdir -p $(INST_FOLDER)/include/$(PROJ_NAME)
	cp -r ./include/* $(INST_FOLDER)/include/$(PROJ_NAME)
	chmod +r $(INST_FOLDER)/include/$(PROJ_NAME)/GL/*.h

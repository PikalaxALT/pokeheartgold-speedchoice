DATA_RESDAT_DIR = files/data/resdat
DATA_RESDAT_NARC = $(DATA_RESDAT_DIR).narc

DATA_RESDAT_JSON = $(wildcard $(DATA_RESDAT_DIR)/*.json)
DATA_RESDAT_BIN = $(DATA_RESDAT_JSON:%.json=%.bin)

$(DATA_RESDAT_BIN): %.bin: %.json $(DATA_RESDAT_DIR).json.txt | $(WORK_DIR)/include/global.h
	$(JSONPROC) $^ $*.c
	$(WINE) $(MWCC) $(MWCFLAGS) -c -o $*.o $*.c
	$(O2NARC) $*.o $@ -f
	@$(RM) $*.c $*.o

$(DATA_RESDAT_NARC): $(DATA_RESDAT_BIN)
	$(KNARC) -d $(DATA_RESDAT_DIR) -p $@ -i

clean-resdat:
	$(RM) $(DATA_RESDAT_NARC) $(DATA_RESDAT_BIN)

.PHONY: clean-resdat
clean-filesystem: clean-resdat

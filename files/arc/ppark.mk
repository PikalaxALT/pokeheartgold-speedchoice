ARC_PPARK_DIR := files/arc/ppark

PPARK_NARC := $(ARC_PPARK_DIR).narc
PPARK_JSON := $(ARC_PPARK_DIR).json
PPARK_TEMPLATE := $(ARC_PPARK_DIR).json.txt

$(PPARK_NARC): %.narc: $(PPARK_JSON) $(PPARK_TEMPLATE)
	$(JSONPROC) $^ $*.c
	$(WINE) $(MWCC) $(MWCFLAGS) -c -o $*.o $*.c
	$(O2NARC) $*.o $@ -n
	@$(RM) $*.o $*.c

$(PPARK_JSON): | $(WORK_DIR)/include/global.h

clean-ppark:
	$(RM) $(PPARK_NARC) $(PPARK_NARC:%.narc=%.c) $(PPARK_NARC:%.narc=%.o)

.PHONY: clean-ppark
clean-filesystem: clean-ppark

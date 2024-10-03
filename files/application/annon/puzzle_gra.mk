ALPH_PUZZLE_DIR := files/application/annon/puzzle_gra
ALPH_PUZZLE_FILES := \
	puzzle_gra_00000000.NCLR \
	puzzle_gra_00000001.NCGR \
	puzzle_gra_00000002.NCER \
	puzzle_gra_00000003.NANR \
	puzzle_gra_00000004.NCGR \
	puzzle_gra_00000005.NCGR \
	puzzle_gra_00000006.NCGR \
	puzzle_gra_00000007.NCGR \
	puzzle_gra_00000008.NCER \
	puzzle_gra_00000009.NANR \
	puzzle_gra_00000010.NCLR \
	puzzle_gra_00000011.NCGR \
	puzzle_gra_00000012.NSCR \
	puzzle_gra_00000013.NSCR \
	puzzle_gra_00000014.NSCR \
	puzzle_gra_00000015.NSCR

$(ALPH_PUZZLE_DIR).narc: $(addprefix $(ALPH_PUZZLE_DIR)/,$(ALPH_PUZZLE_FILES))
	$(KNARC) -p $@ -d $(ALPH_PUZZLE_DIR) -i

CLOBBER_SIZE_VERSION101_NCGR_FILES += \
	$(ALPH_PUZZLE_DIR)/puzzle_gra_00000001.NCGR \
	$(ALPH_PUZZLE_DIR)/puzzle_gra_00000004.NCGR \
	$(ALPH_PUZZLE_DIR)/puzzle_gra_00000005.NCGR \
	$(ALPH_PUZZLE_DIR)/puzzle_gra_00000006.NCGR \
	$(ALPH_PUZZLE_DIR)/puzzle_gra_00000007.NCGR

VERSION101_SOPC_NCGR_FILES += \
	$(ALPH_PUZZLE_DIR)/puzzle_gra_00000011.NCGR

4BPP_NOPAD_PCMP_NCLR_FILES += \
	$(ALPH_PUZZLE_DIR)/puzzle_gra_00000000.NCLR \
	$(ALPH_PUZZLE_DIR)/puzzle_gra_00000010.NCLR

clean-puzzle-gra:
	$(RM) $(ALPH_PUZZLE_DIR).narc \
		$(ALPH_PUZZLE_DIR)/puzzle_gra_00000001.NCGR \
		$(ALPH_PUZZLE_DIR)/puzzle_gra_00000004.NCGR \
		$(ALPH_PUZZLE_DIR)/puzzle_gra_00000005.NCGR \
		$(ALPH_PUZZLE_DIR)/puzzle_gra_00000006.NCGR \
		$(ALPH_PUZZLE_DIR)/puzzle_gra_00000007.NCGR \
		$(ALPH_PUZZLE_DIR)/puzzle_gra_00000000.NCLR \
		$(ALPH_PUZZLE_DIR)/puzzle_gra_00000010.NCLR \
		$(ALPH_PUZZLE_DIR)/puzzle_gra_00000011.NCGR

.PHONY: clean-puzzle-gra
clean-filesystem: clean-puzzle-gra

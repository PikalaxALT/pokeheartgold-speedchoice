POKE_ICON_DIR := files/poketool/icongra/poke_icon
POKE_ICON_NARC := $(POKE_ICON_DIR)/poke_icon.narc

POKE_ICON_GFX_FLAGS_ICON := -clobbersize -version101
POKE_ICON_GFX_FLAGS_PAL := -bitdepth 4

POKE_ICON_PAL_FILES := $(wildcard $(POKE_ICON_DIR)/*.pal)
POKE_ICON_PAL_OBJS := $(patsubst $(POKE_ICON_DIR)/%.pal,$(POKE_ICON_DIR)/%.NCLR,$(POKE_ICON_PAL_FILES))
POKE_ICON_ANIM_FILES := $(POKE_ICON_DIR)/poke_icon_00000001.json $(POKE_ICON_DIR)/poke_icon_00000003.json $(POKE_ICON_DIR)/poke_icon_00000005.json
POKE_ICON_ANIM_OBJS := $(patsubst $(POKE_ICON_DIR)/%.json,$(POKE_ICON_DIR)/%.NANR,$(POKE_ICON_ANIM_FILES))
POKE_ICON_CELL_FILES := $(POKE_ICON_DIR)/poke_icon_00000002.json $(POKE_ICON_DIR)/poke_icon_00000004.json $(POKE_ICON_DIR)/poke_icon_00000006.json
POKE_ICON_CELL_OBJS := $(patsubst $(POKE_ICON_DIR)/%.json,$(POKE_ICON_DIR)/%.NCER,$(POKE_ICON_CELL_FILES))
POKE_ICON_ICON_FILES := $(wildcard $(POKE_ICON_DIR)/*.png)
POKE_ICON_ICON_OBJS := $(patsubst $(POKE_ICON_DIR)/%.png,$(POKE_ICON_DIR)/%.NCGR,$(POKE_ICON_ICON_FILES))

$(POKE_ICON_DIR)/%.NCLR: $(POKE_ICON_DIR)/%.pal
	$(GFX) $< $@ $(POKE_ICON_GFX_FLAGS_PAL)

$(POKE_ICON_DIR)/%.NCER: $(POKE_ICON_DIR)/%.json
	$(GFX) $< $@

$(POKE_ICON_DIR)/%.NANR: $(POKE_ICON_DIR)/%.json
	$(GFX) $< $@

$(POKE_ICON_DIR)/%.NCGR: $(POKE_ICON_DIR)/%.png
	$(GFX) $< $@ $(POKE_ICON_GFX_FLAGS_ICON)

$(POKE_ICON_NARC): %.narc: $(POKE_ICON_PAL_OBJS) $(POKE_ICON_ANIM_OBJS) $(POKE_ICON_CELL_OBJS) $(POKE_ICON_ICON_OBJS)
	$(KNARC) -d $(POKE_ICON_DIR) -p $@ -i

clean-poke-icon:
	$(RM) $(POKE_ICON_NARC) $(POKE_ICON_PAL_OBJS) $(POKE_ICON_ANIM_OBJS) $(POKE_ICON_CELL_OBJS) $(POKE_ICON_ICON_OBJS)

.PHONY: clean-poke-icon
clean-filesystem: clean-poke-icon

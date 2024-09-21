MSGDATA_SPEEDCHOICE_MSG_DIR := $(MSGDATA_DIR)/msg_speedchoice

MSGFILE_SPEEDCHOICE_GMM := $(sort $(wildcard $(MSGDATA_SPEEDCHOICE_MSG_DIR)/*.gmm))
MSGFILE_SPEEDCHOICE_BIN := $(patsubst %.gmm,%.bin,$(MSGFILE_SPEEDCHOICE_GMM))
MSGFILE_SPEEDCHOICE_H   := $(patsubst %.gmm,%.h,$(MSGFILE_SPEEDCHOICE_GMM))

$(MSGDATA_SPEEDCHOICE_MSG_DIR).narc: %.narc: $(MSGFILE_SPEEDCHOICE_BIN)

$(MSGFILE_SPEEDCHOICE_BIN): MSGKEY = $(shell echo "$*.gmm" | python -c "from sys import stdin; from binascii import crc_hqx; print(hex(crc_hqx(stdin.read().strip().encode(), 65535)))")
$(MSGFILE_SPEEDCHOICE_BIN): MSGENCFLAGS = -e -c charmap.txt --gmm -H $*.h -k $(MSGKEY)
$(MSGFILE_SPEEDCHOICE_BIN): %.bin: %.gmm charmap.txt
	$(MSGENC) $(MSGENCFLAGS) $< $@

$(MSGFILE_SPEEDCHOICE_H): %.h: %.bin

FS_CLEAN_TARGETS += $(MSGDATA_SPEEDCHOICE_MSG_DIR).narc $(MSGFILE_SPEEDCHOICE_BIN) $(MSGFILE_SPEEDCHOICE_H)

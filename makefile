PROJECT     = ElfViewer
PROJECT_LC  = elfviewer

WORKDIR = .

C_LIST  = elfdump.cpp viewer.cpp
RC_LIST += viewer.rc
SOURCEPATH = src
INCLUDEPATH = $(WORKDIR) $(WORKDIR)/src $(WORKDIR)/3p_src/elfio-3.3

CC = gcc.exe
CXX = g++.exe
AR = ar.exe
LD = g++.exe
WINDRES = windres.exe

INC = $(addprefix -I,$(INCLUDEPATH))
CFLAGS = -w -fpermissive -DBUILD_DLL -std=c++11 -DPROJECT=$(PROJECT) -DPROJECT_LC=$(PROJECT_LC) $(INC)
RESINC =
LIBDIR =
LIB = -lgdi32
LDFLAGS = -static -static-libgcc -static-libstdc++ -Wl,--subsystem,windows -Wl,--kill-at

CFLAGS32 = -m32 -mwin32
CFLAGS64 = -m64 -D__X64__
LDFLAGS32 = -m32
LDFLAGS64 = -m64

CFLAGS_DEBUG = -Wall -g -D_DEBUG_
LDFLAGS_DEBUG =
CFLAGS_RELEASE = -O
LDFLAGS_RELEASE = -s

BUILD_DIR = $(WORKDIR)/build

DEBUG_DIR         = $(BUILD_DIR)/debug
DEBUG_OBJDIR      = $(DEBUG_DIR)/obj
DEBUG_OUT         = $(DEBUG_DIR)/$(PROJECT_LC).wlx


RELEASE_DIR       = $(BUILD_DIR)/release
RELEASE_OBJDIR    = $(RELEASE_DIR)/obj
RELEASE_OUT       = $(RELEASE_DIR)/$(PROJECT_LC).wlx

DEBUG64_DIR       = $(BUILD_DIR)/debug64
DEBUG64_OBJDIR    = $(DEBUG64_DIR)/obj
DEBUG64_OUT       = $(DEBUG64_DIR)/$(PROJECT_LC).wlx64

RELEASE64_DIR     = $(BUILD_DIR)/release64
RELEASE64_OBJDIR  = $(RELEASE64_DIR)/obj
RELEASE64_OUT     = $(RELEASE64_DIR)/$(PROJECT_LC).wlx64


OBJS = $(subst .rc,.res,$(filter %.rc,$(RC_LIST))) $(subst .c,.o,$(filter %.c,$(C_LIST))) $(subst .cpp,.o,$(filter %.cpp,$(C_LIST)))
DEBUG_OBJS      = $(addprefix $(DEBUG_OBJDIR)/,$(OBJS))
RELEASE_OBJS    = $(addprefix $(RELEASE_OBJDIR)/,$(OBJS))
DEBUG64_OBJS    = $(addprefix $(DEBUG64_OBJDIR)/,$(OBJS))
RELEASE64_OBJS  = $(addprefix $(RELEASE64_OBJDIR)/,$(OBJS))

vpath %.cpp  $(SOURCEPATH)
vpath %.c   $(SOURCEPATH)
vpath %.rc  $(SOURCEPATH)

# pull in dependency info for *existing* .o files
#-include $($(filter-out %.res,$(DEBUG_OBJS)):.o=.d)


all: debug release debug64 release64
rebuild: clean all
clean:
	if exist $(subst /,\,$(BUILD_DIR)) rd /Q /S $(subst /,\,$(BUILD_DIR))


debug: LDFLAGS+=$(LDFLAGS_DEBUG) $(LDFLAGS32)
debug: CFLAGS+=$(CFLAGS_DEBUG) $(CFLAGS32)
debug: $(DEBUG_OBJDIR) $(DEBUG_OUT)
	@echo ## $@ done
	@date /T & @time /T
$(DEBUG_OBJDIR):
	@echo ## $@
	@cmd /c if not exist $(subst /,\\,$@) md $(subst /,\\,$@)
$(DEBUG_OUT): $(DEBUG_OBJS)
	@echo ## $@
	@$(LD) $(LDFLAGS) -shared $(LIBDIR) $^ $(LIB) -o $@
-include $(subst .o,.d,$(filter-out %.res,$(DEBUG_OBJS)))
$(DEBUG_OBJDIR)/%.o: %.c
	@echo ## $@
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(DEBUG_OBJDIR)/%.o: %.cpp
	@echo ## $@
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(DEBUG_OBJDIR)/%.res: %.rc
	@echo ## $@
	@$(WINDRES) -i $< -J rc  -O coff -F pe-i386 $(INC)  -o $@
debug-clean:
	@echo ## $@
	if exist $(subst /,\,$(DEBUG_DIR)) rd /Q /S $(subst /,\,$(DEBUG_DIR))


release: LDFLAGS+=$(LDFLAGS_RELEASE) $(LDFLAGS32)
release: CFLAGS+=$(CFLAGS_RELEASE) $(CFLAGS32)
release: $(RELEASE_OBJDIR) $(RELEASE_OUT)
	@echo ## $@ done
	@date /T & @time /T
$(RELEASE_OBJDIR):
	@echo ## $@
	@cmd /c if not exist $(subst /,\\,$@) md $(subst /,\\,$@)
$(RELEASE_OUT): $(RELEASE_OBJS)
	@echo ## $@
	@$(LD) $(LDFLAGS) -shared $(LIBDIR) $^ $(LIB) -o $@
-include $(subst .o,.d,$(filter-out %.res,$(RELEASE_OBJS)))
$(RELEASE_OBJDIR)/%.o: %.c
	@echo ## $@
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(RELEASE_OBJDIR)/%.o: %.cpp
	@echo ## $@
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(RELEASE_OBJDIR)/%.res: %.rc
	@echo ## $@
	@$(WINDRES) -i $< -J rc  -O coff -F pe-i386 $(INC)  -o $@
release-clean:
	@echo ## $@
	@if exist $(subst /,\\,$(RELEASE_DIR)) rd /Q/S $(subst /,\\,$(RELEASE_DIR))


debug64: LDFLAGS+=$(LDFLAGS_DEBUG) $(LDFLAGS64)
debug64: CFLAGS+=$(CFLAGS_DEBUG) $(CFLAGS64)
debug64: $(DEBUG64_OBJDIR) $(DEBUG64_OUT)
	@echo ## $@ done
	@date /T & @time /T
$(DEBUG64_OBJDIR):
	@echo ## $@
	@cmd /c if not exist $(subst /,\\,$@) md $(subst /,\\,$@)
$(DEBUG64_OUT): $(DEBUG64_OBJS)
	@echo ## $@
	@$(LD) $(LDFLAGS) -shared $(LIBDIR) $^ $(LIB) -o $@
-include $(subst .o,.d,$(filter-out %.res,$(DEBUG64_OBJS)))
$(DEBUG64_OBJDIR)/%.o: %.c
	@echo ## $@
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(DEBUG64_OBJDIR)/%.o: %.cpp
	@echo ## $@
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(DEBUG64_OBJDIR)/%.res: %.rc
	@echo ## $@
	@$(WINDRES) -i $< -J rc  -O coff -F pe-x86-64 $(INC)  -o $@
debug64-clean:
	@echo ## $@
	@if exist $(subst /,\\,$(DEBUG64_DIR)) rd /Q/S $(subst /,\\,$(DEBUG64_DIR))


release64: LDFLAGS+=$(LDFLAGS_RELEASE) $(LDFLAGS64)
release64: CFLAGS+=$(CFLAGS_RELEASE) $(CFLAGS64)
release64: $(RELEASE64_OBJDIR) $(RELEASE64_OUT)
	@echo ## $@ done
	@date /T & @time /T
$(RELEASE64_OBJDIR):
	@echo ## $@
	@cmd /c if not exist $(subst /,\\,$@) md $(subst /,\\,$@)
$(RELEASE64_OUT): $(RELEASE64_OBJS)
	@echo ## $@
	@$(LD) $(LDFLAGS) -shared $(LIBDIR) $^ $(LIB) -o $@
-include $(subst .o,.d,$(filter-out %.res,$(RELEASE64_OBJS)))
$(RELEASE64_OBJDIR)/%.o: %.c
	@echo ## $@
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(RELEASE64_OBJDIR)/%.o: %.cpp
	@echo ## $@
	@$(CC) $(CFLAGS) -MMD -c $< -o $@
$(RELEASE64_OBJDIR)/%.res: %.rc
	@echo ## $@
	@$(WINDRES) -i $< -J rc  -O coff -F pe-x86-64 $(INC)  -o $@
release64-clean:
	@echo ## $@
	@if exist $(subst /,\\,$(RELEASE64_DIR)) rd /Q/S $(subst /,\\,$(RELEASE64_DIR))


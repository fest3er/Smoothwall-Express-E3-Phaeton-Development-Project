include ../../Makefile.conf

TARGET_DIR = /build/target

OUTPUT_FILENAME = $(MOD_NAME)-$(ARCH)-mod.tar.gz

all: clean $(OUTPUT_FILENAME)

patch.tar.gz:
	@mkdir distrib
	@(for I in \
		$(MOD_PACKAGES) \
	; do \
		if [ ! -e /$(TARGET_DIR)/smoothwall-$$I.tar.gz ]; then \
			make -C $$I clean all; \
			if [ $$? -ne 0 ]; then \
				echo "$$I FAILED"; \
				exit 1; \
			fi; \
		fi; \
		echo "Unpacking $$I ..."; \
		tar -zxf /$(TARGET_DIR)/smoothwall-$$I.tar.gz -C distrib; \
		if [ $$? -ne 0 ]; then \
			echo "Unpacking $$I FAILED"; \
			exit 1; \
		fi; \
	done; \
	);
	
	@echo "Cleaning tree ..."
	@/build/striptree distrib/

	@echo "Securing tree ..."
	@/build/securetree distrib/
	@(if [ "$(SETUIDS)" != "" ]; then \
		cd distrib/; \
		chmod u+s $(SETUIDS); \
	fi)

	@echo "Building patch.tar.gz, stand by ..."
	@tar -zcf patch.tar.gz -C distrib .

$(OUTPUT_FILENAME): patch.tar.gz
	@echo "Making mod file ..."
	@tar -zcf $(OUTPUT_FILENAME) patch.tar.gz setup $(EXTRA_MOD_FILES)

clean:
	@rm -rf distrib
	@rm -f patch.tar.gz
	@rm -f $(OUTPUT_FILENAME)

download:
	@true

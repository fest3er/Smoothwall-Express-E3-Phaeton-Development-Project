# Modified version of Original Makefile.mod from Official Smoothwall Build System
# Portions (c) 2009 Steve McNeill
#
# $Id: Makefile.mod 431 2009-10-30 12:02:29Z steve@domesticsecurity.com.au $
#

include ../../Makefile.conf
include ../../Makefile

TARGET_DIR = /build/target

OUTPUT_FILENAME = $(MOD_NAME)-$(MOD_VERSION)-$(PRODUCT_NAME)-$(PRODUCT_VERSION)-$(ARCH).tar.gz

all: cleanall $(OUTPUT_FILENAME)

patch.tar.gz:
	@ntpdate pool.ntp.org
	@mkdir distrib
	@echo "Packing up Source..."
	@mkdir -p distrib$(MOD_DIR)/archives
	@tar -jcvf  distrib$(MOD_DIR)/archives/$(MOD_NAME)-src.bz2 --exclude='*.svn*' --exclude='var' ../$(MOD_NAME)
	@echo "Building Packages..."
	@(for I in \
		$(MOD_PACKAGES) \
	; do \
		if [ ! -e $(TARGET_DIR)/smoothwall-$(MOD_NAME)-$$I.tar.gz ]; then \
			make -C $$I clean all; \
			if [ $$? -ne 0 ]; then \
				echo "$$I FAILED"; \
				exit 1; \
			fi; \
		fi; \
		echo "Unpacking $$I ..."; \
		tar -zxf $(TARGET_DIR)/smoothwall-$(MOD_NAME)-$$I.tar.gz -C distrib; \
		if [ $$? -ne 0 ]; then \
			echo "Unpacking $$I FAILED"; \
			exit 1; \
		fi; \
	done; \
	);
	
	@echo "Cleaning tree ..."
	@/build/striptree distrib/$(MOD_DIR);

	@echo "Securing tree ..."
	@/build/securetree distrib/$(MOD_DIR);
	@(if [ "$(SETUIDS)" != "" ]; then \
		cd distrib/$(MOD_DIR); \
		chmod u+s $(SETUIDS); \
	fi)
	
	@(if [ -e DETAILS ]; then \
		cp -pf DETAILS distrib/$(MOD_DIR); \
		sed -i 's/ = /=/g' distrib/$(MOD_DIR)/DETAILS; \
	fi)

	@(if [ -e .svn ]; then \
		svn log > Changelog; \
	fi)
	
	@(if [ -e Changelog ]; then \
		 cp -pf Changelog distrib/$(MOD_DIR); \
	fi)
	
	@echo "Building $(MOD_NAME)-install.bz2, stand by ..."
	@cp -pf ../CheckSI distrib$(MOD_DIR)/archives
	@cp -pf install-$(MOD_NAME)* distrib$(MOD_DIR)
	@cp -pf uninstall-$(MOD_NAME) distrib$(MOD_DIR)
	@mkdir -p distrib$(MOD_DIR)/httpd/cgi-bin
	@(echo 'AuthName "Restricted"' > distrib$(MOD_DIR)/httpd/cgi-bin/.htaccess; \
	echo 'AuthType Basic' >> distrib$(MOD_DIR)/httpd/cgi-bin/.htaccess; \
	echo 'AuthUserFile /var/smoothwall/auth/users' >> distrib$(MOD_DIR)/httpd/cgi-bin/.htaccess; \
	echo 'require user admin' >> distrib$(MOD_DIR)/httpd/cgi-bin/.htaccess);
	@mkdir -p tmp
	@chmod 777 tmp
	@cp -pf install-$(MOD_NAME) tmp
	@tar -jcf tmp/$(MOD_NAME)-install.bz2 -C distrib .

$(OUTPUT_FILENAME): patch.tar.gz
	@echo "Making mod file ..."
	#@tar -zcf $(OUTPUT_FILENAME) tmp $(EXTRA_MOD_FILES) # superseeded by makeself
	@../makeself/makeself.sh tmp "$(MOD_NAME)-$(MOD_VERSION)-$(PRODUCT_NAME)-$(PRODUCT_VERSION)-$(ARCH).run" "$(MOD_INFO)" ./install-$(MOD_NAME)

clean:
	@echo "Removing Build & tmp Directories ..."
	@rm -rf distrib
	@rm -rf var
	@rm -fr tmp
	@rm -f $(MOD_NAME)-*-$(PRODUCT_NAME)-$(PRODUCT_VERSION)-$(ARCH).tar.gz
	@rm -f $(MOD_NAME)-*-$(PRODUCT_NAME)-$(PRODUCT_VERSION)-$(ARCH).run

cleanall: clean
	@echo "Removing Prebuilt Packages ..."
	@rm -f build.out
	@rm -f build.build
	@find . -name *~ -exec rm {} \;
	@(for I in \
		$(MOD_PACKAGES) \
	; do \
		make -C $$I clean; \
		rm -f $$I/*gz; \
		rm -f $$I/*.asc; \
		if [ "$$I" != "$(MOD_NAME)_smoothd" ]; then \
			rm -fr $(find $$I/$$I* -maxdepth 0 -type d -print); \
		fi \
	done; \
	);
	@echo "Removing $(MOD_DIR) ..."
	@rm -fr $(MOD_DIR)
	@rm -f $(TARGET_DIR)/smoothwall-$(MOD_NAME)-*


download:
	@true


rebuild: cleanall
	@make

	

# vim: syn=make

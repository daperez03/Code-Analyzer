.DEFAULT_GOAL=help
.PHONY: run build clean help
DIR := $(realpath $(dir $(realpath $(lastword $(MAKEFILE_LIST)))))
STONESOUP := "$(DIR)/stonesoup"
SS_DEPS = $(STONESOUP)/dependencies
CFLAGS := -fno-stack-protector
CPPFLAGS := -DSTONESOUP_TRACE=1 -I$(SS_DEPS)/include
LDFLAGS := -L$(SS_DEPS)/lib -L$(SS_DEPS)/lib64 -Wl,-R$(STONESOUP)/workspace/install/lib -Wl,-R$(STONESOUP)/workspace/install/lib64 -Wl,-R$(SS_DEPS)/lib -Wl,-R$(SS_DEPS)/lib64 -Wl,-z,execstack  -L$(SS_DEPS)/lib64 -L$(SS_DEPS)/lib  -L$(SS_DEPS)/lib -Wl,--as-needed -Wl,-rpath,'$(STONESOUP)/workspace/install/lib',--enable-new-dtags
LIBS:= -ldl
TARGET = install/bin/postgres


run: $(TARGET) ## Run the test case

build: $(TARGET) ## Build the test case

$(TARGET): .installed
	cd src && ./configure --prefix="$(DIR)/install" --with-pgport=8432 --with-libraries="$(SS_DEPS)/lib" \
	--with-includes="$(SS_DEPS)/include" --with-openssl --with-libxml --with-libxslt --without-selinux \
	--with-readline --without-tcl --without-perl --without-python --without-gssapi --without-pam --without-krb5 \
	--with-ldap --without-bonjour --with-zlib --disable-thread-safety CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)"
	$(MAKE) -C src
	$(MAKE) -C src install

clean: ## Clean the test case
	$(MAKE) -C src clean
	rm -r install

.installed: ## Install the dependencies
	sh install-dependencies.sh

help: ## Show this help
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

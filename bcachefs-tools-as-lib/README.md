project sample using bcachefs-tools as shared library
=====================================================
Advantage of this approach is much faster linking times, which in turn shortens test-debug-deploy (play) cycle (as long as you not modifying bcachefs source code)



To make it work you need to add libbcachefs.so target in bcachefs-tools source (somewhere between rules, for example after libbcachefs.a):
```make

libbcachefs.so: libbcachefs.a
	@echo "    [LD]     $@"
	$(Q)$(CC) -shared $(LDFALGS) $(LOADLIBES) $(LDLIBS) -o $@ -Wl,--whole-archive $< -Wl,--no-whole-archive
```

After compiling shared library (you can add rule to make by default or explictitly 'make libbcachefs.so') you need modify pathes in cmakelists.txt according you system.


>don't forget bcachefs tools relays on constructor and destructor function attribute (https://gcc.gnu.org/onlinedocs/gcc-4.1.2/gcc/Function-Attributes.html)

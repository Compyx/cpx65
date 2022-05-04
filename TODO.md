# TODO

Non-exhaustive list of TODOs, to keep track of issues noticed during development
while working on other issues or additions.

## Fixes

* Fix duplicate array length macros BASE_ARRAY_SIZE() (in src/base/helpers.h)
  and base_array_len() (in src/base/mem.h).

## Additions

* Add object dependencies in Makefile.
* Add search paths to CFLAGS in Makefile to make `#include` directives less
  verbose and less dependent on header locations.
* Add `dict_size()` to base/dict.

# Blatt 7

## Ex 1

Generate new bmp file with following commands:

```sh
# copy first 54 bytes from tux.bmp to new.bmp
dd if=tux.bmp of=new.bmp bs=1 count=54

# skip 54 bytes from tux.encrypted, write with starting position 54 bytes
dd if=tux.encrypted of=new.bmp bs=1 seek=54 skip=54
```

### Explanation

We can clearly see a repeating pattern where the colors are uniform (background).
Only the parts with other colors is mangled. This can be explained that every block of bytes gets encrypted with the same
key over and over. For a block that consists of only white pixels this results in the same output every time.

### Fix

One way to fix it is to not have data with repeating blocks or increase the block size until there are no repeating blocks anymore. This is not practicable.
An other way to fix this is to use a mode that does not reuse the same key for each block but rather generates a new key for every block.

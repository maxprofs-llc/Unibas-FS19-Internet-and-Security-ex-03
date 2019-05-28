# Blatt 7

## Files

- tux.bmp
- tux.encrypted - encrypted with original algorithm
- tux-fix.encrypted - encrypted with fixed algorithm
- new.bmp - original header + body of tux.encrypted
- new-fix.bmp - original header + body of tux-fix.encrypted
- padding.py - replacement for Crypto.Util.Padding because pip doesn't install it correctly somehow?
- decrypted.bmp - same as tux.bmp: decrypted version

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

#### Explanation of fix

The AES.MODE_CBC mode changes the key in such a way that for every block that the ciphertext depends on all previous blocks of
plain text. This causes the key to not have repeat and therefore we don't have repeating patterns in our output.

### Testing the fix

```sh
# copy first 54 bytes from tux.bmp to new.bmp
dd if=tux.bmp of=new-fix.bmp bs=1 count=54

# skip 54(header) + 16(iv) bytes from tux.encrypted, write with starting position 54 bytes
dd if=tux-fix.encrypted of=new-fix.bmp bs=1 seek=54 skip=70
```

No repeating patterns are visible!

## Ex 2

### Security

No! The encryption of ab-encrypt/ab-decrypt is not information-theoretically secure. An attacker with unlimited computing power could brute force
all possible keys and find the correct one.

### Properties

#### Authenticity

No, everyone can encrypt a message with some key and transmit this message to bob.

#### Confidentiality

Yes it is guaranteed that the message is only visible to bob as long as he is the only one with the decryption key.
To read the content of the encrypted message it is neccesairy to know the key.

#### Reliable Delivery

No. There are no measures in place to detect or even correct for delivery errors. The file could be lost, duplicated, not in the correct order etc and
bob would not notice it.

#### Integrity

No. If the file was tampered with there would be no protection in place. A malicious actor could for example cut the end of a message off and
bob would assume the message had ended early without knowing it had been tampered with.

#### Tools

Yes there are tools that can guarantee all of the above. For example could messages be numerated/timestamped, then signed, then encrypted.

The numeration/timestamping of messages would prevent unreliable delivery.
Its can be detected if the message with the same number has already been received or if a message has been skipped.

Signing would prevent integrity and authenticity errors.

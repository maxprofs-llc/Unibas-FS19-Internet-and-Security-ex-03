#!/bin/python3

import argparse
from Crypto import Random
from Crypto.Cipher import AES

## USING CUSTOM PADDING BECAUSE PIP DOESN'T INSTALL THIS MODULE SOMEHOW?
#from Crypto.Util.Padding import pad, unpad  # requires 'pycrypto' module
from padding import pad, unpad  # requires 'pycrypto' module


block_size = 16            # the size of the blocks after the pad function is applied
key = b'!pre-shared-key!'  # The key used to encrypt each block


def read_encrypted(path):
    file = open(path, "rb")
    bytes = file.read()
    file.close()
    return bytes


def write_plain(bytes, path):
    file = open(path, "wb")
    file.write(bytes)
    file.close()


def main(args):
    # read encrypted data
    encrypted_text = read_encrypted(args.input)
    
    # split it into initialization vector (first 16 bytes) and data
    iv = encrypted_text[:16]
    data = encrypted_text[16:]

    # create cipher with key and iv
    cipher = AES.new(key, AES.MODE_CBC, iv)

    # decrypt and unpad data
    padded_plain_text = cipher.decrypt(data)
    plain_text = unpad(padded_plain_text, block_size)
    
    # write to file
    write_plain(plain_text, args.output)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Alice-Bob Encryption')
    parser.add_argument('-i', '--input', type=str, help="Path to plain file", required=True)
    parser.add_argument('-o', '--output', type=str, help="Path of encrypted file (should not exist)", required=True)
    args = parser.parse_args()
    main(args)

#!/bin/python3

import argparse

from aead import AEAD


block_size = 16            # the size of the blocks after the pad function is applied
key = b'FE24vRVUx5DATHCeo4NWMVQXwjK7uVLkV-P26Mfl3W0='  # The key used to encrypt each block


def read(path):
    file = open(path, "rb")
    bytes = file.read()
    file.close()
    return bytes


def write(bytes, path):
    file = open(path, "wb")
    file.write(bytes)
    file.close()

def encrypt(args):
    plain = read(args.input)

    header = plain[:54]
    data = plain[54:]

    cryptor = AEAD(key)

    ct = cryptor.encrypt(data, header)

    out = header + ct

    write(out, args.output)


def decrypt(args):
    encrypted = read(args.input)

    header = encrypted[:54]
    data = encrypted[54:]

    cryptor = AEAD(key)

    ct = cryptor.decrypt(data, header)

    out = header + ct

    write(out, args.output)
    


def main(args):

    if (args.decrypt):
        decrypt(args)
    else:
        encrypt(args)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Alice-Bob Encryption')
    parser.add_argument('-d', '--decrypt', action='store_true', help="decrypt file" )
    parser.add_argument('-i', '--input', type=str, help="Path to plain file", required=True)
    parser.add_argument('-o', '--output', type=str, help="Path of encrypted file (should not exist)", required=True)
    args = parser.parse_args()
    main(args)

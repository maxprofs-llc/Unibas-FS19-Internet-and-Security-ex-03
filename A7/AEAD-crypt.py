#!/bin/python3

import argparse

from aead import AEAD


key = b'FE24vRVUx5DATHCeo4NWMVQXwjK7uVLkV-P26Mfl3W0='  # The key used to encrypt


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
    # read plain data
    plain = read(args.input)

    # split in header and body
    header = plain[:54]
    data = plain[54:]

    # init AEAD
    cryptor = AEAD(key)

    # encrypt body and add signature of header
    ct = cryptor.encrypt(data, header)

    # concat header and encrypted body + signature
    out = header + ct

    # write blob to file
    write(out, args.output)


def decrypt(args):

    # read encrypted data
    encrypted = read(args.input)

    # split in header and encrypted body
    header = encrypted[:54]
    data = encrypted[54:]

    cryptor = AEAD(key)

    # decrypt and verify body and verify header
    ct = cryptor.decrypt(data, header)

    # write header and decrypted body to file
    out = header + ct
    write(out, args.output)
    


def main(args):

    # check if decrypt or encrypt
    # decrypt with argument -d or --decrypt
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

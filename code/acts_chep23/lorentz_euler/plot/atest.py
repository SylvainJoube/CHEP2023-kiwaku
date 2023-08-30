#!/usr/bin/env python3

# !/usr/bin/python

import argparse, sys

parser=argparse.ArgumentParser()

parser.add_argument("-bar", help="Do the bar option")
parser.add_argument("-foo", help="Foo the program")

args=parser.parse_args()

print(f"Args: {args}\nCommand Line: {sys.argv}\nfoo: {args.foo}")
print(f"Dict format: {vars(args)}")

ag = vars(args)

if "foo" in ag:
  print(["popo"])
else:
  print("no popo")

if ag["foo"] == None:
  print("no fooo")
else:
  print("foo = " + ag["foo"])
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse

argparser = argparse.ArgumentParser()
argparser.add_argument('--binary_file', required=True)
argparser.add_argument('--header_file', required=True)
argparser.add_argument('--data_name', required=True)
argparser.add_argument('--signed', action='store_true')
argparser.add_argument('--subtract_const', type=int, default=0)
argparser.add_argument('--add_const', type=int, default=0)
args = argparser.parse_args()

def getArrayType():
	if args.signed:
		return "int"
	else:
		return "unsigned int"

def renderNumber(num):
	if args.signed:
		adjusted_num = num + args.add_const - args.subtract_const
		if (adjusted_num > 32767) or (adjusted_num < -32768):
			raise Exception("Cannot fit number into this range!")
		return "{}".format(adjusted_num)
	else:
		return "0x{:04x}".format(num)

with open(args.binary_file, 'rb') as f_bin:
	with open(args.header_file, 'w') as f_header:
		total_numbers_written = 0
		numbers_written_this_col = 0
		f_header.write(getArrayType() + " " + args.data_name + "[] = {\n\t")
		bin_data = f_bin.read(2)
		while len(bin_data) == 2:
			value = int.from_bytes(bin_data, "little")
			f_header.write(renderNumber(value))
			total_numbers_written += 1
			numbers_written_this_col += 1
			bin_data = f_bin.read(2)
			if len(bin_data) == 2:
				f_header.write(",")
				if numbers_written_this_col == 10:
					numbers_written_this_col = 0;
					f_header.write("\n\t")
				else:
					f_header.write(" ")
		f_header.write("\n};\n")
		f_header.write("unsigned int " + args.data_name + "_len = " + str(total_numbers_written) + ";\n")

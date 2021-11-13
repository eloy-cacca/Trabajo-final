#!/usr/local/bin/python2.7
# encoding: utf-8
'''
LC_FW_merge -- python script used to get the MFG image

LC_FW_merge is a python script that needs the OTA firmware image and the
bootloader image, in order to create the OTA firmware image.

It defines classes_and_methods

@author:     ML

@copyright:  2015 Petra Systems. All rights reserved.

@license:    license

@contact:    mlao@lyrtron.com.ar
@deffield    updated: 04.04.2015
'''

import sys
import os
import struct as st

import intelhex

from argparse import ArgumentParser
from argparse import RawDescriptionHelpFormatter
#from __builtin__ import len  # 2.7
from builtins import len

__all__ = []
__version__ = 2.0
__date__ = '2015-04-06'
__updated__ = '2015-04-06'

DEBUG = 0

class CLIError(Exception):
    '''Generic exception to raise and log different fatal errors.'''
    def __init__(self, msg):
        super(CLIError).__init__(type(self))
        self.msg = "E: %s" % msg

    def __str__(self):
        return self.msg

    def __unicode__(self):
        return self.msg


def dbg_convert_string_to_hex(message):
    return (':'.join('{0:=#04x}'.format(ord(char)) for char in message))


def calculate_crc(str_input):

    CRCPoly = 0xA001

    result = 0
    for i in range(len(str_input)):
        data = ord(str_input[i])

        for _ in range(8):
            result_bit = result & 0x01
            result = result >> 1

            data_bit = data & 0x01
            data = data >> 1

            if(result_bit != data_bit):
                result = result ^ CRCPoly

    return result


def main(argv=None):    # IGNORE:C0111
    '''Command line options.'''

    if argv is None:
        argv = sys.argv
    else:
        sys.argv.extend(argv)

    program_name = os.path.basename(sys.argv[0])
    program_version = "v%s" % __version__
    program_build_date = str(__updated__)
    program_version_message = '%%(prog)s %s (%s)' % (program_version,
                                                     program_build_date)
    program_shortdesc = __import__('__main__').__doc__.split("\n")[1]
    program_license = '''%s

  Created by ML on %s.
  Copyright 2015 Petra Systems. All rights reserved.

  Licensed under the Apache License 2.0
  http://www.apache.org/licenses/LICENSE-2.0

  Distributed on an "AS IS" basis without warranties
  or conditions of any kind, either express or implied.

USAGE
''' % (program_shortdesc, str(__date__))

    try:
        # Setup argument parser
        parser = ArgumentParser(description=program_license,
                                formatter_class=RawDescriptionHelpFormatter)

        parser.add_argument("-a",
                            dest="main_app",
                            help="LC main application image")

        parser.add_argument("-b",
                            dest="btldr",
                            help="LC Bootloader image")

        parser.add_argument("-o",
                            dest="out",
                            help="Merged image name")

        parser.add_argument('-V', '--version',
                            action='version',
                            version=program_version_message)

        # Process arguments
        args = parser.parse_args()

        if args.main_app is None:
            sys.stdout.write('Main app image is missing... Exiting\n')
            return 1
        else:
            application = args.main_app

        if args.btldr is None:
            sys.stdout.write('Bootloader image is missing... Exiting\n')
            return 1
        else:
            bootlaoder = args.btldr

        if args.out is None:
            sys.stdout.write('Output file name is missing... Exiting\n')
            return 1
        else:
            output_name = args.out

        res = intelhex.IntelHex()
        overlap = 'error'

        # Open main app image and calculate CRC and length
        img1 = intelhex.IntelHex(application)
        img1 = img1[int("00402000", 16):]
        sys.stdout.write("\n")
        sys.stdout.write("Reading main app image\n")
        file_string = img1.tobinstr()

#        CRC = calculate_crc(file_string)                           # 2.7
#        CRC_b = bytearray(st.pack('<H', CRC))                      # 2.7
#        sys.stdout.write("CRC value of read file is %d\n" % CRC)   # 2.7

        file_length = len(file_string)
        #len_b = bytearray(st.pack('<H', file_length))
        len_b = bytearray(st.pack('<L', file_length))
        sys.stdout.write("File length is %d bytes\n" % file_length)

        # merge main app image with CRC and length values
        res.merge(img1, overlap)

        # Merge BTLDR image with Main app image
        img2 = intelhex.IntelHex(bootlaoder)
        img2 = img2[int("00400000", 16):int("00402000", 16)]
        sys.stdout.write("\nReading bootloader image\n")
        res.merge(img2, overlap)

        sys.stdout.write("Merging files and generating output\n")
        res.write_hex_file(output_name, True)
        sys.stdout.write("Script finished\n")

        return 0

    except KeyboardInterrupt:
        # handle keyboard interrupt #
        return 2

#    except Exception, e:
    except Exception as e:        
        if DEBUG:
            raise(e)
        indent = len(program_name) * " "
        sys.stderr.write(program_name + ": " + repr(e) + "\n")
        sys.stderr.write(indent + "  for help use --help")
        return 2

if __name__ == "__main__":
    sys.exit(main())

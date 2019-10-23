#!/bin/bash
qemu-system-x86_64 -S -s -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspk 

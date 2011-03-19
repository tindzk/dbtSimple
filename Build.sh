#!/bin/bash

mkdir -p Build/{Jivai,Debit,Project}

../Depend/Depend.bin              \
	build                         \
	output=httpd.bin              \
	manifest=Manifest.h           \
	main=Main.c                   \
	include=..                    \
	include=../Jivai/src          \
	add=Resources/*.c             \
	map=.:Build/Project           \
	map=../Jivai/src:Build/Jivai  \
	map=../Debit:Build/Debit      \
	optimlevel=0                  \
	dbgsym=yes                    \
	inclhdr=../Jivai/config.h     \
	link=@bfd                     \
	link=@dl

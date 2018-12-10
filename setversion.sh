#!/bin/bash
# version changing script
NAME=Libtrie
VERSION=0.1.10

FROM="$NAME v[0-9]{1,2}.[0-9]{1,2}.[0-9]{1,2}"
TO="$NAME v$VERSION"

sed -ri "s#$FROM#$TO#" src/*
sed -ri "s#$FROM#$TO#" README.md
echo $VERSION
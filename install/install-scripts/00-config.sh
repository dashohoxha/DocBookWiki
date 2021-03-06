#!/bin/bash -x

export DEBIAN_FRONTEND=noninteractive

cwd=$(dirname $0)

$cwd/10-install-additional-packages.sh

$cwd/20-make-and-install-docbookwiki.sh

$cwd/30-git-clone-docbookwiki.sh

$cwd/40-configure-docbookwiki.sh

### copy overlay files over to the system
cp -TdR $(dirname $cwd)/overlay/ /

$cwd/50-misc-config.sh

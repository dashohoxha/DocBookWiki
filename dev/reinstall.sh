#!/bin/bash
### Reinstall docbookwiki from scratch.
### Useful for testing installation scripts.

mv /var/www/docbookwiki /var/www/docbookwiki-bak

cd $(dirname $0)
cd ../install/install-scripts/

./20-make-and-install-docbookwiki.sh
./30-git-clone-docbookwiki.sh
./40-configure-docbookwiki.sh

../config.sh


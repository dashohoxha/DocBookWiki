#!/bin/bash -x
### Replace the profile docbookwiki with a version
### that is cloned from github, so that any updates
### can be retrieved easily (without having to
### reinstall the whole application).

### clone docbookwiki from github
cd /var/www/docbookwiki/profiles/
mv docbookwiki docbookwiki-bak
git clone https://github.com/dashohoxha/DocBookWiki docbookwiki

### copy contrib libraries and modules
cp -a docbookwiki-bak/libraries/ docbookwiki/
cp -a docbookwiki-bak/modules/contrib/ docbookwiki/modules/
cp -a docbookwiki-bak/themes/contrib/ docbookwiki/themes/

### cleanup
rm -rf docbookwiki-bak/
#!/bin/bash
### Clone the dev branch from
### /var/www/docbookwiki_dev/profiles/docbookwiki/

### create a symlink /var/www/DocBookWiki to the git repo
cd /var/www/
test -h DocBookWiki || ln -s docbookwiki_dev/profiles/docbookwiki/ DocBookWiki

### on the repo create a 'dev' branch
cd DocBookWiki/
git branch dev master

### clone the dev branch
cd /var/www/docbookwiki/profiles/
rm -rf docbookwiki-bak
mv docbookwiki docbookwiki-bak
git clone -b dev /var/www/DocBookWiki docbookwiki

### copy contrib libraries and modules
cp -a docbookwiki-bak/libraries/ docbookwiki/
cp -a docbookwiki-bak/modules/contrib/ docbookwiki/modules/
cp -a docbookwiki-bak/modules/libraries/ docbookwiki/modules/
cp -a docbookwiki-bak/themes/contrib/ docbookwiki/themes/

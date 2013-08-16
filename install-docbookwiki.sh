#!/bin/bash -x
### Install the Drupal profile 'docbookwiki'.

### retrieve all the projects/modules and build the application directory
#makefile="https://raw.github.com/dashohoxha/DocBookWiki/master/build-docbookwiki.make"
makefile="/var/www/DocBookWiki/build-docbookwiki.make"
appdir="/var/www/docbookwiki"
rm -rf $appdir
drush make --prepare-install --force-complete \
           --contrib-destination=profiles/btranslator \
           $makefile $appdir
cp -a $appdir/profiles/docbookwiki/{libraries/bootstrap,themes/contrib/bootstrap/}

### settings for the database and the drupal site
db_name=docbookwiki
db_user=docbookwiki
db_pass=docbookwiki
site_name="DocBookWiki"
site_mail="admin@example.com"
account_name=admin
account_pass=admin
account_mail="admin@example.com"

### create the database and user
mysql='mysql --defaults-file=/etc/mysql/debian.cnf'
$mysql -e "
    DROP DATABASE IF EXISTS $db_name;
    CREATE DATABASE $db_name;
    GRANT ALL ON $db_name.* TO $db_user@localhost IDENTIFIED BY '$db_pass';
"

### start site installation
sed -e '/memory_limit/ c memory_limit = -1' -i /etc/php5/cli/php.ini
cd $appdir
drush site-install --verbose --yes docbookwiki \
      --db-url="mysql://$db_user:$db_pass@localhost/$db_name" \
      --site-name="$site_name" --site-mail="$site_mail" \
      --account-name="$account_name" --account-pass="$account_pass" --account-mail="$account_mail"

### update to the latest version of core and modules
drush --yes pm-update

### set propper directory permissions
mkdir -p sites/default/files/
chown -R www-data: sites/default/files/
mkdir -p cache/
chown -R www-data: cache/

# protect Drupal settings from prying eyes
drupal_settings=$appdir/sites/default/settings.php
chown root:www-data $drupal_settings
chmod 640 $drupal_settings

### clone docbookwiki from git
cd $appdir/profiles/
mv docbookwiki docbookwiki-bak
#git clone https://github.com/dashohoxha/DocBookWiki docbookwiki
git clone /var/www/DocBookWiki -b dev docbookwiki

### copy contrib libraries and modules
cp -a docbookwiki-bak/libraries/ docbookwiki/
cp -a docbookwiki-bak/modules/contrib/ docbookwiki/modules/
cp -a docbookwiki-bak/modules/libraries/ docbookwiki/modules/
cp -a docbookwiki-bak/themes/contrib/ docbookwiki/themes/

### cleanup
rm -rf docbookwiki-bak/

